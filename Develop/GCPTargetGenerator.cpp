#include "GCPTargetGenerator.h"

string GCPTargetGenerator::fresh_buffer()
{
	string ret = buffer.str();
	buffer.str(string());
	buffer.clear();
	return ret;
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
	}
}

void GCPRegisterAllocator::free_tmp_reg_of_var(irelem_t var)
{
	for (irelem_t reg : tmp_regs)
	{
		if (tmp_reg_pool[reg] == var)
		{
			tmp_reg_pool[reg] = IrType::NIL;
		}
	}
}

irelem_t GCPRegisterAllocator::alloc_tmp_reg_for_var(irelem_t var)
{
	for (irelem_t reg : tmp_regs)
	{
		irelem_t using_var = tmp_reg_pool[reg];
		if (using_var == IrType::NIL)
		{
			tmp_reg_pool[reg] = var;
			return reg;
		}
	}
	// TODO 决策一个tmp-reg被释放
}

void GCPRegisterAllocator::protect_var(irelem_t var)
{
	for (irelem_t reg : tmp_regs)
	{
		if (tmp_reg_pool[reg] == var)
		{
			free_reg_and_protect_content(reg);
		}
	}
}

void GCPRegisterAllocator::free_reg_and_protect_content(irelem_t reg)
{
	irelem_t var = tmp_reg_pool[reg];
	tmp_reg_pool[reg] = IrType::NIL;
	buffer.push_back(ir.protect(reg, var));
	protected_var.insert(var);
}

GCPRegisterAllocator::GCPRegisterAllocator(shared_ptr<IrElemAllocator> allocator, shared_ptr<const IrTable> ir_table)
	:
	allocator_ptr(allocator),
	origin_ir_table_ptr(ir_table),
	tmp_reg_pool(),
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
		})
{
	init_tmp_reg_pool();
}
