
#ifndef __OPTIMIZER_CALCULATE_STRENGTH_REDUCTION_H__
#define __OPTIMIZER_CALCULATE_STRENGTH_REDUCTION_H__

#include "IOptimizer.h"
class OptimizerCalculateStrengthReduction :
    public IOptimizer
{
private:
	/// <summary>
	/// 如果存在 n < 32, 使得 num == 2^n, 则返回n, 否则返回 -1
	/// </summary>
	/// <param name="num"></param>
	/// <returns></returns>
	int pow2(unsigned num);
	void trans_mult(const Ir& origin);
	void trans_div(const Ir& origin);

	shared_ptr<IrElemAllocator> allocator_ptr;
	IrTableBuilder buffer;
public:
	virtual ~OptimizerCalculateStrengthReduction() = default;
	virtual shared_ptr<IrTable> parse(const IrTable& origin, shared_ptr<IrElemAllocator> allocator);
};

#endif // !__OPTIMIZER_CALCULATE_STRENGTH_REDUCTION_H__


