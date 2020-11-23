#include "Detectors.h"



namespace IrDetectors
{
	shared_ptr<const BlockDetectResult> func_block_detect(const IrTable& codes, const IrElemAllocator& elems, size_t func_beg_index, const unordered_set<irelem_t>& unused_labels)
	{
		unordered_map<irelem_t, size_t> label_index;
		shared_ptr<BlockDetectResult> ret = make_shared<BlockDetectResult>();
		const unordered_set<IrHead> end_set = { IrHead::label, IrHead::bne, IrHead::beq, IrHead::_goto };
		auto& blocks = ret->blocks;

		size_t index = func_beg_index;
		for (;; ++index)
		{
			const auto& ir = codes.at(index);
			if (ir.head != IrHead::label) continue;
			const auto label = ir.elem[0];
			ASSERT(4, IrType::is_label(label));
			if (!IrType::is_func(label)) continue;
			if (!IrType::is_mid(label)) continue;
			break;
		}
		blocks.emplace_back();
		blocks.back().beg = ++index;
		bool init = true;
		for (;; ++index)
		{
			const auto& ir = codes.at(index);
			const auto head = ir.head;
			if (end_set.count(head) != 0)
			{
				size_t current_block_index = ret->blocks.size() - 1;
				if (head == IrHead::label)
				{
					const auto label = ir.elem[0];
					ASSERT(4, IrType::is_label(label));
					if (IrType::is_func(label) && IrType::is_end(label))
					{
						// 函数结束
						blocks.back().end = index;
						break;
					}
					if (init)
					{
						// 当前基本块多了一个入口标签
						label_index.insert(make_pair(label, current_block_index));
						continue;
					}
					if (unused_labels.count(label) != 0)
					{
						// 无用的标签, 不会有其它基本块跳入到这里
						continue;
					}
				}
				// TODO 基本块结束
				blocks[current_block_index].end = index;
				blocks.emplace_back();
				blocks.back().beg = index;
				init = true;
				if (head != IrHead::_goto)
				{
					// 连接相连的两个基本块
					blocks[current_block_index].nexts.insert(current_block_index + 1);
					blocks.back().fronts.insert(current_block_index);
				}
			}
			else
			{
				init = false;
			}
		}
	}

	shared_ptr<const DataStreamAnalyzeResult> data_stream_analyze(const IrTable& codes, const IrElemAllocator& elems, size_t func_beg_index, const BlockDetectResult& block_detect_result)
	{
		// TODO
	}


	shared_ptr<const VarActivetionAnalyzeResult> var_activition_analyze(const IrTable& codes, const IrElemAllocator& elems, size_t func_beg_index, const BlockDetectResult& block_detect_result)
	{
		// TODO
	}

}