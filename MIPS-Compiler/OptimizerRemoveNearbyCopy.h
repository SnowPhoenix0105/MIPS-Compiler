
#ifndef __OPTIMIZER_REMOVE_NEARBY_COPY_H__
#define __OPTIMIZER_REMOVE_NEARBY_COPY_H__

#include "global_control.h"
#include "IOptimizer.h"
#include "IrTable.h"
#include "Detectors.h"
#include <memory>

using std::unique_ptr;


class OptimizerRemoveNearbyCopy : public IOptimizer
{
private:
	size_t current_index;

	irelem_t copy_target(const IrTable& origin);
	shared_ptr<IrElemAllocator> allocator_ptr;
	shared_ptr<const IrDetectors::BlockDetectResult> block_detect_result;
	shared_ptr<const IrDetectors::BlockVarActivetionAnalyzeResult> block_var_activition_analyze_result;
	shared_ptr<const IrDetectors::VarActivetionAnalyzeResult> var_activition_analyze_result;
public:
	virtual ~OptimizerRemoveNearbyCopy() = default;
	virtual shared_ptr<IrTable> parse(const IrTable & origin, shared_ptr<IrElemAllocator> allocator);
};




#endif // !__OPTIMIZER_REMOVE_NEARBY_COPY_H__
