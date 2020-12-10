#pragma once

#ifndef __OPTIMIZER_CALCULATE_STRENGTH_REDUCTION_H__
#define __OPTIMIZER_CALCULATE_STRENGTH_REDUCTION_H__

#include "IOptimizer.h"
class OptimizerCalculateStrengthReduction :
    public IOptimizer
{
private:
	/// <summary>
	/// ������� n < 32, ʹ�� num == 2^n, �򷵻�n, ���򷵻� -1
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

