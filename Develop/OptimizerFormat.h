#pragma once


#ifndef __OPTIMIZER_FORMAT_H__
#define __OPTIMIZER_FORMAT_H__

#include "IOptimizer.h"

class OptimizerFormat : IOptimizer
{
public:
	const int MAX_SIGNED_16 = 0x7fff;
	const int MIN_SIGNED_16 = ~0x7fff;
	virtual ~OptimizerFormat() = default;
	virtual shared_ptr<IrTable> parse(const IrTable& origin, shared_ptr<IrElemAllocator> allocator);
};

#endif // !__FORMAT_OPTIMIZER_H__

