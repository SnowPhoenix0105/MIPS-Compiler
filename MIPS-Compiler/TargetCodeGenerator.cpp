#include "TargetCodeGenerator.h"

void SimpleGenerator::next_function_info()
{
	// TODO
	const IrElemAllocator& allocator = *allocator_ptr;
	func_beg_index = func_end_index;
	while (true)
	{
		const Ir& ir = ir_table->at(++func_beg_index);
		irelem_t label = ir.elem[0];
		if (ir.head != IrHead::label || !IrType::is_func(label)  || !IrType::is_beg(label))
		{
			continue;
		}
	}
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
