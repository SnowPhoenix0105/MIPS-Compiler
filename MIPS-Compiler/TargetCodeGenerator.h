
#ifndef __TARGET_CODE_GENERATOR_H__
#define __TARGET_CODE_GENERATOR_H__

#include "global_control.h"
#include "IrTable.h"
#include <iostream>

using std::ostream;

struct ITargetCodeGenerator
{
	virtual ~ITargetCodeGenerator() = 0;
	virtual void translate(const IrTable& ir, const ostream& os) = 0;
};


class SimpleGenerator : public ITargetCodeGenerator
{
private:

public:
	virtual ~SimpleGenerator() = default;
	virtual void translate(const IrTable& ir, const ostream& os);
};


#endif // !__TARGET_CODE_GENERATOR_H__
