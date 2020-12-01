#pragma once


#ifndef __OPTIMIZER_FORMAT_H__
#define __OPTIMIZER_FORMAT_H__

#include "IOptimizer.h"

class OptimizerFormat : IOptimizer
{
public:
	virtual ~OptimizerFormat() = default;
	virtual shared_ptr<IrTable> parse(const IrTable& origin, shared_ptr<IrElemAllocator> allocator);
};

#endif // !__FORMAT_OPTIMIZER_H__

