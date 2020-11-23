#include "Detectors.h"



namespace IrDetectors
{
	shared_ptr<const unordered_set<irelem_t>> detect_unused_label(const IrTable& codes, const IrElemAllocator& elems)
	{
		shared_ptr<unordered_set<irelem_t>> ret = make_shared<unordered_set<irelem_t>>();

		for (const auto& ir : codes)
		{
			switch (ir.head)
			{
			case IrHead::call:
			case IrHead::_goto:
			case IrHead::beq:
			case IrHead::bne:
			{
				irelem_t label = ir.elem[2];
				ASSERT(4, IrType::is_label(label));
				ret->erase(label);
			}
			case IrHead::label:
			{
				irelem_t label = ir.elem[0];
				ASSERT(4, IrType::is_label(label));
				ret->insert(label);
				break;
			}
			default:
				break;
			}
		}
		return ret;
	}


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
		for (size_t i = 0; i != blocks.size(); ++i)
		{
			auto& block = blocks[i];
			const auto& ir = codes.at(block.end - 1);
			irelem_t target;
			switch (ir.head)
			{
			case IrHead::beq:
			case IrHead::bne:
				target = ir.elem[2];
			case IrHead::_goto:
				target = ir.elem[0];
			default:
				continue;
			}
			ASSERT(4, IrType::is_label(target));
			auto target_index_it = label_index.find(target);
			ASSERT(4, target_index_it != label_index.end());
			size_t target_index = target_index_it->second;
			blocks[target_index].fronts.insert(i);
			block.nexts.insert(i);
		}
		return ret;
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