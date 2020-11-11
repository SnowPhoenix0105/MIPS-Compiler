#include "TargetCodeGenerator.h"

void SimpleGenerator::next_function_info()
{
	const IrElemAllocator& allocator = *allocator_ptr;
	const Ir* ir;
	irelem_t label;
	func_beg_index = func_end_index;
	do
	{
		ir = &(ir_table_ptr->at(++func_beg_index));
		label = ir->elem[0];
	} while (ir->head != IrHead::label || !IrType::is_func(label) || !IrType::is_beg(label));
	func_name = allocator.func_name(label);
	func_mid_index = func_beg_index;
	do
	{
		ir = &(ir_table_ptr->at(++func_mid_index));
		label = ir->elem[0];
	} while (ir->head != IrHead::label || !IrType::is_func(label) || !IrType::is_beg(label));
	ASSERT(3, func_name == allocator.func_name(label));
	func_end_index = func_mid_index;
	do
	{
		ir = &(ir_table_ptr->at(++func_end_index));
		label = ir->elem[0];
	} while (ir->head != IrHead::label || !IrType::is_func(label) || !IrType::is_beg(label));
	ASSERT(3, func_name == allocator.func_name(label));
}

void SimpleGenerator::init_global()
{
	const IrElemAllocator& allocator = *allocator_ptr;
	const IrTable& ir_table = *ir_table_ptr;
	unsigned offset = 0;
	buffer << ".data" << endl;
	buffer << "__GP__:" << endl;
	bool flag = true;
	for (size_t i = 0; flag ; ++i)
	{
		const auto& ir = ir_table.at(i);
		switch (ir.head)
		{
		case IrHead::label:
			// 变量声明结束
			flag = false;
			break;
		case IrHead::gvar:
		{
			// 全局变量
			irelem_t var = ir.elem[0];
			string var_label = allocator.var_to_string(var);
			global_var_offset_table.insert(make_pair(var, offset));
			offset += 4;
			buffer << var_label << ":\t.word";
			if (ir.elem[1] == IrType::NIL)
			{
				buffer << endl;
			}
			else
			{
				buffer << '\t' << allocator.imm_value(ir.elem[1]) << endl;
			}
			break;
		}
		case IrHead::arr:
		{
			// 全局数组
			irelem_t arr = ir.elem[0];
			bool is_int = ir.elem[1] == IrType::_int;
			int size = allocator.imm_value(ir.elem[2]);
			global_arr_info_table.insert(make_pair(arr, ArrayInfo{ offset, is_int }));
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
			int count = 0;
			for (; count != size; ++count)
			{
				const auto& init = ir_table.at(i + count);
				ASSERT(3, init.head == IrHead::init);
				irelem_t imm = init.elem[0];
				unsigned value = allocator.imm_value(imm);
				buffer << head << value << endl;
			}
			if (is_int)
			{
				break;
			}
			// 字对齐
			for (; count != space; ++count)
			{
				buffer << head << 0 << endl;
			}
			break;
		}
		default:
			PANIC();
		}
	}
	buffer << ".text" << endl;
}

void SimpleGenerator::init_func()
{
	// TODO
	const IrElemAllocator& allocator = *allocator_ptr;
	const IrTable& ir_table = *ir_table_ptr;
	// 遍历 beg -> mid , 统计形参, 数组
	unordered_set<irelem_t> param_set;
	for (size_t i = func_beg_index + 1; i != func_mid_index; ++i)
	{
		const auto& ir = ir_table.at(i);
		switch (ir.head)
		{
		case IrHead::func:
			break;
		case IrHead::param:
		{
			ASSERT(4, IrType::is_var(ir.elem[0]));
			param_set.insert(ir.elem[0]);
		}
		case IrHead::arr:
		{
			// TODO
		}
		default:
			break;
		}
	}

	// 遍历 mid -> end , 统计局部var数量
	unordered_set<irelem_t> var_set;
	for (size_t i = func_mid_index + 1; i != func_end_index; ++i)
	{
		const auto& ir = ir_table.at(i);
		for (size_t j = 0; j < 3; ++j)
		{
			if (allocator.is_local_var(ir.elem[j]))
			{
				var_set.insert(ir.elem[j]);
			}
		}
	}
	// TODO
}

void SimpleGenerator::beg_func()
{
	buffer << "sw $ra, " << stack_size - 4 << "($sp)" << endl;
	buffer << "addiu $sp, $sp, -" << stack_size << endl;
}

void SimpleGenerator::end_func()
{
	buffer << "lw $ra, " << stack_size - 4 << "($sp)" << endl;
	buffer << "addiu $sp, $sp, " << stack_size << endl;
	buffer << "jr $ra" << endl;
}

void SimpleGenerator::fresh_buffer(ostream& os)
{
	os << buffer.str();
	buffer.clear();
}

void SimpleGenerator::translate(ostream& os)
{
	init_global();
	// TODO 
	// TODO main : la $gp,__GP__
}
