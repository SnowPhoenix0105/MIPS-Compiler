#include "OptimizerRemoveNearbyCopy.h"
#include <unordered_set>

using std::unordered_set;

unique_ptr<FromToInfo> OptimizerRemoveNearbyCopy::detect_copy(const Ir& code)
{
	const auto& allocator = *allocator_ptr;
	switch (code.head)
	{
	case IrHead::add:
	case IrHead::_or:
		if (IrType::is_imm(code.elem[1]) && IrType::is_var(code.elem[2]) && allocator.imm_to_value(code.elem[1]) == 0)
		{
			return FromToInfo::build_unique(code.elem[2], code.elem[0]);
		}
		if (IrType::is_imm(code.elem[2]) && IrType::is_var(code.elem[1]) && allocator.imm_to_value(code.elem[2]) == 0)
		{
			return FromToInfo::build_unique(code.elem[1], code.elem[0]);
		}
		if (code.elem[1] == allocator.zero() && IrType::is_var(code.elem[2]))
		{
			return FromToInfo::build_unique(code.elem[2], code.elem[0]);
		}
		if (code.elem[2] == allocator.zero() && IrType::is_var(code.elem[1]))
		{
			return FromToInfo::build_unique(code.elem[1], code.elem[0]);
		}
		return nullptr;
	case IrHead::mult:
		if (IrType::is_imm(code.elem[1]) && allocator.imm_to_value(code.elem[1]) == 1 && IrType::is_var(code.elem[2]))
		{
			return FromToInfo::build_unique(code.elem[2], code.elem[0]);
		}
	case IrHead::div:
		if (IrType::is_imm(code.elem[2]) && allocator.imm_to_value(code.elem[2]) == 1 && IrType::is_var(code.elem[1]))
		{
			return FromToInfo::build_unique(code.elem[1], code.elem[0]);
		}
		return nullptr;
	case IrHead::_and:
		if (IrType::is_imm(code.elem[1]) && allocator.imm_to_value(code.elem[1]) == ~0 && IrType::is_var(code.elem[2]))
		{
			return FromToInfo::build_unique(code.elem[2], code.elem[0]);
		}
		if (IrType::is_imm(code.elem[2]) && allocator.imm_to_value(code.elem[2]) == ~0 && IrType::is_var(code.elem[1]))
		{
			return FromToInfo::build_unique(code.elem[1], code.elem[0]);
		}
		return nullptr;
	case IrHead::sub:
	case IrHead::sl:
	case IrHead::sr:
		if (IrType::is_imm(code.elem[2]) && allocator.imm_to_value(code.elem[2]) == 0 && IrType::is_var(code.elem[1]))
		{
			return FromToInfo::build_unique(code.elem[1], code.elem[0]);
		}
		if (code.elem[2] == allocator.zero() && IrType::is_var(code.elem[1]))
		{
			return FromToInfo::build_unique(code.elem[1], code.elem[0]);
		}
		return nullptr;
	default:
		return nullptr;
	}
}

irelem_t OptimizerRemoveNearbyCopy::copy_target(const IrTable& origin)
{
	irelem_t ret = origin.at(current_index).elem[0];
	while (true)
	{
		size_t next_index = current_index + 1;
		unique_ptr<FromToInfo> cpinfo = detect_copy(origin.at(next_index));
		if (cpinfo == nullptr)
		{
			// 未发生复制
			return ret;
		}
		if (var_activition_analyze_result->get_out(next_index).count(ret) != 0)
		{
			// 复制后, 源仍然活跃
			return ret;
		}
		if (cpinfo->from != ret)
		{
			// 并非从当前值进行的复制
			return ret;
		}
		ret = cpinfo->to;
		current_index = next_index;
	}
}

shared_ptr<IrTable> OptimizerRemoveNearbyCopy::parse(const IrTable& origin, shared_ptr<IrElemAllocator> allocator)
{
	allocator_ptr = allocator;
	IrTableBuilder builder;
	unordered_set<IrHead> write_ops =
	{
		IrHead::add,
		IrHead::sub,
		IrHead::mult,
		IrHead::div,
		IrHead::_and,
		IrHead::_or,
		IrHead::_nor,
		IrHead::_xor,
		IrHead::sl,
		IrHead::sr,
		IrHead::less,
		IrHead::lw,
		IrHead::lb,
		IrHead::scanf,
	};
	for (size_t current_index = 0; current_index != origin.size(); ++current_index)
	{
		const auto& code = origin.at(current_index);
		if (write_ops.count(code.head) != 0)
		{
			Ir new_code(code);
			new_code.elem[0] = copy_target(origin);
			builder.push_back(new_code);
			continue;
		}
		if (code.head == IrHead::label && IrType::is_func(code.elem[0]) && IrType::is_beg(code.elem[0]))
		{
			block_detect_result = IrDetectors::func_block_detect(origin, *allocator_ptr, current_index);
			block_var_activition_analyze_result = IrDetectors::block_var_activition_analyze(origin, *allocator_ptr, current_index, *block_detect_result);
			var_activition_analyze_result = IrDetectors::var_activition_analyze(origin, *allocator_ptr, current_index, *block_detect_result, *block_var_activition_analyze_result);
		}
		builder.push_back(code);
	}
	return builder.build();
}
