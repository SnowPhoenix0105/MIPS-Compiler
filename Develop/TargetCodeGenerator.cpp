#include "TargetCodeGenerator.h"

void SimpleGenerator::next_function_info()
{
	const IrElemAllocator& allocator = *allocator_ptr;
	const Ir* ir;
	irelem_t label;
	func_beg_index = func_end_index;
	do
	{
		ir = &(ir_table->at(++func_beg_index));
		label = ir->elem[0];
	} while (ir->head != IrHead::label || !IrType::is_func(label) || !IrType::is_beg(label));
	func_name = allocator.func_name(label);
	func_mid_index = func_beg_index;
	do
	{
		ir = &(ir_table->at(++func_mid_index));
		label = ir->elem[0];
	} while (ir->head != IrHead::label || !IrType::is_func(label) || !IrType::is_beg(label));
	ASSERT(3, func_name == allocator.func_name(label));
	func_end_index = func_mid_index;
	do
	{
		ir = &(ir_table->at(++func_end_index));
		label = ir->elem[0];
	} while (ir->head != IrHead::label || !IrType::is_func(label) || !IrType::is_beg(label));
	ASSERT(3, func_name == allocator.func_name(label));
}

void SimpleGenerator::init_total()
{
	// TODO
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

	// TODO
}
