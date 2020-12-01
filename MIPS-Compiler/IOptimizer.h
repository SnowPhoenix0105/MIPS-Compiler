
#ifndef __I_OPTIMIZER_H__
#define __I_OPTIMIZER_H__

#include "IrTable.h"


class IOptimizer
{
public:
	virtual ~IOptimizer() = default;
	virtual shared_ptr<IrTable> parse(const IrTable& origin, shared_ptr<IrElemAllocator> allocator) = 0;
};



#endif // !__I_OPTIMIZER_H__
