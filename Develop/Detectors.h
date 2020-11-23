#pragma once
#ifndef __DETECTORS_H__
#define __DETECTORS_H__

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <forward_list>
#include "global_control.h"
#include "IrTable.h"

using std::forward_list;
using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::string;
using std::unordered_map;
using std::unordered_set;

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

	struct StreamInfo
	{
		vector<unordered_set<irelem_t>> in;
		vector<unordered_set<irelem_t>> out;
	};

	class DataStreamAnalyzeResult
	{
	private:
		vector<StreamInfo> blocks;
		DataStreamAnalyzeResult() = default;
	public:
		const vector<StreamInfo>& get_blocks() const { return blocks; }
		friend shared_ptr<const DataStreamAnalyzeResult> data_stream_analyze(const IrTable& codes, const IrElemAllocator& elems, size_t func_beg_index, const BlockDetectResult& block_detect_result);
	};

	class VarActivetionAnalyzeResult
	{
	private:
		vector<StreamInfo> blocks;
		VarActivetionAnalyzeResult() = default;
	public:
		const vector<StreamInfo>& get_blocks() const { return blocks; }
		friend shared_ptr<const VarActivetionAnalyzeResult> var_activition_analyze(const IrTable& codes, const IrElemAllocator& elems, size_t func_beg_index, const BlockDetectResult& block_detect_result);
	};

	shared_ptr<const unordered_set<irelem_t>> detect_unused_label(const IrTable& codes, const IrElemAllocator& elems);

	shared_ptr<const BlockDetectResult> func_block_detect(const IrTable& codes, const IrElemAllocator& elems, size_t func_beg_index, const unordered_set<irelem_t>& unused_labels);

	shared_ptr<const DataStreamAnalyzeResult> data_stream_analyze(const IrTable& codes, const IrElemAllocator& elems, size_t func_beg_index, const BlockDetectResult& block_detect_result);

	shared_ptr<const VarActivetionAnalyzeResult> var_activition_analyze(const IrTable& codes, const IrElemAllocator& elems, size_t func_beg_index, const BlockDetectResult& block_detect_result);
}


#endif // !__DETECTORS_H__
