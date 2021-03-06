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

	unique_ptr<FromToInfo> detect_copy(const Ir& code, const IrElemAllocator& allocator)
	{
		switch (code.head)
		{
		case IrHead::add:
		case IrHead::_or:
			if (IrType::is_imm(code.elem[1]) && allocator.imm_to_value(code.elem[1]) == 0 && IrType::is_var(code.elem[2]))
			{
				return FromToInfo::build_unique(code.elem[2], code.elem[0]);
			}
			if (code.elem[1] == allocator.zero() && IrType::is_var(code.elem[2]))
			{
				return FromToInfo::build_unique(code.elem[2], code.elem[0]);
			}
			if (IrType::is_imm(code.elem[2]) && allocator.imm_to_value(code.elem[2]) == 0 && IrType::is_var(code.elem[1]))
			{
				return FromToInfo::build_unique(code.elem[1], code.elem[0]);
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

	shared_ptr<const unordered_set<irelem_t>> detect_unused_label(const IrTable& codes, const IrElemAllocator& elems)
	{
		shared_ptr<unordered_set<irelem_t>> ret = make_shared<unordered_set<irelem_t>>();
		unordered_set<irelem_t> used_set;

		for (const auto& ir : codes)
		{
			switch (ir.head)
			{
			case IrHead::call:
			case IrHead::_goto:
			{
				irelem_t label = ir.elem[0];
				ASSERT(4, IrType::is_label(label));
				used_set.insert(label);
				//ret->erase(label);
				break;
			}
			case IrHead::beq:
			case IrHead::bne:
			{
				irelem_t label = ir.elem[2];
				ASSERT(4, IrType::is_label(label));
				used_set.insert(label);
				//ret->erase(label);
				break;
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

		for (irelem_t used : used_set)
		{
			ret->erase(used);
		}
		return ret;
	}

	shared_ptr<const BlockDetectResult> func_block_detect(
		const IrTable& codes,
		const IrElemAllocator& elems,
		size_t func_beg_index)
	{
		return func_block_detect(codes, elems, func_beg_index, *detect_unused_label(codes, elems));
	}

	shared_ptr<const BlockDetectResult> func_block_detect(const IrTable& codes, const IrElemAllocator& elems, size_t func_beg_index, const unordered_set<irelem_t>& unused_labels)
	{
		unordered_map<irelem_t, size_t> label_index;
		shared_ptr<BlockDetectResult> ret = make_shared<BlockDetectResult>();
		const unordered_set<IrHead> end_set = { IrHead::label, IrHead::bne, IrHead::beq, IrHead::_goto, IrHead::ret };
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
				size_t idx = index + 1;
				if (head == IrHead::label)
				{
					const auto label = ir.elem[0];
					ASSERT(4, IrType::is_label(label));
					if (IrType::is_func(label) && IrType::is_end(label))
					{
						// ????????
						if (blocks.back().beg == index)
						{
							blocks.pop_back();
						}
						else
						{
							blocks.back().end = index;
						}
						break;
					}
					if (unused_labels.count(label) != 0)
					{
						// ??????????, ??????????????????????????
						continue;
					}
					if (init)
					{
						// ??????????????????????????
						label_index.insert(make_pair(label, current_block_index));
						continue;
					}
					--idx;
					label_index.insert(make_pair(label, current_block_index + 1));
				}
				// TODO ??????????
				if (blocks[current_block_index].end == idx)
				{
					continue;
				}
				blocks[current_block_index].end = idx;
				blocks.emplace_back();
				blocks.back().beg = idx;
				init = true;
				if (head != IrHead::_goto && head != IrHead::ret)
				{
					// ????????????????????
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
				break;
			case IrHead::_goto:
				target = ir.elem[0];
				break;
			default:
				continue;
			}
			ASSERT(4, IrType::is_label(target));
			auto target_index_it = label_index.find(target);
			ASSERT(4, target_index_it != label_index.end());
			size_t target_index = target_index_it->second;
			blocks[target_index].fronts.insert(i);
			block.nexts.insert(target_index);
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
		case IrHead::movn:
		case IrHead::movz:
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

		const auto& blocks = block_detect_result.get_blocks();
		size_t block_count = blocks.size();
		vector<unordered_set<DefInfo>> gen(block_count);
		vector<unordered_set<DefInfo>> kill(block_count);
		shared_ptr<DataStreamAnalyzeResult> ret = make_shared<DataStreamAnalyzeResult>();
		vector<unordered_set<DefInfo>>& in = ret->infos.in;
		vector<unordered_set<DefInfo>>& out = ret->infos.out;
		in.resize(block_count);
		out.resize(block_count);

		size_t beg_index = blocks.at(0).beg;
		size_t end_index = blocks.back().end;

		// ????????????????????????????
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
		}

		throw;
	}

	void get_def_and_use_elem(
		const Ir& ir,
		const IrElemAllocator& elems,
		irelem_t* def_elem,
		irelem_t* use_elem_1,
		irelem_t* use_elem_2,
		irelem_t* use_elem_3)
	{
		*use_elem_3 = IrType::NIL;
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
			*def_elem = ir.elem[0];
			*use_elem_1 = ir.elem[1];
			*use_elem_2 = ir.elem[2];
			return;
		case IrHead::lw:		
		case IrHead::lb:
			*def_elem = ir.elem[0];
			*use_elem_1 = ir.elem[1];
			*use_elem_2 = elems.arr_root(ir.elem[2]);
			return;
		case IrHead::sw:		
		case IrHead::sb:
			*def_elem = elems.arr_root(ir.elem[2]);
			*use_elem_1 = ir.elem[0];
			*use_elem_2 = ir.elem[1];
			return;
		case IrHead::beq:	
		case IrHead::bne:
			*def_elem = IrType::NIL;
			*use_elem_1 = ir.elem[0];
			*use_elem_2 = ir.elem[1];
			return;
		case IrHead::push:
			*def_elem = IrType::NIL;
			*use_elem_1 = ir.elem[0];
			*use_elem_2 = IrType::NIL;
			return;
		case IrHead::scanf:
			*def_elem = ir.elem[0];
			*use_elem_1 = IrType::NIL;
			*use_elem_2 = IrType::NIL;
			return;
		case IrHead::printf:
			*def_elem = IrType::NIL;
			*use_elem_1 = ir.elem[1];
			*use_elem_2 = IrType::NIL;
			return;
		case IrHead::movn:
		case IrHead::movz:
			*def_elem = ir.elem[0];
			*use_elem_1 = ir.elem[1];
			*use_elem_2 = ir.elem[2];
			*use_elem_3 = ir.elem[0];
			return;
		default:
			*def_elem = IrType::NIL;
			*use_elem_1 = IrType::NIL;
			*use_elem_2 = IrType::NIL;
			return;
		}
	}



	shared_ptr<const BlockVarActivetionAnalyzeResult> block_var_activition_analyze(
		const IrTable& codes,
		const IrElemAllocator& elems,
		size_t func_beg_index,
		const BlockDetectResult& block_detect_result)
	{
		// TODO

		const auto& blocks = block_detect_result.get_blocks();
		size_t block_count = blocks.size();
		vector<unordered_set<irelem_t>> def(block_count);
		vector<unordered_set<irelem_t>> use(block_count);
		shared_ptr<BlockVarActivetionAnalyzeResult> ret = make_shared<BlockVarActivetionAnalyzeResult>();
		vector<unordered_set<irelem_t>>& in = ret->infos.in;
		vector<unordered_set<irelem_t>>& out = ret->infos.out;
		in.resize(block_count);
		out.resize(block_count);

		// ????use??def
		for (size_t i = 0; i != block_count; ++i)
		{
			const auto& block = blocks.at(i);
			for (size_t j = block.beg; j != block.end; ++j)
			{
				irelem_t def_elem;
				irelem_t use_elem_1;
				irelem_t use_elem_2;
				irelem_t use_elem_3;
				const auto& ir = codes.at(j);
				get_def_and_use_elem(ir, elems, &def_elem, &use_elem_1, &use_elem_2, &use_elem_3);
				if (use_elem_1 != IrType::NIL 
					&& IrType::is_var(use_elem_1) 
					&& !elems.is_global_var(use_elem_1) 
					&& !elems.is_reserved_var(use_elem_1) 
					&& def[i].count(use_elem_1) == 0)
				{
					use[i].insert(use_elem_1);
				}
				if (use_elem_2 != IrType::NIL 
					&& IrType::is_var(use_elem_2) 
					&& !elems.is_global_var(use_elem_2) 
					&& !elems.is_reserved_var(use_elem_2) 
					&& def[i].count(use_elem_2) == 0)
				{
					use[i].insert(use_elem_2);
				}
				if (use_elem_3 != IrType::NIL
					&& IrType::is_var(use_elem_3)
					&& !elems.is_global_var(use_elem_3)
					&& !elems.is_reserved_var(use_elem_3)
					&& def[i].count(use_elem_3) == 0)
				{
					use[i].insert(use_elem_3);
				}
				if (def_elem != IrType::NIL 
					&& IrType::is_var(def_elem) 
					&& !elems.is_global_var(def_elem) 
					&& !elems.is_reserved_var(def_elem)
					&& use[i].count(def_elem) == 0)
				{
					def[i].insert(def_elem);
				}
			}
		}

		// ??use????????in????
		for (size_t i = 0; i != block_count; ++i)
		{
			for (irelem_t elem : use[i])
			{
				in[i].insert(elem);
			}
		}

		bool changed = true;
		while (changed)
		{
			changed = false;
			for (size_t i = 0; i != block_count; ++i)
			{
				size_t current = block_count - i - 1;
				const auto& block = blocks.at(current);
				for (size_t next : block.nexts)
				{
					for (irelem_t elem : in[next])
					{
						// ?? in[next] ?????? out[current] ??
						if (out[current].count(elem) == 0)
						{
							out[current].insert(elem);
							if (def[current].count(elem) == 0)
							{
								// ?? in[next] - def[current] ?????? in[current] ??
								in[current].insert(elem);
							}
							changed = true;
						}
					}
				}
			}
		}

		return ret;
	}




	shared_ptr<const VarActivetionAnalyzeResult> var_activition_analyze(
		const IrTable& codes,
		const IrElemAllocator& elems,
		size_t func_beg_index,
		const BlockDetectResult& block_detect_result,
		const BlockVarActivetionAnalyzeResult& block_var_activition_analyze_result)
	{
		const auto& blocks = block_detect_result.get_blocks();
		size_t block_count = blocks.size();

		shared_ptr<VarActivetionAnalyzeResult> ret = make_shared<VarActivetionAnalyzeResult>();
		size_t beg_index = blocks.at(0).beg;
		size_t end_index = blocks.back().end;
		ret->base_index = beg_index;

		vector<unordered_set<irelem_t>>& in = ret->infos.in;
		vector<unordered_set<irelem_t>>& out = ret->infos.out;
		in.resize(end_index - beg_index);
		out.resize(end_index - beg_index);
		
		for (size_t i = 0; i != block_count; ++i)
		{
			const auto& block = blocks.at(i);
			out[block.end - 1 - beg_index] = block_var_activition_analyze_result.get_infos().out.at(i);
			for (size_t j = 0; j != block.end - block.beg; ++j)
			{
				size_t current = block.end - 1 - j - beg_index;
				in[current] = out[current];

				irelem_t def_elem;
				irelem_t use_elem_1;
				irelem_t use_elem_2;
				irelem_t use_elem_3;
				const auto& ir = codes.at(block.end - 1 - j);
				get_def_and_use_elem(ir, elems, &def_elem, &use_elem_1, &use_elem_2, &use_elem_3);
				if (def_elem != IrType::NIL 
					&& IrType::is_var(def_elem) 
					&& !elems.is_global_var(def_elem)
					&& !elems.is_reserved_var(def_elem))
				{
					in[current].erase(def_elem);
				}
				if (use_elem_1 != IrType::NIL 
					&& IrType::is_var(use_elem_1) 
					&& !elems.is_global_var(use_elem_1)
					&& !elems.is_reserved_var(use_elem_1))
				{
					in[current].insert(use_elem_1);
				}
				if (use_elem_2 != IrType::NIL 
					&& IrType::is_var(use_elem_2) 
					&& !elems.is_global_var(use_elem_2)
					&& !elems.is_reserved_var(use_elem_2))
				{
					in[current].insert(use_elem_2);
				}
				if (use_elem_3 != IrType::NIL
					&& IrType::is_var(use_elem_3)
					&& !elems.is_global_var(use_elem_3)
					&& !elems.is_reserved_var(use_elem_3))
				{
					in[current].insert(use_elem_3);
				}

				if (j != block.end - block.beg - 1)
				{
					out[current - 1] = in[current];
				}
			}
			ASSERT(4, in[block.beg - beg_index] == block_var_activition_analyze_result.get_infos().in.at(i));
		}
		return ret;
	}
}