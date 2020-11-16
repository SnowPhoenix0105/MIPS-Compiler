#include "SimpleTargetGenerator.h"
void SimpleCodeGenerator::next_function_info()
{
	const IrElemAllocator& allocator = *allocator_ptr;
	const Ir* code;
	irelem_t label;
	func_beg_index = func_end_index - 1;
	do
	{
		code = &(ir_table_ptr->at(++func_beg_index));
		label = code->elem[0];
	} while (code->head != IrHead::label || !IrType::is_func(label) || !IrType::is_beg(label));
	func_name = allocator.func_name(label);
	func_mid_index = func_beg_index;
	do
	{
		code = &(ir_table_ptr->at(++func_mid_index));
		label = code->elem[0];
	} while (code->head != IrHead::label || !IrType::is_func(label) || !IrType::is_mid(label));
	ASSERT(3, func_name == allocator.func_name(label));
	func_end_index = func_mid_index;
	do
	{
		code = &(ir_table_ptr->at(++func_end_index));
		label = code->elem[0];
	} while (code->head != IrHead::label || !IrType::is_func(label) || !IrType::is_end(label));
	ASSERT(3, func_name == allocator.func_name(label));
}

void SimpleCodeGenerator::init_global()
{
	const IrElemAllocator& allocator = *allocator_ptr;
	const IrTable& ir_table = *ir_table_ptr;
	unsigned offset = 0;
	buffer << ".data" << endl;
	buffer << "__GP__:" << endl;
	bool flag = true;
	for (size_t i = 0; flag; ++i)
	{
		const auto& code = ir_table.at(i);
		switch (code.head)
		{
		case IrHead::label:
			// 变量声明结束
			flag = false;
			break;
		case IrHead::gvar:
		{
			// 全局变量
			irelem_t var = code.elem[0];
			string var_label = allocator.var_to_string(var);
			global_var_offset_table.insert(make_pair(var, offset));
			offset += 4;
			//buffer << var_label << ":\t.word";
			buffer << ".word";
			if (ir_table.at(i + 1).head == IrHead::init)
			{
				buffer << '\t' << allocator.imm_to_value(ir_table.at(++i).elem[0]) << endl;
			}
			else
			{
				buffer << endl;
			}
			break;
		}
		case IrHead::arr:
		{
			// 全局数组
			irelem_t arr = code.elem[0];
			bool is_int = code.elem[1] == IrType::_int;
			int size = allocator.imm_to_value(code.elem[2]);
			allocator_ptr->set_arr_value(arr, offset);
			int space = size * (is_int ? 4 : 1);
			space = (space + 3) & ~3;			// up tp 4*n
			offset += space;
			// 无初始化
			if (ir_table.at(i + 1).head != IrHead::init)
			{
				buffer << ".space " << space << endl;
				break;
			}
			// 有初始化
			const char* head = is_int ? ".word " : ".byte ";
			int end = i + size;
			while (i != end)
			{
				const auto& init = ir_table.at(++i);
				ASSERT(3, init.head == IrHead::init);
				irelem_t imm = init.elem[0];
				unsigned value = allocator.imm_to_value(imm);
				buffer << head << value << endl;
			}
			if (is_int)
			{
				break;
			}
			// 字对齐
			for (int j = 0; j != space - size; ++j)
			{
				buffer << head << 0 << endl;
			}
			break;
		}
		default:
			PANIC();
		}
	}
	int str_count = 0;
	for (const auto& p : allocator.get_string_map())
	{
		string l = "string_";
		l += to_string(str_count++);
		string_label_table.insert(make_pair(p.second, l));
		buffer << mips.label(l) << endl;
		buffer << ".asciiz " << p.first << endl;
	}
	buffer << "\n\n\n\n\n.text" << endl;
	buffer << mips.j("func_beg_main") << endl;
}

void SimpleCodeGenerator::init_func()
{
	const IrElemAllocator& allocator = *allocator_ptr;
	const IrTable& ir_table = *ir_table_ptr;

	// 遍历 mid -> end , 统计局部var数量
	unordered_set<irelem_t> var_set;
	for (size_t i = func_mid_index + 1; i != func_end_index; ++i)
	{
		const auto& code = ir_table.at(i);
		for (size_t j = 0; j < 3; ++j)
		{
			if (allocator.is_local_var(code.elem[j]))
			{
				var_set.insert(code.elem[j]);
			}
		}
	}
	// 遍历 beg -> mid , 统计形参, 初始化数组
   	vector<irelem_t> param_list;
	unsigned offset = 0;
	for (size_t i = func_beg_index + 1; i != func_mid_index; ++i)
	{
		const auto& code = ir_table.at(i);
		switch (code.head)
		{
		case IrHead::func:
			break;
		case IrHead::param:
		{
			ASSERT(4, IrType::is_var(code.elem[0]));
			param_list.push_back(code.elem[0]);
			break;
		}
		case IrHead::arr:
		{
			irelem_t arr = code.elem[0];
			bool is_int = code.elem[1] == IrType::_int;
			int size = allocator.imm_to_value(code.elem[2]);
			allocator_ptr->set_arr_value(arr, offset);
			int space = size * (is_int ? 4 : 1);
			space = (space + 3) & ~3;			// up tp 4*n
			unsigned off = offset;
			offset += space;
			// 无初始化
			if (ir_table.at(i + 1).head != IrHead::init)
			{
				break;
			}
			// 有初始化
			const char* head = is_int ? "sw " : "sb ";
			unsigned delta_off = is_int ? 4 : 1;
			int end = i + size;
			while (i != end)
			{
				const auto& init = ir_table.at(++i);
				ASSERT(3, init.head == IrHead::init);
				irelem_t imm = init.elem[0];
				unsigned value = allocator.imm_to_value(imm);
				buffer << mips.li("$t0", value) << endl;
				buffer << mips.mem_op(head, "$t0", "$sp", off) << endl;
				off += delta_off;
			}
			break;
		}
		default:
			break;
		}
	}
	// 清除需要特殊分配的寄存器
	for (auto p : param_list)
	{
		var_set.erase(p);
	}
	var_set.erase(allocator.sp());
	var_set.erase(allocator.gp());
	var_set.erase(allocator.ret());
	var_set.erase(allocator.zero());
	for (auto v : var_set)
	{
		func_var_offset_table.insert(make_pair(v, offset));
		offset += 4;
	}
	for (auto p : param_list)
	{
		func_var_offset_table.insert(make_pair(p, offset));
		offset += 4;
	}
	func_var_offset_table.insert(make_pair(allocator.ret(), offset));
	offset += 4;
	// $ra
	offset += 4;
	stack_size = offset;
}

void SimpleCodeGenerator::beg_main()
{
	buffer << mips.label("func_beg_main") << endl;
	buffer << mips.la("$gp", "__GP__") << endl;
	buffer << mips.subu("$sp", "$sp", stack_size) << endl;
}

void SimpleCodeGenerator::beg_func()
{
	buffer << mips.label(allocator_ptr->label_to_string(ir_table_ptr->at(func_beg_index).elem[0])) << endl;
	buffer << mips.sw("$ra", "$sp", stack_size - 4) << endl;
	buffer << mips.subu("$sp", "$sp", stack_size) << endl;
}

void SimpleCodeGenerator::end_main()
{
	buffer << mips.label("func_end_main") << endl;
	buffer << mips.li("$v0", 10) << endl;
	buffer << mips.syscall() << endl;
}

void SimpleCodeGenerator::end_func()
{
	buffer << mips.label(allocator_ptr->label_to_string(ir_table_ptr->at(func_end_index).elem[0])) << endl;
	buffer << mips.lw("$ra", "$sp", stack_size - 4) << endl;
	buffer << mips.addu("$sp", "$sp", stack_size) << endl;
	buffer << mips.jr() << endl;
}

void SimpleCodeGenerator::load_val_to_reg(const string& reg, irelem_t val)
{
	ASSERT(4, IrType::is_val(val));
	const IrElemAllocator& allocator = *allocator_ptr;
	ostringstream oss;
	if (IrType::is_cst(val))
	{
		buffer << mips.li(reg, allocator.cst_to_value(val)) << endl;
		return;
	}
	ASSERT(4, IrType::is_var(val));
	if (allocator.is_reserved_var(val))
	{
		buffer << mips.addu(reg, "$0", allocator.var_to_string(val)) << endl;
		return;
	}
	if (allocator.is_global_var(val))
	{
		buffer << mips.lw(reg, "$gp", global_var_offset_table.at(val)) << endl;
		return;
	}
	buffer << mips.lw(reg, "$sp", func_var_offset_table.at(val)) << endl;
	return;
}

void SimpleCodeGenerator::save_reg_to_var(const string& reg, irelem_t var)
{
	const IrElemAllocator& allocator = *allocator_ptr;
	ostringstream oss;
	ASSERT(4, IrType::is_var(var));
	if (allocator.is_reserved_var(var))
	{
		ASSERT(4, var == allocator.ret());
		buffer << mips.addu("$v0", "$0", reg) << endl;
		return;
	}
	if (allocator.is_global_var(var))
	{
		buffer << mips.sw(reg, "$gp", global_var_offset_table.at(var)) << endl;
		return;
	}
	buffer << mips.sw(reg, "$sp", func_var_offset_table.at(var)) << endl;
	return;
}

#define CASE(head, type)									\
case IrHead::head:											\
	load_val_to_reg("$t1", code.elem[1]);					\
	load_val_to_reg("$t2", code.elem[2]);					\
	buffer << mips.type("$t0", "$t1", "$t2") << endl;		\
	save_reg_to_var("$t0", code.elem[0]);					\
	break		

void SimpleCodeGenerator::func_body()
{
	// buffer << allocator_ptr->label_to_string(ir_table_ptr->at(func_mid_index).elem[0]) << ':' << endl;
	const IrElemAllocator& allocator = *allocator_ptr;
	const IrTable& ir_table = *ir_table_ptr;

	for (size_t index = func_mid_index; index < func_end_index; ++index)
	{
		const auto& code = ir_table.at(index);
		switch (code.head)
		{
		case IrHead::label:
			buffer << allocator.label_to_string(code.elem[0]) << ':' << endl;
			break;
		CASE(add, addu);
		CASE(sub, subu);
		CASE(mult, mul);
		CASE(div, div);
		CASE(_and, _and);
		CASE(_or, _or);
		CASE(_nor, _nor);
		CASE(_xor, _xor);
		CASE(sl, sll);
		CASE(sr, sra);
		CASE(less, slt);
		case IrHead::lw:
			load_val_to_reg("$t1", code.elem[1]);
			buffer << mips.lw("$t0", "$t1", allocator.cst_to_value(code.elem[2])) << endl;
			save_reg_to_var("$t0", code.elem[0]);
			break;
		case IrHead::lb:
			load_val_to_reg("$t1", code.elem[1]);
			buffer << mips.lb("$t0", "$t1", allocator.cst_to_value(code.elem[2])) << endl;
			save_reg_to_var("$t0", code.elem[0]);
			break;
		case IrHead::sw:
			load_val_to_reg("$t0", code.elem[0]);
			load_val_to_reg("$t1", code.elem[1]);
			buffer << mips.sw("$t0", "$t1", allocator.cst_to_value(code.elem[2])) << endl;
			break;
		case IrHead::sb:
			load_val_to_reg("$t0", code.elem[0]);
			load_val_to_reg("$t1", code.elem[1]);
			buffer << mips.sb("$t0", "$t1", allocator.cst_to_value(code.elem[2])) << endl;
			break;
		case IrHead::beq:
			load_val_to_reg("$t0", code.elem[0]);				
			load_val_to_reg("$t1", code.elem[1]);			
			buffer << mips.beq("$t0", "$t1", allocator.label_to_string(code.elem[2])) << endl;
			break;
		case IrHead::bne:
			load_val_to_reg("$t0", code.elem[0]);
			load_val_to_reg("$t1", code.elem[1]);
			buffer << mips.bne("$t0", "$t1", allocator.label_to_string(code.elem[2])) << endl;
			break;
		case IrHead::_goto:
			buffer << mips.j(allocator.label_to_string(code.elem[0])) << endl;
			break;
		case IrHead::call:
			buffer << mips.sw("$v0", "$sp", stack_size - 8) << endl;
			buffer << mips.jal(allocator.label_to_string(code.elem[0])) << endl;
			buffer << mips.lw("$v0", "$sp", stack_size - 8) << endl;
			break;
		case IrHead::ret:
			buffer << mips.j(allocator.label_to_string(ir_table.at(func_end_index).elem[0])) << endl;
			break;
		case IrHead::scanf:
			buffer << mips.sw("$v0", "$sp", stack_size - 8) << endl;
			buffer << mips.li("$v0", code.elem[1] == IrType::_int ? mips.read_int : mips.read_cahr) << endl;
			buffer << mips.syscall() << endl;
			save_reg_to_var("$v0", code.elem[0]);
			buffer << mips.lw("$v0", "$sp", stack_size - 8) << endl;
			break;
		case IrHead::printf:
		{
			bool str = code.elem[0] != IrType::NIL;
			bool val = code.elem[1] != IrType::NIL;
			buffer << mips.sw("$v0", "$sp", stack_size - 8) << endl;
			if (str)
			{
				const auto& label = string_label_table.at(code.elem[0]);
				buffer << mips.la("$a0", label) << endl;
				buffer << mips.li("$v0", mips.print_string) << endl;
				buffer << mips.syscall() << endl;
			}
			if (val)
			{
				load_val_to_reg("$a0", code.elem[1]);
				buffer << mips.li("$v0", code.elem[2] == IrType::_int ? mips.print_int : mips.print_char) << endl;
				buffer << mips.syscall() << endl;
			}
			buffer << mips.li("$a0", '\n') << endl;
			buffer << mips.li("$v0", mips.print_char) << endl;
			buffer << mips.syscall() << endl;
			buffer << mips.lw("$v0", "$sp", stack_size - 8) << endl;
			break;
		}
		default:
			PANIC();
		}
	}
}

string SimpleCodeGenerator::fresh_buffer()
{
	string ret = buffer.str();
	buffer.str(string());
	buffer.clear();
	return ret;
}

void SimpleCodeGenerator::translate(ostream& os)
{
	init_global();
	os << fresh_buffer() << endl;
	// os << "\n\n\n\n\n.text" << endl;
	bool flag = true;
	while (flag)
	{
		next_function_info();
		init_func();
		string arr_init_codes = fresh_buffer();
		if (func_name == "main")
		{
			flag = false;
			beg_main();
		}
		else
		{
			beg_func();
		}
		os << fresh_buffer();

		os << arr_init_codes;
		func_body();

		os << fresh_buffer();
		if (flag)
		{
			end_func();
		}
		else
		{
			end_main();
		}
		os << fresh_buffer();

	}
}
