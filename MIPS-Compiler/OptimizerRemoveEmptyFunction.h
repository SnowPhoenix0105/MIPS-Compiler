
#ifndef __OPTIMIZER_REMOVE_EMPTY_FUNCTION__
#define __OPTIMIZER_REMOVE_EMPTY_FUNCTION__


#include "IOptimizer.h"


enum class OptimizerRemoveEmptyFunctionState
{
	end, beg, mid, dying, reading
};

class OptimizerRemoveEmptyFunction :
    public IOptimizer
{
private:
	using state_t = OptimizerRemoveEmptyFunctionState;
	unordered_map<irelem_t, unsigned> empty_func_beg_to_param_count_map;
	bool next_func_need_remove = false;

	void next_call(const IrTable& origin, size_t i);
public:
	bool empty_main = false;
	virtual ~OptimizerRemoveEmptyFunction() = default;
	virtual shared_ptr<IrTable> parse(const IrTable & origin, shared_ptr<IrElemAllocator> allocator_ptr);
};


#endif // !__OPTIMIZER_REMOVE_EMPTY_FUNCTION__

