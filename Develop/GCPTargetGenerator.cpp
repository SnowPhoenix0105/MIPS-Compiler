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

void GCPTargetGenerator::translate(ostream& os)
{
	// TODO
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

	// 统计函数的参数列表
	params.clear();
	for (size_t i = func_mid_index; i != func_beg_index; --i)
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
	}

	alloc_all_save_reg();
}


void GCPRegisterAllocator::alloc_all_save_reg()
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
				unused_regs.erase(save_reg_alloc[vars[p]]);
			}
		}
		save_reg_alloc[vars[point]] = *unused_regs.begin();
	}
}


void GCPRegisterAllocator::walk()
{
	IrElemAllocator& allocator = *allocator_ptr;
	const IrTable& codes = *origin_ir_table_ptr;

	for (current_index = 0; current_index != codes.size(); ++current_index)
	{
		if (block_begs.count(current_index) != 0)
		{
			// 保存被分配至栈上的函数内全局变量
			for (const auto& pair : *var_status)
			{
				if (save_reg_alloc.at(pair.first) == IrType::NIL)
				{
					buffer.push_back(ir.protect(pair.second, pair.first));
				}
			}
			// 保存所有函数外全局变量
			for (const auto& pair : gvar_status)
			{
				if (allocator.is_reg(pair.second))
				{
					buffer.push_back(ir.protect(pair.second, pair.first));
				}
			}
			var_status->clear();
			init_tmp_reg_pool();
			for (irelem_t var : var_activition_analyze_result->get_in(current_index))
			{
				var_status->insert(make_pair(var, save_reg_alloc.at(var)));
			}
		}
		const Ir& code = codes.at(current_index);
		switch (code.head)
		{
		case IrHead::param:
		{
			Ir new_code = code;
			if (save_reg_alloc.count(code.elem[0]) != 0)
			{
				new_code.elem[1] = save_reg_alloc.at(code.elem[0]);
				buffer.push_back(new_code);
				break;
			}
			int index = -1;
			for (size_t i = 0; i != 4 && i < params.size(); ++i)
			{
				if (params[i] == code.elem[0])
				{
					index = i;
				}
			}
			switch (index)
			{
			case 0:
				new_code.elem[1] = allocator.reg(Reg::a0);
				break;
			case 1:
				new_code.elem[1] = allocator.reg(Reg::a1);
				break;
			case 2:
				new_code.elem[1] = allocator.reg(Reg::a2);
				break;
			case 3:
				new_code.elem[1] = allocator.reg(Reg::a3);
				break;
			default:
				break;
			}
			buffer.push_back(new_code);
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
			if (remain_push == 0)
			{
				fresh_push_and_call_info();
			}
			if (remain_push > 4 
				|| params.size() < remain_push 
				|| var_activition_analyze_result->get_out(current_index).count(params[remain_push - 1]) == 0)
			{
				// 不需要保护 $ax
				Ir new_code = code;
				new_code.elem[0] = trans_val_to_reg_or_cst(code.elem[0]);
				new_code.elem[1] = code.elem[1];
				new_code.elem[2] = code.elem[2];
				buffer.push_back(new_code);
			}
			else
			{
				// 此时 $ax 在push后仍旧有活性, 需要判断在call之前知否还有活跃.
				irelem_t param_var = params[remain_push - 1];
				irelem_t reg_ax = var_status->at(param_var);
				ASSERT(4, current_index < call_index);
				bool need_protect_to_reg = false;
				for (size_t i = current_index + 1; i != call_index; ++i)
				{
					irelem_t def_elem;
					irelem_t use_elem_1;
					irelem_t use_elem_2;
					IrDetectors::get_def_and_use_elem(origin_ir_table_ptr->at(i), *allocator_ptr, &def_elem, &use_elem_1, &use_elem_2);
					
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
					buffer.push_back(ir.add(reg, reg_ax, allocator.reg(Reg::zero)));
				}
				else
				{
					var_status->at(param_var) = param_var;
					buffer.push_back(ir.protect(reg_ax, param_var));
				}
			}
			--remain_push;
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

irelem_t GCPRegisterAllocator::get_reg_of_var(irelem_t var)
{
	// TODO
	return irelem_t();
}

irelem_t GCPRegisterAllocator::ensure_var_in_reg(irelem_t var)
{
	// TODO
	return irelem_t();
}

void GCPRegisterAllocator::free_reg_and_protect_content(irelem_t reg)
{
	// TODO
}

irelem_t GCPRegisterAllocator::trans_val_to_reg_or_cst(irelem_t val)
{
	if (IrType::is_var(val))
	{
		return ensure_var_in_reg(val);
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

shared_ptr<const IrTable> GCPRegisterAllocator::build()
{
	// TODO
	return shared_ptr<const IrTable>();
}
