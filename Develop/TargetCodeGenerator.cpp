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

void SimpleGenerator::init_total()
{
	// TODO
	const IrElemAllocator& allocator = *allocator_ptr;
	const IrTable& ir_table = *ir_table_ptr;
	unsigned offset = 0;
	buffer << ".data" << endl;
	buffer << "__GP__:" << endl;
	for (size_t i = 0; ; ++i)
	{
		const auto& ir = ir_table.at(i);
		switch (ir.head)
		{
		case IrHead::label:
			goto END_FOR;
		case IrHead::gvar:
		{
			string var_label = allocator.var_to_string(ir.elem[0]);
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
			irelem_t arr = ir.elem[0];
			bool is_int = ir.elem[1] == IrType::_int;
			int size = allocator.imm_value(ir.elem[2]);
			global_arr_info_table.insert(make_pair(arr, ArrayInfo{ offset, is_int }));
			int space = size * (is_int ? 4 : 1);
			space = (space + 3) ^ 3;
			if (ir_table.at(i + 1).head != IrHead::init)
			{
				buffer << ".space";
				break;
			}
			
			break;
		}
		default:
			PANIC();
		}
	}
END_FOR:
	buffer << ".text" << endl;
}

void SimpleGenerator::init_func()
{
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
	init_total();
	// TODO 
	// TODO main : la $gp,__GP__
}
