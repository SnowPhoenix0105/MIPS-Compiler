#include "Detectors.h"


#include <forward_list>
#include <unordered_map>
#include <unordered_set>
#include <string>
using std::pair;
using std::unordered_map;
using std::unordered_set;
using std::forward_list;
using std::make_shared;
using std::string;
using std::pair;
using std::make_pair;

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

	irelem_t get_redef_elem(const Ir& ir, const IrElemAllocator& elems)
	{
		switch (ir.head)
		{
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
		case IrHead::lw:	
		case IrHead::lb:
			return ir.elem[0];
		case IrHead::sw:		
		case IrHead::sb:		
			return ir.elem[2];
		case IrHead::push:		
		case IrHead::call:		
			return elems.ret();
		case IrHead::scanf:	
			return ir.elem[0];
		default:
			return IrType::NIL;
		}
	}

	shared_ptr<const DataStreamAnalyzeResult> data_stream_analyze(const IrTable& codes, const IrElemAllocator& elems, size_t func_beg_index, const BlockDetectResult& block_detect_result)
	{
		// TODO
		vector<unordered_set<DefInfo>> gen;
		vector<unordered_set<DefInfo>> kill;
		shared_ptr<DataStreamAnalyzeResult> ret = make_shared<DataStreamAnalyzeResult>();
		vector<unordered_set<DefInfo>>& in = ret->infos.in;
		vector<unordered_set<DefInfo>>& out = ret->infos.out;

		const auto& blocks = block_detect_result.get_blocks();

		size_t beg_index = blocks.at(0).beg;
		size_t end_index = blocks.back().end;

		// 统计所有变量、数组出现的位置
		unordered_map<irelem_t, unordered_set<DefInfo>> var_def_location;
		for (size_t i = 0; i != blocks.size(); ++i)
		{
			auto& block = blocks[i];
			const auto& ir = codes.at(block.end - 1);
			for (size_t j = block.beg; j != block.end; ++j)
			{
				irelem_t redef_elem = get_redef_elem(ir, elems);
				if (redef_elem != IrType::NIL)
				{
					var_def_location[redef_elem].insert(DefInfo(i, j));
				}
			}
		}


		unordered_map<irelem_t, unordered_set<DefInfo>> var_def_location;
		for (size_t i = 0; i != blocks.size(); ++i)
		{
			auto& block = blocks[i];
			const auto& ir = codes.at(block.end - 1);
			for (size_t j = block.beg; j != block.end; ++j)
			{
				irelem_t redef_elem = get_redef_elem(ir, elems);
				if (redef_elem != IrType::NIL)
				{
					var_def_location[redef_elem].insert(DefInfo(i, j));
				}
			}
		}
		// TODO
		for (size_t i = 0; i != blocks.size(); ++i)
		{
			auto& block = blocks[i];
			const auto& ir = codes.at(block.end - 1);
			for (size_t j = block.beg; j != block.end; ++j)
			{
				irelem_t redef_elem = get_redef_elem(ir, elems);
				if (redef_elem != IrType::NIL)
				{
					var_def_location[redef_elem].insert(DefInfo(i, j));
				}
			}
		}
	}


	shared_ptr<const VarActivetionAnalyzeResult> var_activition_analyze(const IrTable& codes, const IrElemAllocator& elems, size_t func_beg_index, const BlockDetectResult& block_detect_result)
	{
		// TODO
	}

}