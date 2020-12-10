#pragma once

#ifndef __OPTIMIZER_COMMON_SUBEXPRESSION_REMOVE_H__
#define __OPTIMIZER_COMMON_SUBEXPRESSION_REMOVE_H__

#include "global_control.h"
#include "IrTable.h"
#include "IOptimizer.h"
#include "Detectors.h"
#include "Graph.h"
#include <memory>

using std::shared_ptr;
using std::make_shared;

class OpitimizerCommonSubexpressionRemove :
	public IOptimizer
{
private:
	IrTableBuilder buffer;
	shared_ptr<const IrDetectors::BlockDetectResult> block_detect_result;
	shared_ptr<const IrDetectors::BlockVarActivetionAnalyzeResult> block_var_activition_analyze_result;
	IrFactory ir;
	Graph<unsigned> graph;
	shared_ptr<IrElemAllocator> allocator_ptr;
public:
	virtual ~OpitimizerCommonSubexpressionRemove() = default;
	virtual shared_ptr<IrTable> parse(const IrTable& origin, shared_ptr<IrElemAllocator> allocator_ptr);

};



#endif // !__OPTIMIZER_COMMON_SUBEXPRESSION_REMOVE_H__
