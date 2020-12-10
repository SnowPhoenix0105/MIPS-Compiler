#include "GCPTargetGenerator.h"
#include "Graph.h"
#include <stack>
#include <set>

using std::stack;
using std::set;



void GCPTargetGenerator::init_global()
{
	const IrElemAllocator& allocator = *allocator_ptr;
	const IrTable& ir_table = *ir_table_ptr;
	unsigned offset = 0;
	buffer << ".data" << endl;
	buffer << "__GP__:" << endl;
	bool flag = true;
#ifdef DEBUG_LEVEL
	std::cout << "     ____GLOBAL_____" << endl;
	std::cout << "+ - - - - - - - - - - - + 0" << endl;
#endif // DEBUG_LEVEL

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
#ifdef DEBUG_LEVEL
			std::cout << "|\t" << allocator.val_to_string(var) << ' ' << endl;
			std::cout << "+ - - - - - - - - - + " << offset << endl;
#endif // DEBUG_LEVEL
			//buffer << var_label << ":\t.word";
			buffer << ".word";
			if (ir_table.at(i + 1).head == IrHead::init)
			{
				buffer << '\t' << allocator.imm_to_value(ir_table.at(++i).elem[0]) << endl;
			}
			else
			{
				buffer << '\t' << 0 << endl;
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
#ifdef DEBUG_LEVEL
			std::cout << "|\t" << allocator.val_to_string(arr) << ' ' << endl;
			std::cout << "+ - - - - - - - - - + " << offset << endl;
#endif // DEBUG_LEVEL
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
#ifdef DEBUG_LEVEL
	std::cout << "+ - - - - - - - - - + " << offset << endl;
	std::cout << "\n\n\n\n" << endl;
#endif // DEBUG_LEVEL
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


void GCPTargetGenerator::next_function_info()
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

void GCPTargetGenerator::init_func()
{
	const IrElemAllocator& allocator = *allocator_ptr;
	const IrTable& ir_table = *ir_table_ptr;
	const unordered_set<irelem_t> sregs({
		allocator.reg(Reg::s0),
		allocator.reg(Reg::s1),
		allocator.reg(Reg::s2),
		allocator.reg(Reg::s3),
		allocator.reg(Reg::s4),
		allocator.reg(Reg::s5),
		allocator.reg(Reg::s6),
		allocator.reg(Reg::s7)
		});


	// 遍历 mid -> end , 统计局部var数量, 统计使用过的 $sx, 统计是否出现 call
	set<irelem_t> var_set;
	used_sregs.clear();
	is_leaf = true;
	for (size_t i = func_mid_index + 1; i != func_end_index; ++i)
	{
		const auto& code = ir_table.at(i);
		for (size_t j = 0; j < 3; ++j)
		{
			irelem_t elem = code.elem[j];
			if (sregs.count(elem) != 0)
			{
				used_sregs.insert(elem);
			}
#ifdef DEBUG_LEVEL
			bool b = 
				code.head == IrHead::protect 
				|| code.head == IrHead::reload
				|| !IrType::is_var(elem) || allocator.is_reg(elem);
			ASSERT(4, b);
#endif // DEBUG_LEVEL
		}
		if (code.head == IrHead::call)
		{
			is_leaf = false;
		}
		else if (code.head == IrHead::protect && allocator.is_local_var(code.elem[1]))
		{
			var_set.insert(code.elem[1]);
		}
	}

#ifdef DEBUG_LEVEL
	std::cout << "     " << allocator.label_to_string(ir_table.at(func_beg_index).elem[0]) << endl;
	std::cout << "+ - - - - - - - - - - - + 0" << endl;
#endif // DEBUG_LEVEL
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
#ifdef DEBUG_LEVEL
			std::cout << "|\t" << allocator.val_to_string(arr) << ' ' << endl;
			std::cout << "+ - - - - - - - - - + " << offset << endl;
#endif // DEBUG_LEVEL
			// 无初始化
			if (ir_table.at(i + 1).head != IrHead::init)
			{
				break;
			}
			// 有初始化

			// TODO
			if (is_int)
			{
				int end = i + size;
				while (i != end)
				{
					const auto& init = ir_table.at(++i);
					ASSERT(3, init.head == IrHead::init);
					irelem_t imm = init.elem[0];
					unsigned value = allocator.imm_to_value(imm);
					buffer << mips.li("$t0", value) << endl;
					buffer << mips.mem_op("sw ", "$t0", "$sp", off) << endl;
					off += 4;
				}
				break;
			}
			else
			{
				uint8_t* table8 = new uint8_t[space];
				uint32_t* table32 = reinterpret_cast<uint32_t*>(table8);
				for (size_t i = 0; i != space / 4; ++i)
				{
					table32[i] = static_cast<uint32_t>(0u);
				}
				
				for (size_t idx = 0; idx != size; ++idx)
				{
					const auto& init = ir_table.at(++i);
					ASSERT(3, init.head == IrHead::init);
					irelem_t imm = init.elem[0];
					unsigned value = allocator.imm_to_value(imm);
					table8[idx] = static_cast<uint8_t>(value);
				}
				for (size_t idx = 0; idx != space / 4; ++idx)
				{
					uint32_t value = table32[idx];
					buffer << mips.li("$t0", value) << endl;
					buffer << mips.mem_op("sw ", "$t0", "$sp", off) << endl;
					off += 4;
				}
				delete[] table8;
			}
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
	ASSERT(4, var_set.count(allocator.sp()) == 0);
	ASSERT(4, var_set.count(allocator.gp()) == 0);
	ASSERT(4, var_set.count(allocator.zero()) == 0);
	ASSERT(4, var_set.count(allocator.ret()) == 0);
	func_var_offset_table.clear();
	for (auto v : var_set)
	{
		func_var_offset_table.insert(make_pair(v, offset));
		offset += 4;
#ifdef DEBUG_LEVEL
		std::cout << "|\t" << allocator.val_to_string(v) << ' ' << endl;
		std::cout << "+ - - - - - - - - - + " << offset << endl;
#endif // DEBUG_LEVEL
	}
	for (auto sreg : used_sregs)
	{
		func_var_offset_table.insert(make_pair(sreg, offset));
		offset += 4;
#ifdef DEBUG_LEVEL
		std::cout << "|\t" << allocator.val_to_string(sreg) << ' ' << endl;
		std::cout << "+ - - - - - - - - - + " << offset << endl;
#endif // DEBUG_LEVEL
	}
	for (auto p : param_list)
	{
		func_var_offset_table.insert(make_pair(p, offset));
		offset += 4;
#ifdef DEBUG_LEVEL
		std::cout << "|\t" << allocator.val_to_string(p) << ' ' << endl;
		std::cout << "+ - - - - - - - - - + " << offset << endl;
#endif // DEBUG_LEVEL
	}
#ifdef DEBUG_LEVEL
	std::cout << "+ - - - - - - - - - + " << offset << endl;
	std::cout << "\n\n\n\n" << endl;
#endif // DEBUG_LEVEL
	// $ra
	offset += 4;
	stack_size = offset;
}


void GCPTargetGenerator::beg_func()
{
	buffer << mips.label(allocator_ptr->label_to_string(ir_table_ptr->at(func_beg_index).elem[0])) << endl;
	buffer << mips.subu("$sp", "$sp", stack_size) << endl;
	if (!is_leaf) buffer << mips.sw("$ra", "$sp", stack_size - 4) << endl;
	for (irelem_t sreg : used_sregs)
	{
		buffer << mips.sw(allocator_ptr->var_to_string(sreg), "$sp", func_var_offset_table.at(sreg)) << endl;
	}
}

void GCPTargetGenerator::ret_func()
{
	// buffer << mips.label(allocator_ptr->label_to_string(ir_table_ptr->at(func_end_index).elem[0])) << endl;
	for (irelem_t sreg : used_sregs)
	{
		buffer << mips.lw(allocator_ptr->var_to_string(sreg), "$sp", func_var_offset_table.at(sreg)) << endl;
	}
	if (!is_leaf) buffer << mips.lw("$ra", "$sp", stack_size - 4) << endl;
	buffer << mips.addu("$sp", "$sp", stack_size) << endl;
	buffer << mips.jr() << endl;
}

void GCPTargetGenerator::beg_main()
{
	buffer << mips.label("func_beg_main") << endl;
	buffer << mips.la("$gp", "__GP__") << endl;
	buffer << mips.subu("$sp", "$sp", stack_size) << endl;
}

void GCPTargetGenerator::ret_main()
{
	buffer << mips.li("$v0", 10) << endl;
	buffer << mips.syscall() << endl;
}

#define CASE(head, type)									\
case IrHead::head:											\
	buffer													\
		<< (												\
			IrType::is_var(code.elem[2]) ?					\
			mips.type(										\
			allocator.val_to_string(code.elem[0]),			\
			allocator.val_to_string(code.elem[1]),			\
				allocator.val_to_string(code.elem[2])) 		\
			:												\
			mips.type(										\
			allocator.val_to_string(code.elem[0]),			\
			allocator.val_to_string(code.elem[1]),			\
				allocator.cst_to_value(code.elem[2]))		\
				)											\
		<< endl;											\
	break	

void GCPTargetGenerator::func_body()
{
	const IrElemAllocator& allocator = *allocator_ptr;
	const IrTable& ir_table = *ir_table_ptr;

	bool pushing = false;
	int push_count = 0;
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
			buffer 
				<< mips.lw(
					allocator.val_to_string(code.elem[0]), 
					allocator.val_to_string(code.elem[1]), 
					allocator.cst_to_value(code.elem[2])) 
				<< endl;
			break;
		case IrHead::lb:
			buffer 
				<< mips.lb(
					allocator.val_to_string(code.elem[0]),
					allocator.val_to_string(code.elem[1]),
					allocator.cst_to_value(code.elem[2])) 
				<< endl;
			break;
		case IrHead::sw:
			buffer 
				<< mips.sw(
					allocator.val_to_string(code.elem[0]),
					allocator.val_to_string(code.elem[1]),
					allocator.cst_to_value(code.elem[2])) 
				<< endl;
			break;
		case IrHead::sb:
			buffer 
				<< mips.sb(
					allocator.val_to_string(code.elem[0]),
					allocator.val_to_string(code.elem[1]),
					allocator.cst_to_value(code.elem[2])) 
				<< endl;
			break;
		case IrHead::beq:
			buffer 
				<< mips.beq(
					allocator.val_to_string(code.elem[0]),
					allocator.val_to_string(code.elem[1]),
					allocator.label_to_string(code.elem[2]))
				<< endl;
			break;
		case IrHead::bne:
			buffer
				<< mips.bne(
					allocator.val_to_string(code.elem[0]),
					allocator.val_to_string(code.elem[1]),
					allocator.label_to_string(code.elem[2]))
				<< endl;
			break;
		case IrHead::_goto:
			buffer << mips.j(allocator.label_to_string(code.elem[0])) << endl;
			break;
		case IrHead::push:
		{
			if (pushing)
			{
				++push_count;
			}
			else
			{
				push_count = 0;
			}
			pushing = true;
			int offset = -8 - push_count * 4;
			if (IrType::is_cst(code.elem[0]))
			{
				buffer << mips.li("$v0", allocator.cst_to_value(code.elem[0]));
				buffer << mips.sw("$v0", "$sp", offset) << endl;
			}
			else
			{
				buffer << mips.sw(allocator.var_to_string(code.elem[0]), "$sp", offset) << endl;
			}
			break;
		}
		case IrHead::call:
			// buffer << mips.sw("$v0", "$sp", stack_size - 8) << endl;
			buffer << mips.jal(allocator.label_to_string(code.elem[0])) << endl;
			pushing = false;
			// buffer << mips.lw("$v0", "$sp", stack_size - 8) << endl;
			break;
		case IrHead::ret:
			if (func_name == "main")
			{
				ret_main();
			}
			else
			{
				ret_func();
			}
			break;
		case IrHead::scanf:
			buffer << mips.li("$v0", code.elem[1] == IrType::_int ? mips.read_int : mips.read_cahr) << endl;
			buffer << mips.syscall() << endl;
			buffer << mips.addu(allocator.val_to_string(code.elem[0]), "$v0", "$0") << endl;
			break;
		case IrHead::printf:
		{
			bool str = code.elem[0] != IrType::NIL;
			bool val = code.elem[1] != IrType::NIL;
			if (str)
			{
				const auto& label = string_label_table.at(code.elem[0]);
				buffer << mips.la("$a0", label) << endl;
				buffer << mips.li("$v0", mips.print_string) << endl;
				buffer << mips.syscall() << endl;
			}
			if (val)
			{
				buffer << mips.addu("$a0", "$0", allocator.val_to_string(code.elem[1])) << endl;
				buffer << mips.li("$v0", code.elem[2] == IrType::_int ? mips.print_int : mips.print_char) << endl;
				buffer << mips.syscall() << endl;
			}
			buffer << mips.li("$a0", '\n') << endl;
			buffer << mips.li("$v0", mips.print_char) << endl;
			buffer << mips.syscall() << endl;
			break;
		}
		case IrHead::movn:
			buffer
				<< mips.movn(
					allocator.var_to_string(code.elem[0]),
					allocator.var_to_string(code.elem[1]),
					allocator.var_to_string(code.elem[2]))
				<< endl;
			break;
		case IrHead::movz:
			buffer
				<< mips.movz(
					allocator.var_to_string(code.elem[0]),
					allocator.var_to_string(code.elem[1]),
					allocator.var_to_string(code.elem[2]))
				<< endl;
			break;
		case IrHead::reload:
		{
			auto rlt = func_var_offset_table.find(code.elem[1]);
			if (rlt != func_var_offset_table.end())
			{
				buffer
					<< mips.lw(
						allocator.val_to_string(code.elem[0]),
						"$sp",
						rlt->second)
					<< endl;
			}
			else
			{
				buffer
					<< mips.lw(
						allocator.val_to_string(code.elem[0]),
						"$gp",
						global_var_offset_table.at(code.elem[1]))
					<< endl;
			}
			break;
		}
		case IrHead::protect:
		{
			auto rlt = func_var_offset_table.find(code.elem[1]);
			if (rlt != func_var_offset_table.end())
			{
				buffer
					<< mips.sw(
						allocator.val_to_string(code.elem[0]),
						"$sp",
						rlt->second)
					<< endl;
			}
			else
			{
				buffer
					<< mips.sw(
						allocator.val_to_string(code.elem[0]),
						"$gp",
						global_var_offset_table.at(code.elem[1]))
					<< endl;
			}
			break;
		}
		default:
			PANIC();
		}
	}
}


string GCPTargetGenerator::fresh_buffer()
{
	string ret = buffer.str();
	buffer.str(string());
	buffer.clear();
	return ret;
}



void GCPTargetGenerator::translate(ostream& os)
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
		os << allocator_ptr->label_to_string(ir_table_ptr->at(func_end_index).elem[0]) << ':' << endl;
		if (flag)
		{
			ret_func();
		}
		else
		{
			ret_main();
		}
		os << fresh_buffer();
		os << "\n\n\n" << endl;
	}
}


























GCPTargetGenerator::GCPTargetGenerator(shared_ptr<IrElemAllocator> allocator, shared_ptr<const IrTable> ir_table)
	:
	allocator_ptr(allocator),
	ir_table_ptr(ir_table),
	global_var_offset_table(),
	func_var_offset_table(),
	string_label_table(),
	buffer(),
	mips()
{
}

void GCPRegisterAllocator::init_tmp_reg_pool()
{
	for (irelem_t reg : tmp_regs)
	{
		tmp_reg_pool[reg] = IrType::NIL;
		tmp_reg_dirty[reg] = false;
	}
}

void GCPRegisterAllocator::next_function_info()
{
	const IrElemAllocator& allocator = *allocator_ptr;
	const Ir* code;
	irelem_t label;
	func_beg_index = func_end_index - 1;
	do
	{
		code = &(origin_ir_table_ptr->at(++func_beg_index));
		label = code->elem[0];
	} while (code->head != IrHead::label || !IrType::is_func(label) || !IrType::is_beg(label));
#ifdef DEBUG_LEVEL
	string func_name = allocator.func_name(label);
#endif // DEBUG_LEVEL
	DEBUG_LOG_VAR(4, func_name);
	func_mid_index = func_beg_index;
	do
	{
		code = &(origin_ir_table_ptr->at(++func_mid_index));
		label = code->elem[0];
	} while (code->head != IrHead::label || !IrType::is_func(label) || !IrType::is_mid(label));
	ASSERT(3, func_name == allocator.func_name(label));
	func_end_index = func_mid_index;
	do
	{
		code = &(origin_ir_table_ptr->at(++func_end_index));
		label = code->elem[0];
	} while (code->head != IrHead::label || !IrType::is_func(label) || !IrType::is_end(label));
	ASSERT(3, func_name == allocator.func_name(label));
	block_detect_result = IrDetectors::func_block_detect(*origin_ir_table_ptr, *allocator_ptr, func_beg_index);
	block_var_activition_analyze_result = IrDetectors::block_var_activition_analyze(*origin_ir_table_ptr, *allocator_ptr, func_beg_index, *block_detect_result);
	var_activition_analyze_result = IrDetectors::var_activition_analyze(*origin_ir_table_ptr, *allocator_ptr, func_beg_index, *block_detect_result, *block_var_activition_analyze_result);

	// 统计函数的参数列表
	params.clear();
	for (size_t i = func_beg_index; i != func_end_index; ++i)
	{
		const auto& code = origin_ir_table_ptr->at(i);
		if (code.head == IrHead::param)
		{
			params.push_back(code.elem[0]);
		}
	}
	block_begs.clear();
	for (const auto& block : block_detect_result->get_blocks())
	{
		block_begs.insert(block.beg);
		block_lasts.insert(block.end - 1);
	}

	alloc_all_save_reg();
}


void GCPRegisterAllocator::alloc_all_save_reg()
{
	const auto& codes = *origin_ir_table_ptr;
	auto& allocator = *allocator_ptr;
	save_reg_alloc.clear();

	// TODO 扫描当前函数, 确定全局变量集合
	// in 包含第一个块中的入口 param
	/*for (const auto& in_set : block_var_activition_analyze_result->get_infos().in)
	{
		for (irelem_t var : in_set)
		{
			save_reg_alloc.insert(make_pair(var, IrType::NIL));
		}
	}*/
	for (const auto& out_set : block_var_activition_analyze_result->get_infos().out)
	{
		for (irelem_t var : out_set)
		{
			save_reg_alloc.insert(make_pair(var, IrType::NIL));
		}
	}

	// $a0-$a3 已经分配了寄存器
	for (size_t i = 0; i != 4 && i < params.size(); ++i)
	{
		save_reg_alloc.erase(params[i]);
	}

	// TODO 构造全局变量冲突图
	Graph<bool> graph(save_reg_alloc.size(), true);
	unordered_map<irelem_t, unsigned> ord;
	vector<irelem_t> vars;
	int count = 0;
	for (const auto& pair : save_reg_alloc)
	{
		ord.insert(make_pair(pair.first, count++));
		vars.push_back(pair.first);
	}
	for (const auto& in_set : block_var_activition_analyze_result->get_infos().in)
	{
		for (irelem_t var1 : in_set)
		{
			if (ord.count(var1) == 0) continue;
			for (irelem_t var2 : in_set)
			{
				if (ord.count(var2) == 0) continue;
				graph.d_link(ord[var1], ord[var2], true);
			}
		}
	}
	for (const auto& out_set : block_var_activition_analyze_result->get_infos().in)
	{
		for (irelem_t var1 : out_set)
		{
			if (ord.count(var1) == 0) continue;
			for (irelem_t var2 : out_set)
			{
				if (ord.count(var2) == 0) continue;
				graph.d_link(ord[var1], ord[var2], true);
			}
		}
	}
	// 将节点与自身的边删去
	for (size_t i = 0; i != ord.size(); ++i)
	{
		graph[i][i] = false;
	}

#ifdef DEBUG_LEVEL
	for (size_t i = 0; i != ord.size(); ++i)
	{
		for (size_t j = 0; j != ord.size(); ++j)
		{
			std::cout << (graph[i][j] ? 1 : 0) << ' ';
		}
		std::cout << endl;
	}
#endif // DEBUG_LEVEL

	// TODO 图着色分配寄存器
	stack<size_t> stack;
	unordered_set<size_t> untracked_points;
	for (size_t i = 0; i != ord.size(); ++i)
	{
		untracked_points.insert(i);
	}

	int reg_num_require = -1;

	while (untracked_points.size() != 0)
	{
		size_t max_count_point;
		int max_count = -1;
		size_t min_count_point;
		int min_count = graph.size();
		for (size_t p1 : untracked_points)
		{
			int count = 0;
			for (size_t p2 : untracked_points)
			{
				if (graph[p1][p2])
				{
					++count;
				}
			}
			if (count < min_count)
			{
				min_count = count;
				min_count_point = p1;
			}
			if (count > max_count)
			{
				max_count = count;
				max_count_point = p1;
			}
		}
		if (min_count < save_regs.size())
		{
			if (min_count > reg_num_require)
			{
				reg_num_require = min_count + 1;
			}
			untracked_points.erase(min_count_point);
			stack.push(min_count_point);
			continue;
		}
		untracked_points.erase(max_count_point);
	}

	while (stack.size() != 0)
	{
		size_t point = stack.top();
		stack.pop();
		unordered_set<irelem_t> unused_regs;
		//unused_regs.reserve(reg_num_require);
		for (irelem_t reg : save_regs)
		{
			if (unused_regs.size() >= reg_num_require)
			{
				break;
			}
			unused_regs.insert(reg);
		}
		for (size_t p : untracked_points)
		{
			if (graph[point][p])
			{
				unused_regs.erase(save_reg_alloc[vars[p]]);
			}
		}
		untracked_points.insert(point);
		save_reg_alloc[vars[point]] = *unused_regs.begin();
	}

	// 分配 $a0-$a3
	switch (params.size())
	{
	default:
	case 4:
		save_reg_alloc[params[3]] = allocator_ptr->reg(Reg::a3);
	case 3:
		save_reg_alloc[params[2]] = allocator_ptr->reg(Reg::a2);
	case 2:
		save_reg_alloc[params[1]] = allocator_ptr->reg(Reg::a1);
	case 1:
		save_reg_alloc[params[0]] = allocator_ptr->reg(Reg::a0);
	case 0:
		break;
	}

	for (irelem_t param : params)
	{
		if (save_reg_alloc.count(param) == 0)
		{
			save_reg_alloc.insert(make_pair(param, IrType::NIL));
		}
	}
}


void GCPRegisterAllocator::walk()
{
	IrElemAllocator& allocator = *allocator_ptr;
	const IrTable& codes = *origin_ir_table_ptr;

	for (current_index = 0; current_index != codes.size(); ++current_index)
	{
		// 整理 status
		if (block_begs.count(current_index) != 0)
		{
#ifdef DEBUG_LEVEL
			for (auto& pair : tmp_reg_pool)
			{
				if (save_reg_alloc.count(pair.second) != 0
					|| gvar_status.count(pair.second) != 0)
				{
					ASSERT(4, tmp_reg_dirty[pair.first] == false);
				}
			}
#endif // DEBUG_LEVEL

			free_treg_of_sync_no_reg_svar_and_gvar();
			tmp_reg_gc();
#ifdef DEBUG_LEVEL
			for (auto& pair : tmp_reg_pool)
			{
				ASSERT(4, pair.second = IrType::NIL);
			}
#endif // DEBUG_LEVEL
			init_tmp_reg_pool();
			var_status->clear();
			for (irelem_t var : var_activition_analyze_result->get_in(current_index))
			{
				irelem_t location = save_reg_alloc.at(var);
				if (location == IrType::NIL)
				{
					location = var;
				}
				var_status->insert(make_pair(var, location));
			}
		}

		bool prepare_to_end = block_lasts.count(current_index) != 0;

		// 翻译IR
		const Ir& code = codes.at(current_index);
		switch (code.head)
		{
		case IrHead::gvar:
			gvar_status.insert(make_pair(code.elem[0], code.elem[0]));
			buffer.push_back(code);
			break;
		case IrHead::param:
		{
			buffer.push_back(code);
			irelem_t reg = save_reg_alloc.at(code.elem[0]);
			for (irelem_t sreg : save_regs)
			{
				if (reg == sreg)
				{
					buffer.push_back(ir.reload(reg, code.elem[0]));
					break;
				}
			}
			break;
		}
		case IrHead::add:
		case IrHead::sub:
		case IrHead::mult:
		case IrHead::div:
		case IrHead::_and:
		case IrHead::_or:
		case IrHead::_nor:
		case IrHead::_xor:
		case IrHead::sl:
		case IrHead::sr:
		case IrHead::less:
		case IrHead::movn:
		case IrHead::movz:
		{
			Ir new_code = code;

			new_code.elem[1] = use_reg_or_cst_of_val(code.elem[1]);
			keep_in_tx.insert(new_code.elem[1]);

			new_code.elem[2] = use_reg_or_cst_of_val(code.elem[2]);
			keep_in_tx.insert(new_code.elem[2]);

			new_code.elem[0] = write_reg_of_var(code.elem[0]);

			buffer.push_back(new_code);

			keep_in_tx.erase(new_code.elem[2]);
			keep_in_tx.erase(new_code.elem[1]);
			break;
		}
		case IrHead::lw:
		case IrHead::lb:
		{
			Ir new_code = code;

			new_code.elem[1] = use_reg_or_cst_of_val(code.elem[1]);
			keep_in_tx.insert(new_code.elem[1]);

			new_code.elem[2] = code.elem[2];

			new_code.elem[0] = write_reg_of_var(code.elem[0]);

			buffer.push_back(new_code);

			keep_in_tx.erase(new_code.elem[1]);
			break;
		}
		case IrHead::beq:
		case IrHead::bne:
			sync_no_reg_svar_and_gvar_in_treg();
			prepare_to_end = false;
			// 不要 break
		case IrHead::sw:
		case IrHead::sb:
		{
			Ir new_code = code;

			new_code.elem[0] = use_reg_or_cst_of_val(code.elem[0]);
			keep_in_tx.insert(new_code.elem[0]);

			new_code.elem[1] = use_reg_or_cst_of_val(code.elem[1]);

			new_code.elem[2] = code.elem[2];
			buffer.push_back(new_code);

			keep_in_tx.erase(new_code.elem[0]);
			break;
		}
		case IrHead::push:
		{
			if (remain_push == 0)
			{
				fresh_push_and_call_info();
			}
			if (remain_push > 4)
			{
				// 直接压栈, 故不需要保护 $ax
				Ir new_code = code;
				if (IrType::is_var(code.elem[0]))
				{
					new_code.elem[0] = use_reg_or_cst_of_val(code.elem[0]);
				}
				else
				{
					irelem_t cst = code.elem[0];
					ASSERT(4, IrType::is_cst(cst));
					irelem_t reg = alloc_tmp_reg();
					buffer.push_back(ir.add(reg, allocator.reg(Reg::zero), cst));
					new_code.elem[0] = reg;
					tmp_reg_pool[reg] = IrType::NIL;
					tmp_reg_dirty[reg] = false;
				}
				new_code.elem[1] = code.elem[1];
				new_code.elem[2] = code.elem[2];
				buffer.push_back(new_code);
			}
			else
			{
				// 通过 $ax传参
				irelem_t reg_ax;
				switch (remain_push)
				{
				case 1:
					reg_ax = allocator.reg(Reg::a0);
					break;
				case 2:
					reg_ax = allocator.reg(Reg::a1);
					break;
				case 3:
					reg_ax = allocator.reg(Reg::a2);
					break;
				case 4:
					reg_ax = allocator.reg(Reg::a3);
					break;
				}
				if (remain_push <= params.size()
					&& var_activition_analyze_result->get_out(current_index).count(params[remain_push - 1]) != 0)
				{
					// 此时 $ax 在push后仍旧有活性, 需要判断在call之前知否还有活跃.
					irelem_t param_var = params[remain_push - 1];
					ASSERT(4, current_index < call_index);
					bool need_protect_to_reg = false;
					for (size_t i = current_index + 1; i != call_index; ++i)
					{
						irelem_t def_elem;
						irelem_t use_elem_1;
						irelem_t use_elem_2;
						irelem_t use_elem_3;
						IrDetectors::get_def_and_use_elem(origin_ir_table_ptr->at(i), *allocator_ptr, &def_elem, &use_elem_1, &use_elem_2, &use_elem_3);

						if (use_elem_1 == param_var || use_elem_2 == param_var)
						{
							need_protect_to_reg = true;
							break;
						}
						// 如果有def, 那么前面一定有use, 否则out[current_index]一定不包含param_var 
						ASSERT(4, def_elem != param_var);
					}
					if (need_protect_to_reg)
					{
						irelem_t reg = alloc_tmp_reg();
						var_status->at(param_var) = reg;
						tmp_reg_pool[reg] = param_var;
						tmp_reg_dirty[reg] = true;
						keep_in_tx.insert(reg);
						buffer.push_back(ir.add(reg, reg_ax, allocator.reg(Reg::zero)));
					}
					else
					{
						var_status->at(param_var) = param_var;
						buffer.push_back(ir.protect(reg_ax, param_var));
					}
				}
				buffer.push_back(ir.add(reg_ax, allocator.reg(Reg::zero), use_reg_or_cst_of_val(code.elem[0])));
			}
			--remain_push;
			break;
		}
		case IrHead::call:
		{
			// TODO 如果有没被保护的仍有活性的 param_var, 将其保护
			for (size_t i = 0; i != 4 && i < params.size(); ++i)
			{
				if (var_activition_analyze_result->get_out(current_index).count(params[i]) != 0)
				{
					irelem_t reg = var_status->at(params[i]);
					if (allocator_ptr->is_reg(reg))
					{
						if (tmp_reg_pool.count(reg) != 0)
						{
							tmp_reg_pool[reg] = IrType::NIL;
						}
						buffer.push_back(ir.protect(var_status->at(params[i]), params[i]));
						var_status->at(params[i]) = params[i];
					}
				}
			}

			// TODO 如果 $tx 中局部变量有活性/保存着脏的全局变量/保存着脏的gvar, 将其保护
			keep_in_tx.clear();
			protect_all_vars_in_tmp_regs_to_stack();

			// TODO 调用目标函数
			buffer.push_back(code);

			// 重新加载活跃的 $ax
			switch (params.size())
			{
# define CHECK_PARAM_REG(num)																\
			{																				\
				irelem_t reg = allocator_ptr->reg(Reg::a##num);							\
				irelem_t var = params[num];													\
				if (var_activition_analyze_result->get_out(current_index).count(var) != 0)	\
				{																			\
					var_status->at(var) = reg;												\
					buffer.push_back(ir.reload(reg, var));									\
				}																			\
			}
			default:
			case 4:
				CHECK_PARAM_REG(3)
			case 3:
				CHECK_PARAM_REG(2)
			case 2:
				CHECK_PARAM_REG(1)
			case 1:
				CHECK_PARAM_REG(0)
			case 0:
				break;
			}
			break;
		}
		case IrHead::_goto:
		case IrHead::ret:
			sync_no_reg_svar_and_gvar_in_treg();
			prepare_to_end = false;
			buffer.push_back(code);
			break;
		case IrHead::scanf:
		{
			Ir new_code = code;
			new_code.elem[0] = write_reg_of_var(code.elem[0]);
			new_code.elem[1] = code.elem[1];
			new_code.elem[2] = code.elem[2];
			buffer.push_back(new_code);
			break;
		}
		case IrHead::printf:
		{
			// 保护 $a0
			irelem_t reg = IrType::NIL;
			if (params.size() != 0 
				&& var_activition_analyze_result->get_in(current_index).count(params[0]) != 0)
			{
				irelem_t avar = params[0];
				ASSERT(4, var_status->at(avar) == allocator_ptr->reg(Reg::a0));
				reg = alloc_tmp_reg();
				var_status->at(avar) = reg;
				keep_in_tx.insert(reg);
				tmp_reg_dirty[reg] = true;
				tmp_reg_pool[reg] = avar;
				buffer.push_back(ir.add(reg, allocator_ptr->reg(Reg::a0), allocator_ptr->reg(Reg::zero)));
			}
			Ir new_code = code;
			new_code.elem[0] = code.elem[0];
			new_code.elem[1] = use_reg_or_cst_of_val(code.elem[1]);
			new_code.elem[2] = code.elem[2];
			buffer.push_back(new_code);
			if (reg != IrType::NIL)
			{
				irelem_t avar = params[0];
				buffer.push_back(ir.add(allocator_ptr->reg(Reg::a0), reg, allocator_ptr->reg(Reg::zero)));
				keep_in_tx.erase(reg);
				tmp_reg_dirty[reg] = false;
				tmp_reg_pool[reg] = IrType::NIL;
				var_status->at(avar) = allocator_ptr->reg(Reg::a0);
			}
			break;
		}
		case IrHead::label:
		{
			if (IrType::is_func(code.elem[0]) && IrType::is_beg(code.elem[0]))
			{
				next_function_info();
			}
			// 不要 break
		}
		default:
			buffer.push_back(code);
		}

		if (prepare_to_end)
		{
			sync_no_reg_svar_and_gvar_in_treg();
		}
	}
}

void GCPRegisterAllocator::sync_no_reg_svar_and_gvar_in_treg()
{
	for (auto& pair : tmp_reg_pool)
	{
		if (save_reg_alloc.count(pair.second) != 0)
		{
			if (tmp_reg_dirty.at(pair.first))
			{
				buffer.push_back(ir.protect(pair.first, pair.second));
				tmp_reg_dirty[pair.first] = false;
			}
		}
		else if (gvar_status.count(pair.second) != 0)
		{
			if (tmp_reg_dirty.at(pair.first))
			{
				buffer.push_back(ir.protect(pair.first, pair.second));
				tmp_reg_dirty[pair.first] = false;
			}
		}
	}
}

void GCPRegisterAllocator::free_treg_of_sync_no_reg_svar_and_gvar()
{
	for (auto& pair : tmp_reg_pool)
	{
		if (keep_in_tx.count(pair.first))
		{
			continue;
		}
		if (save_reg_alloc.count(pair.second) != 0)
		{
			if (!tmp_reg_dirty.at(pair.first))
			{
				var_status->at(pair.second) = pair.second;
				pair.second = IrType::NIL;
			}
		}
		else if (gvar_status.count(pair.second) != 0)
		{
			if (!tmp_reg_dirty.at(pair.first))
			{
				gvar_status[pair.second] = pair.second;
				pair.second = IrType::NIL;
			}
		}
	}
}

void GCPRegisterAllocator::protect_all_vars_in_tmp_regs_to_stack()
{
	for (auto& pair : tmp_reg_pool)
	{
		if (save_reg_alloc.count(pair.second) != 0)
		{
			if (tmp_reg_dirty.at(pair.first))
			{
				buffer.push_back(ir.protect(pair.first, pair.second));
				tmp_reg_dirty[pair.first] = false;
			}
			var_status->at(pair.second) = pair.second;
		}
		else if (gvar_status.count(pair.second) != 0)
		{
			if (tmp_reg_dirty.at(pair.first))
			{
				buffer.push_back(ir.protect(pair.first, pair.second));
				tmp_reg_dirty[pair.first] = false;
			}
			gvar_status[pair.second] = pair.second;
		}
		else if (var_activition_analyze_result->get_out(current_index).count(pair.second) != 0)
		{
			buffer.push_back(ir.protect(pair.first, pair.second));
			tmp_reg_dirty[pair.first] = false;
			var_status->at(pair.second) = pair.second;
		}
		else
		{
			var_status->erase(pair.second);
		}
		pair.second = IrType::NIL;
	}
}

irelem_t GCPRegisterAllocator::tmp_reg_gc()
{
	irelem_t ret = IrType::NIL;
	const auto& in = var_activition_analyze_result->get_in(current_index);
	for (auto& pair : tmp_reg_pool)
	{
		if (keep_in_tx.count(pair.first) != 0)
		{
			continue;
		}
		irelem_t var = pair.second;
		if (gvar_status.count(var) == 0 && in.count(var) == 0)
		{
			var_status->erase(var);
			pair.second = IrType::NIL;
			ret = pair.first;
		}
	}
	return ret;
}

irelem_t GCPRegisterAllocator::alloc_tmp_reg()
{
	// 有空闲的 $tx
	for (auto& pair : tmp_reg_pool)
	{
		if (pair.second == IrType::NIL)
		{
			return pair.first;
		}
	}
	
	// 回收无用的 $tx
	irelem_t ret = tmp_reg_gc();
	if (ret != IrType::NIL)
	{
		return ret;
	}

	// 回收 gvar 变量占用的 $tx
	for (auto& pair : tmp_reg_pool)
	{
		if (keep_in_tx.count(pair.first) != 0)
		{
			continue;
		}
		irelem_t var = pair.second;
		if (gvar_status.count(var) != 0)
		{
			if (tmp_reg_dirty.at(pair.first))
			{
				buffer.push_back(ir.protect(pair.first, pair.second));
				tmp_reg_dirty[pair.first] = false;
			}
			gvar_status[var] = var;
			pair.second = IrType::NIL;
			ret = pair.first;
		}
	}
	if (ret != IrType::NIL)
	{
		return ret;
	}

	// 将一个临时变量淘汰到栈上
	// 优先淘汰不需保存的
	for (auto& pair : tmp_reg_pool)
	{
		if (keep_in_tx.count(pair.first) != 0)
		{
			continue;
		}
		if (tmp_reg_dirty.at(pair.first))
		{
			continue;
		}
		var_status->at(pair.second) = pair.second;
		pair.second = IrType::NIL;
		return pair.first;
	}
	// 随机淘汰一个未被保护的
	for (auto& pair : tmp_reg_pool)
	{
		if (keep_in_tx.count(pair.first) != 0)
		{
			continue;
		}
		buffer.push_back(ir.protect(pair.first, pair.second));
		tmp_reg_dirty[pair.first] = false;
		var_status->at(pair.second) = pair.second;
		pair.second = IrType::NIL;
		return pair.first;
	}
}


irelem_t GCPRegisterAllocator::write_reg_of_var(irelem_t var)
{
	// TODO 
	if (var == allocator_ptr->zero())
	{
		return allocator_ptr->reg(Reg::zero);
	}
	if (var == allocator_ptr->ret())
	{
		return allocator_ptr->reg(Reg::v1);
	}
	if (var == allocator_ptr->sp())
	{
		return allocator_ptr->reg(Reg::sp);
	}
	if (var == allocator_ptr->gp())
	{
		return allocator_ptr->reg(Reg::gp);
	}
	// var_status 中, 表示已经在寄存器中/被保护到栈上
	const auto& in_local = var_status->find(var);
	const auto& in_save = save_reg_alloc.find(var);
	if (in_local != var_status->end())
	{
		irelem_t location = in_local->second;
		if (allocator_ptr->is_reg(location))
		{
			if (tmp_reg_pool.count(location) != 0)
			{
				tmp_reg_dirty.at(location) = true;
			}
			return location;
		}
		irelem_t reg;
		if (in_save != save_reg_alloc.end() && allocator_ptr->is_reg(in_save->second))
		{
			// var 是一个保存在栈上且分配了寄存器的的全局寄存器
			// 应该是 $ax ?
			reg = in_save->second;
		}
		else
		{
			reg = alloc_tmp_reg();
			tmp_reg_pool.at(reg) = var;
			tmp_reg_dirty.at(reg) = true;
		}
		var_status->at(var) = reg;
		return reg;
	}
	// save_reg_alloc中, 表示是一个全局变量, 但是未被使用
	if (in_save != save_reg_alloc.end())
	{
		irelem_t location = in_save->second;
		if (allocator_ptr->is_reg(location))
		{
			// 此时 location 是 $sx 或者 $ax, 不需要标记脏位
			var_status->insert(make_pair(var, location));
			return location;
		}
		irelem_t reg = alloc_tmp_reg();
		var_status->insert(make_pair(var, reg));
		tmp_reg_pool.at(reg) = var;
		tmp_reg_dirty.at(reg) = true;
		return reg;
	}
	// gvar_status 中, 表示是一个全局变量
	const auto& in_global = gvar_status.find(var);
	if (in_global != gvar_status.end())
	{
		irelem_t location = in_global->second;
		if (allocator_ptr->is_reg(location))
		{
			if (tmp_reg_pool.count(location) != 0)
			{
				tmp_reg_dirty.at(location) = true;
			}
			return location;
		}
		irelem_t reg = alloc_tmp_reg();
		gvar_status.at(var) = reg;
		tmp_reg_pool.at(reg) = var;
		tmp_reg_dirty.at(reg) = true;
		return reg;
	}
	irelem_t reg = alloc_tmp_reg();
	var_status->insert(make_pair(var, reg));
	tmp_reg_pool.at(reg) = var;
	tmp_reg_dirty.at(reg) = true;
	return reg;
}

irelem_t GCPRegisterAllocator::use_reg_of_var(irelem_t var)
{
	// TODO
	if (var == allocator_ptr->zero())
	{
		return allocator_ptr->reg(Reg::zero);
	}
	if (var == allocator_ptr->ret())
	{
		return allocator_ptr->reg(Reg::v1);
	}
	if (var == allocator_ptr->sp())
	{
		return allocator_ptr->reg(Reg::sp);
	}
	if (var == allocator_ptr->gp())
	{
		return allocator_ptr->reg(Reg::gp);
	}
	// var_status 中, 表示已经在寄存器中/被保护到栈上
	const auto& in_local = var_status->find(var);
	const auto& in_save = save_reg_alloc.find(var);
	if (in_local != var_status->end())
	{
		irelem_t location = in_local->second;
		if (allocator_ptr->is_reg(location))
		{
			return location;
		}
		irelem_t reg;
		if (in_save != save_reg_alloc.end() && allocator_ptr->is_reg(in_save->second))
		{
			// var 是一个保存在栈上且分配了寄存器的的全局寄存器
			// 应该是 $ax ?
			reg = in_save->second;
		}
		else
		{
			reg = alloc_tmp_reg();
			tmp_reg_pool.at(reg) = var;
			tmp_reg_dirty.at(reg) = false;
		}
		if (IrType::is_var(location))	// location 一定不是 reg
		{
			buffer.push_back(ir.reload(reg, location));
		}
		var_status->at(var) = reg;
		return reg;
	}
	// save_reg_alloc中, 表示是一个全局变量, 但是未被使用, 也未被初始化
	if (in_save != save_reg_alloc.end())
	{
		irelem_t location = in_save->second;
		if (allocator_ptr->is_reg(location))
		{
			var_status->insert(make_pair(var, location));
			return location;
		}
		irelem_t reg = alloc_tmp_reg();
		buffer.push_back(ir.reload(reg, location));
		var_status->insert(make_pair(var, reg));
		tmp_reg_pool.at(reg) = var;
		tmp_reg_dirty.at(reg) = false;
		return reg;
	}
	// gvar_status 中, 表示是一个全局变量
	const auto& in_global = gvar_status.find(var);
	if (in_global != gvar_status.end())
	{
		irelem_t location = in_global->second;
		if (allocator_ptr->is_reg(location))
		{
			return location;
		}
		irelem_t reg = alloc_tmp_reg();
		buffer.push_back(ir.reload(reg, location));
		gvar_status.at(var) = reg;
		tmp_reg_pool.at(reg) = var;
		tmp_reg_dirty.at(reg) = false;
		return reg;
	}
	irelem_t reg = alloc_tmp_reg();
	var_status->insert(make_pair(var, reg));
	tmp_reg_pool.at(reg) = var;
	tmp_reg_dirty.at(reg) = false;
	return reg;
}

irelem_t GCPRegisterAllocator::use_reg_or_cst_of_val(irelem_t val)
{
	if (IrType::is_var(val))
	{
		return use_reg_of_var(val);
	}
	return val;
}

void GCPRegisterAllocator::fresh_push_and_call_info()
{
	remain_push = 0;
	for (size_t idx = current_index; ; ++idx)
	{
		const auto& code = origin_ir_table_ptr->at(idx);
		if (code.head == IrHead::call)
		{
			call_index = idx;
			break;
		}
		if (code.head == IrHead::push)
		{
			++remain_push;
		}
	}
}



GCPRegisterAllocator::GCPRegisterAllocator(shared_ptr<IrElemAllocator> allocator, shared_ptr<const IrTable> ir_table)
	:
	allocator_ptr(allocator),
	origin_ir_table_ptr(ir_table),
	ir(allocator),
	tmp_regs({
		allocator->reg(Reg::t0),
		allocator->reg(Reg::t1),
		allocator->reg(Reg::t2),
		allocator->reg(Reg::t3),
		allocator->reg(Reg::t4),
		allocator->reg(Reg::t5),
		allocator->reg(Reg::t6),
		allocator->reg(Reg::t7),
		allocator->reg(Reg::t8),
		allocator->reg(Reg::t9)
	}),
	save_regs({
		allocator->reg(Reg::s0),
		allocator->reg(Reg::s1),
		allocator->reg(Reg::s2),
		allocator->reg(Reg::s3),
		allocator->reg(Reg::s4),
		allocator->reg(Reg::s5),
		allocator->reg(Reg::s6),
		allocator->reg(Reg::s7),
	}),
	tmp_reg_pool(),
	save_reg_alloc(),
	params(),
	var_status(new unordered_map<irelem_t, irelem_t>()),
	gvar_status()
{
	init_tmp_reg_pool();
}

shared_ptr<IrTable> GCPRegisterAllocator::build()
{
	walk();

	unordered_set<IrHead> end_set = { IrHead::arr, IrHead::init, IrHead::gvar, IrHead::param, IrHead::func };
	for (int i = 0; i < buffer.size(); ++i)
	{
		if (buffer[i].head == IrHead::arr ||
			buffer[i].head == IrHead::param ||
			buffer[i].head == IrHead::init ||
			buffer[i].head == IrHead::label
			&& IrType::is_mid(buffer[i].elem[0])
			&& IrType::is_func(buffer[i].elem[0]))
		{
			for (int j = i - 1; j >= 0; --j)
			{
				if (end_set.count(buffer[j].head) != 0) break;
				// if (builder[j].head == IrHead::label && IrType::is_func(builder[i].elem[0]) && IrType::is_beg(builder[i].elem[0])) break;
				swap(buffer[j], buffer[j + 1]);
			}
		}
	}

	return buffer.build();
}
