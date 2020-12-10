#include "OpitimizerCommonSubexpressionRemove.h"

shared_ptr<IrTable> OpitimizerCommonSubexpressionRemove::parse(const IrTable& origin, shared_ptr<IrElemAllocator> allocator_ptr)
{
    this->allocator_ptr = allocator_ptr;
    auto& allocator = *allocator_ptr;

    // TODO
    return buffer.build();
}
