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
			std::cout << graph[i][j] << '\t';
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
			// 保存被分配至栈上的全局变量
			for (const auto& pair : *var_status)
			{
				auto rlt = save_reg_alloc.find(pair.first);
				if (rlt != save_reg_alloc.end() && rlt->second == IrType::NIL)
				{
					buffer.push_back(ir.protect(pair.second, pair.first));
				}
			}
			// 保存所有 gvar 变量
			for (auto& pair : gvar_status)
			{
				if (allocator.is_reg(pair.second))
				{
					buffer.push_back(ir.protect(pair.second, pair.first));
					pair.second = pair.first;
				}
			}
			var_status->clear();
			init_tmp_reg_pool();
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
			Ir new_code = code;
			new_code.elem[1] = save_reg_alloc.at(code.elem[0]);
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
			keep_in_tx.insert(new_code.elem[0]);

			new_code.elem[1] = trans_val_to_reg_or_cst(code.elem[1]);
			keep_in_tx.insert(new_code.elem[1]);

			new_code.elem[2] = trans_val_to_reg_or_cst(code.elem[2]);
			buffer.push_back(new_code);

			keep_in_tx.erase(new_code.elem[0]);
			keep_in_tx.erase(new_code.elem[1]);
			break;
		}
		case IrHead::lw:
		case IrHead::lb:
		{
			Ir new_code = code;
			new_code.elem[0] = get_reg_of_var(code.elem[0]);
			keep_in_tx.insert(new_code.elem[0]);

			new_code.elem[1] = trans_val_to_reg_or_cst(code.elem[1]);

			new_code.elem[2] = code.elem[2];
			buffer.push_back(new_code);

			keep_in_tx.erase(new_code.elem[0]);
			break;
		}
		case IrHead::sw:
		case IrHead::sb:
		case IrHead::beq:
		case IrHead::bne:
		{
			Ir new_code = code;

			new_code.elem[0] = trans_val_to_reg_or_cst(code.elem[0]);
			keep_in_tx.insert(new_code.elem[0]);

			new_code.elem[1] = trans_val_to_reg_or_cst(code.elem[1]);

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
				new_code.elem[0] = trans_val_to_reg_or_cst(code.elem[0]);
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
						tmp_reg_pool[reg] = param_var;
						buffer.push_back(ir.add(reg, reg_ax, allocator.reg(Reg::zero)));
					}
					else
					{
						var_status->at(param_var) = param_var;
						buffer.push_back(ir.protect(reg_ax, param_var));
					}
				}
				buffer.push_back(ir.add(reg_ax, allocator.reg(Reg::zero), trans_val_to_reg_or_cst(code.elem[0])));
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
			// 不要 break
		}
		default:
			buffer.push_back(code);
		}

	}
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
	irelem_t ret = IrType::NIL;
	const auto& in = var_activition_analyze_result->get_in(current_index);
	for (auto& pair : tmp_reg_pool)
	{
		if (keep_in_tx.count(pair.first) != 0)
		{
			continue;
		}
		irelem_t var = pair.second;
		if (!gvar_status.count(var) != 0 && in.count(var) == 0)
		{
			pair.second = IrType::NIL;
			ret = pair.first;
		}
	}
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
	for (auto& pair : tmp_reg_pool)
	{
		if (keep_in_tx.count(pair.first) != 0)
		{
			continue;
		}
		buffer.push_back(ir.protect(pair.first, pair.second));
		var_status->at(pair.second) = pair.second;
		pair.second = IrType::NIL;
		return pair.first;
	}
}


irelem_t GCPRegisterAllocator::get_reg_of_var(irelem_t var)
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
	if (in_local != var_status->end())
	{
		irelem_t location = in_local->second;
		if (allocator_ptr->is_reg(location))
		{
			return location;
		}
		irelem_t reg = alloc_tmp_reg();
		var_status->at(var) = reg;
		return reg;
	}
	// save_reg_alloc中, 表示是一个全局变量, 但是未被使用
	const auto& in_save = save_reg_alloc.find(var);
	if (in_save != save_reg_alloc.end())
	{
		irelem_t location = in_save->second;
		if (allocator_ptr->is_reg(location))
		{
			var_status->insert(make_pair(var, location));
			return location;
		}
		irelem_t reg = alloc_tmp_reg();
		var_status->at(var) = reg;
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
		var_status->at(var) = reg;
		return reg;
	}
	irelem_t reg = alloc_tmp_reg();
	var_status->insert(make_pair(var, reg));
	return reg;
}

irelem_t GCPRegisterAllocator::ensure_var_in_reg(irelem_t var)
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
	if (in_local != var_status->end())
	{
		irelem_t location = in_local->second;
		if (allocator_ptr->is_reg(location))
		{
			return location;
		}
		irelem_t reg = alloc_tmp_reg();
		if (IrType::is_var(location))
		{
			buffer.push_back(ir.reload(reg, location));
		}
		var_status->at(var) = reg;
		tmp_reg_pool.at(reg) = var;
		return reg;
	}
	// save_reg_alloc中, 表示是一个全局变量, 但是未被使用, 也未被初始化
	const auto& in_save = save_reg_alloc.find(var);
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
		var_status->at(var) = reg;
		tmp_reg_pool.at(reg) = var;
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
		return reg;
	}
	irelem_t reg = alloc_tmp_reg();
	var_status->insert(make_pair(var, reg));
	tmp_reg_pool.at(reg) = var;
	return reg;
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
	return buffer.build();
}
