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
		BlockDetectResult() = default;
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

		bool operator==(const DefInfo& other)
		{
			return block_num == other.block_num && index == other.index;
		}

		bool operator!=(const DefInfo& other)
		{
			return block_num = other.block_num || index != other.index;
		}
	};

	class DataStreamAnalyzeResult
	{
	private:
		StreamInfo<DefInfo> infos;
		DataStreamAnalyzeResult() = default;
	public:
		const StreamInfo<DefInfo>& get_infos() const { return infos; }
		friend shared_ptr<const DataStreamAnalyzeResult> data_stream_analyze(const IrTable& codes, const IrElemAllocator& elems, size_t func_beg_index, const BlockDetectResult& block_detect_result);
	};

	class VarActivetionAnalyzeResult
	{
	private:
		StreamInfo<irelem_t> infos;
		VarActivetionAnalyzeResult() = default;
	public:
		const StreamInfo<irelem_t>& get_infos() const { return infos; }
		friend shared_ptr<const VarActivetionAnalyzeResult> var_activition_analyze(const IrTable& codes, const IrElemAllocator& elems, size_t func_beg_index, const BlockDetectResult& block_detect_result);
	};

	irelem_t get_redef_elem(const Ir& ir, const IrElemAllocator& elems);

	shared_ptr<const unordered_set<irelem_t>> detect_unused_label(const IrTable& codes, const IrElemAllocator& elems);

	shared_ptr<const BlockDetectResult> func_block_detect(const IrTable& codes, const IrElemAllocator& elems, size_t func_beg_index, const unordered_set<irelem_t>& unused_labels);

	shared_ptr<const DataStreamAnalyzeResult> data_stream_analyze(const IrTable& codes, const IrElemAllocator& elems, size_t func_beg_index, const BlockDetectResult& block_detect_result);

	shared_ptr<const VarActivetionAnalyzeResult> var_activition_analyze(const IrTable& codes, const IrElemAllocator& elems, size_t func_beg_index, const BlockDetectResult& block_detect_result);
}


#endif // !__DETECTORS_H__
