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


void GCPRegisterAllocator::init_global()
{
	for (const auto& code : *origin_ir_table_ptr)
	{
		if (code.head == IrHead::label && IrType::is_func(code.elem[0]))
		{
			break;
		}
		buffer.push_back(code);
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
	func_name = allocator.func_name(label);
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
}

void GCPRegisterAllocator::analyze_func()
{
	IrElemAllocator& allocator = *allocator_ptr;
	const IrTable& codes = *origin_ir_table_ptr;
	for (size_t i = func_beg_index; i != func_end_index; ++i)
	{
		const Ir& code = codes.at(i);
		switch (code.head)
		{
		case IrHead::param:
			if (save_reg_alloc[code.elem[0]] == IrType::NIL)
			{
				buffer.push_back(code);
			}
			else
			{
				buffer.push_back(ir.param(get_reg_of_var(code.elem[0])));
			}
			break;
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
		{
			irelem_t def_elem = code.elem[0];
			irelem_t use_elem_1 = code.elem[1];
			irelem_t use_elem_2 = code.elem[2];
			Ir new_code = code;
			def_elem = get_reg_of_var(def_elem);
			if (IrType::is_var(use_elem_1))
		}
		case IrHead::lw:
		case IrHead::lb:
		case IrHead::sw:
		case IrHead::sb:

		case IrHead::beq:
		case IrHead::bne:
		case IrHead::_goto:

		case IrHead::push:
		case IrHead::call:
		case IrHead::ret:

		case IrHead::scanf:
		case IrHead::printf:

		case IrHead::protect:
		case IrHead::reload:

		default:
			buffer.push_back(code);
		}

	}
}



GCPRegisterAllocator::GCPRegisterAllocator(shared_ptr<IrElemAllocator> allocator, shared_ptr<const IrTable> ir_table)
	:
	allocator_ptr(allocator),
	origin_ir_table_ptr(ir_table),
	ir(allocator),
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
