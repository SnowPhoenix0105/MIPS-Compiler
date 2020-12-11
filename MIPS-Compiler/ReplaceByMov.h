
#ifndef __REPLACE_BY_MOV_H__
#define __REPLACE_BY_MOV_H__

#include "global_control.h"
#include "IOptimizer.h"
#include "IrTable.h"
#include "global_control.h"
#include "Detectors.h"
#include <memory>

class ReplaceByMov
{
private:
	shared_ptr<IrElemAllocator> allocator_ptr;
	size_t current_index;
	IrTableBuilder builder;

	void check_bne(const IrTable& origin);
public:
	shared_ptr<IrTable> parse(const IrTable& origin, shared_ptr<IrElemAllocator> allocator);
};





#endif // !__REPLACE_BY_MOV_H__
