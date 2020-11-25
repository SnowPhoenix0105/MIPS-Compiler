#pragma once
#ifndef __DETECTORS_H__
#define __DETECTORS_H__

#include <memory>
#include <vector>
#include "global_control.h"
#include "IrTable.h"

using std::shared_ptr;
using std::vector;

namespace IrDetectors
{
	struct BlockInfo
	{
		size_t beg;
		size_t end;		// 尾后下标
		unordered_set<size_t> nexts;		// 后驱
		unordered_set<size_t> fronts;		// 前驱
	};

	class BlockDetectResult
	{
	private:
		vector<BlockInfo> blocks;
	public:
		const vector<BlockInfo>& get_blocks() const { return blocks; }
		friend shared_ptr<const BlockDetectResult> func_block_detect(const IrTable& codes, const IrElemAllocator& elems, size_t func_beg_index, const unordered_set<irelem_t>& unused_labels);
	};

	template<typename T>
	struct StreamInfo
	{
		vector<unordered_set<T>> in;
		vector<unordered_set<T>> out;
	};

	struct DefInfo
	{
		size_t block_num;
		size_t index;

		DefInfo() = default;
		DefInfo(size_t block_num, size_t index) : block_num(block_num), index(index) { }

		bool operator==(const DefInfo& other) const noexcept
		{
			return block_num == other.block_num && index == other.index;
		}

		bool operator!=(const DefInfo& other) const noexcept
		{
			return block_num != other.block_num || index != other.index;
		}
	};

	class DataStreamAnalyzeResult
	{
	private:
		StreamInfo<DefInfo> infos;
	public:
		const StreamInfo<DefInfo>& get_infos() const { return infos; }
		friend shared_ptr<const DataStreamAnalyzeResult> data_stream_analyze(const IrTable& codes, const IrElemAllocator& elems, size_t func_beg_index, const BlockDetectResult& block_detect_result);
	};

	class BlockVarActivetionAnalyzeResult
	{
	private:
		StreamInfo<irelem_t> infos;
	public:
		const StreamInfo<irelem_t>& get_infos() const { return infos; }
		friend shared_ptr<const BlockVarActivetionAnalyzeResult> block_var_activition_analyze(
			const IrTable& codes,
			const IrElemAllocator& elems,
			size_t func_beg_index,
			const BlockDetectResult& block_detect_result);
	};

	class VarActivetionAnalyzeResult
	{
	private:
		StreamInfo<irelem_t> infos;
		size_t base_index;
	public:
		const unordered_set<irelem_t>& get_in(size_t index) const { return infos.in.at(index - base_index); }
		const unordered_set<irelem_t>& get_out(size_t index) const { return infos.out.at(index - base_index); }
		friend shared_ptr<const VarActivetionAnalyzeResult> var_activition_analyze(
				const IrTable& codes,
				const IrElemAllocator& elems,
				size_t func_beg_index,
				const BlockDetectResult& block_detect_result,
				const BlockVarActivetionAnalyzeResult& block_var_activition_analyze_result);
	};

	irelem_t get_redef_elem(const Ir& ir, const IrElemAllocator& elems);

	void get_def_and_use_elem(
		const Ir& ir,
		const IrElemAllocator& elems,
		irelem_t* def_elem,
		irelem_t* use_elem_1, 
		irelem_t* use_elem_2);

	shared_ptr<const unordered_set<irelem_t>> detect_unused_label(
		const IrTable& codes, 
		const IrElemAllocator& elems);

	shared_ptr<const BlockDetectResult> func_block_detect(
		const IrTable& codes,
		const IrElemAllocator& elems,
		size_t func_beg_index);


	shared_ptr<const BlockDetectResult> func_block_detect(
		const IrTable& codes, 
		const IrElemAllocator& elems, 
		size_t func_beg_index, 
		const unordered_set<irelem_t>& unused_labels);


	shared_ptr<const DataStreamAnalyzeResult> data_stream_analyze(
		const IrTable& codes, 
		const IrElemAllocator& elems, 
		size_t func_beg_index, 
		const BlockDetectResult& block_detect_result);


	shared_ptr<const BlockVarActivetionAnalyzeResult> block_var_activition_analyze(
		const IrTable& codes, 
		const IrElemAllocator& elems, 
		size_t func_beg_index, 
		const BlockDetectResult& block_detect_result);


	shared_ptr<const VarActivetionAnalyzeResult> var_activition_analyze(
		const IrTable& codes, 
		const IrElemAllocator& elems, 
		size_t func_beg_index, 
		const BlockDetectResult& block_detect_result,
		const BlockVarActivetionAnalyzeResult& block_var_activition_analyze_result);
}

namespace std
{
	template<>
	struct hash<IrDetectors::DefInfo>
	{
		size_t operator()(const IrDetectors::DefInfo& info) const noexcept
		{
			return info.index ^ info.block_num;
		}
	};
}

#endif // !__DETECTORS_H__
