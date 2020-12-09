#include "OptimizerRemoveEmptyFunction.h"
#include <unordered_map>

using std::unordered_map;
using std::make_pair;

void OptimizerRemoveEmptyFunction::next_call(const IrTable& origin, size_t i)
{
	unsigned count = 0;
	for (size_t j = i + 1; j != origin.size(); ++j)
	{
		const auto& code = origin.at(j);
		if (code.head == IrHead::label && IrType::is_func(code.elem[0]) && IrType::is_end(code.elem[0]))
		{
			return;
		}
		if (code.head == IrHead::push)
		{
			++count;
		}
		if (code.head == IrHead::call)
		{
			irelem_t target = code.elem[0];
			auto it = empty_func_beg_to_param_count_map.find(target);
			if (it != empty_func_beg_to_param_count_map.end())
			{
				ASSERT(4, it->second == count);
				next_func_need_remove = true;
			}
			else
			{
				next_func_need_remove = false;
			}
			return;
		}
	}
}

shared_ptr<IrTable> OptimizerRemoveEmptyFunction::parse(const IrTable& origin, shared_ptr<IrElemAllocator> allocator_ptr)
{
	IrTableBuilder builder;
	IrTableBuilder buffer;
	IrFactory ir(allocator_ptr);
	IrElemAllocator& allocator = *allocator_ptr;
	state_t status = state_t::end; 
	irelem_t func_beg;
	string func_name;
	unsigned count = 0;
	empty_func_beg_to_param_count_map.clear();
	empty_main = false;
	next_func_need_remove = false;

	for (size_t i = 0; i != origin.size(); ++i)
	{
		const auto& code = origin.at(i);
		switch (status)
		{
		case state_t::end:
			if (code.head == IrHead::label)
			{
				irelem_t label = code.elem[0];
				if (IrType::is_func(label))
				{
					ASSERT(4, IrType::is_beg(label));
					func_name = allocator.func_name(label);
					count = 0;
					status = state_t::beg;
					func_beg = label;
					auto ir_table = buffer.build();
					builder.push_back_all(*ir_table);
					buffer.clear();
				}
			}
			buffer.push_back(code);
			break;
		case state_t::beg:
			buffer.push_back(code);
			if (code.head == IrHead::label)
			{
				irelem_t label = code.elem[0];
				if (IrType::is_func(label))
				{
					ASSERT(4, IrType::is_mid(label));
					ASSERT(4, func_name == allocator.func_name(label));
					status = state_t::mid;
					break;
				}
			}
			if (code.head == IrHead::param)
			{
				++count;
				break;
			}
			break;
		case state_t::mid:
			buffer.push_back(code);
			if (code.head == IrHead::label)
			{
				irelem_t label = code.elem[0];
				if (IrType::is_func(label))
				{
					ASSERT(4, IrType::is_end(label));
					ASSERT(4, func_name == allocator.func_name(label));
					empty_func_beg_to_param_count_map.insert(make_pair(func_beg, count));
					status = state_t::end;
					buffer.clear();
					break;
				}
			}
			if (code.head == IrHead::ret)
			{
				status = state_t::dying;
				break;
			}
			{
				status = state_t::reading;
				next_call(origin, i);
			}
			break;
		case state_t::dying:
			if (code.head == IrHead::label)
			{
				irelem_t label = code.elem[0];
				if (IrType::is_func(label))
				{
					ASSERT(4, IrType::is_end(label));
					ASSERT(4, func_name == allocator.func_name(label));
					empty_func_beg_to_param_count_map.insert(make_pair(func_beg, count));
					status = state_t::end;
					buffer.clear();
					break;
				}
			}
			break;
		case state_t::reading:
			if (code.head == IrHead::push)
			{
				if (next_func_need_remove)
					break;
			}
			if (code.head == IrHead::call)
			{
				bool save = next_func_need_remove;
				next_call(origin, i);
				if (save)
				{
					break;
				}
			}
			if (code.head == IrHead::label)
			{
				irelem_t label = code.elem[0];
				if (IrType::is_func(label))
				{
					ASSERT(4, IrType::is_end(label));
					ASSERT(4, func_name == allocator.func_name(label));
					status = state_t::end;
					buffer.push_back(code);
					break;
				}
			}
			buffer.push_back(code);
			break;
		}
	}
	auto ir_table = buffer.build();
	builder.push_back_all(*ir_table);
	return builder.build();
}
