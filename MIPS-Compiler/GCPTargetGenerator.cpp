#include "GCPTargetGenerator.h"
#include "Graph.h"
#include <stack>

using std::stack;

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
	alloc_save_reg();
	protected_var.clear();
	init_tmp_reg_pool();
}


void GCPRegisterAllocator::alloc_save_reg()
{
	const auto& codes = *origin_ir_table_ptr;
	auto& allocator = *allocator_ptr;
	save_reg_alloc.clear();

	// TODO 扫描当前函数, 确定全局变量集合
	for (const auto& in_set : block_var_activition_analyze_result->get_infos().in)
	{
		for (irelem_t var : in_set)
		{
			save_reg_alloc.insert(make_pair(var, IrType::NIL));
		}
	}
	for (const auto& out_set : block_var_activition_analyze_result->get_infos().in)
	{
		for (irelem_t var : out_set)
		{
			save_reg_alloc.insert(make_pair(var, IrType::NIL));
		}
	}

	// TODO 构造全局变量冲突图
	Graph<bool> graph(save_reg_alloc.size(), true);
	unordered_map<irelem_t, unsigned> ord;
	int count = 0;
	for (const auto& pair : save_reg_alloc)
	{
		ord.insert(make_pair(pair.first, count++));
	}
	for (const auto& in_set : block_var_activition_analyze_result->get_infos().in)
	{
		for (irelem_t var1 : in_set)
		{
			for (irelem_t var2 : in_set)
			{
				graph.d_link(ord[var1], ord[var2], true);
			}
		}
	}
	for (const auto& out_set : block_var_activition_analyze_result->get_infos().in)
	{
		for (irelem_t var1 : out_set)
		{
			for (irelem_t var2 : out_set)
			{
				graph.d_link(ord[var1], ord[var2], true);
			}
		}
	}
	// 将节点与自身的边删去
	for (size_t i = 0; i != ord.size(); ++i)
	{
		graph[i][i] = false;
	}

	// TODO 图着色分配寄存器
	stack<size_t> stack;
	unordered_set<size_t> untracked_points;
	for (size_t i = 0; i != ord.size(); ++i)
	{
		untracked_points.insert(i);
	}

	unsigned reg_num_require = -1;

	while (untracked_points.size() != 0)
	{
		size_t max_count_point;
		int max_count = -1;
		size_t min_count_point;
		int min_count = graph.size();
		for (size_t p1 : untracked_points)
		{
			unsigned count = 0;
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
			untracked_points.insert(min_count_point);
			stack.push(min_count_point);
			continue;
		}
		untracked_points.insert(max_count_point);
	}
	
	while (stack.size() != 0)
	{
		size_t point = stack.top();
		stack.pop();
		unordered_set<irelem_t> unused_regs;
		unused_regs.reserve(reg_num_require);
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
				unused_regs.erase(save_reg_alloc[p]);
			}
		}
		save_reg_alloc[point] = *unused_regs.begin();
	}
}


void GCPRegisterAllocator::walk()
{
	IrElemAllocator& allocator = *allocator_ptr;
	const IrTable& codes = *origin_ir_table_ptr;
	for (current_index = 0; current_index != codes.size(); ++current_index)
	{
		const Ir& code = codes.at(current_index);
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
			Ir new_code = code;
			new_code.elem[0] = get_reg_of_var(code.elem[0]);
			new_code.elem[1] = trans_val_to_reg_or_cst(code.elem[1]);
			new_code.elem[2] = trans_val_to_reg_or_cst(code.elem[2]);
			buffer.push_back(new_code);
			break;
		}
		case IrHead::lw:
		case IrHead::lb:
		{
			Ir new_code = code;
			new_code.elem[0] = get_reg_of_var(code.elem[0]);
			new_code.elem[1] = trans_val_to_reg_or_cst(code.elem[1]);
			new_code.elem[2] = code.elem[2];
			buffer.push_back(new_code);
			break;
		}
		case IrHead::sw:
		case IrHead::sb:
		{
			Ir new_code = code;
			new_code.elem[0] = trans_val_to_reg_or_cst(code.elem[0]);
			new_code.elem[1] = trans_val_to_reg_or_cst(code.elem[1]);
			new_code.elem[2] = code.elem[2];
			buffer.push_back(new_code);
			break;
		}
		case IrHead::beq:
		case IrHead::bne:
		{
			Ir new_code = code;
			new_code.elem[0] = trans_val_to_reg_or_cst(code.elem[0]);
			new_code.elem[1] = trans_val_to_reg_or_cst(code.elem[1]);
			new_code.elem[2] = code.elem[2];
			buffer.push_back(new_code);
			break;
		}
		case IrHead::push:
		{
			Ir new_code = code;
			new_code.elem[0] = trans_val_to_reg_or_cst(code.elem[0]);
			new_code.elem[1] = code.elem[1];
			new_code.elem[2] = code.elem[2];
			buffer.push_back(new_code);
			break;
		}
		case IrHead::scanf:
		{
			Ir new_code = code;
			new_code.elem[0] = get_reg_of_var(code.elem[0]);
			new_code.elem[1] = code.elem[1];
			new_code.elem[2] = code.elem[2];
			buffer.push_back(new_code);
			break;
		}
		case IrHead::printf:
		{
			Ir new_code = code;
			new_code.elem[0] = code.elem[0];
			new_code.elem[1] = trans_val_to_reg_or_cst(code.elem[1]);
			new_code.elem[2] = code.elem[2];
			buffer.push_back(new_code);
			break;
		}
		case IrHead::label:
		{
			if (IrType::is_func(code.elem[0]) && IrType::is_beg(code.elem[0]))
			{
				next_function_info();
			}
		}
		default:
			buffer.push_back(code);
		}

	}
}

irelem_t GCPRegisterAllocator::trans_val_to_reg_or_cst(irelem_t val)
{
	if (IrType::is_var(val))
	{
		return ensure_var_in_reg(val);
	}
	return val;
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
		})
{
	init_tmp_reg_pool();
}
