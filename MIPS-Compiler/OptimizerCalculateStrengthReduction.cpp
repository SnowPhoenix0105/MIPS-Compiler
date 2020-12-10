#include "OptimizerCalculateStrengthReduction.h"
using std::swap;


int OptimizerCalculateStrengthReduction::pow2(unsigned num)
{
    unsigned mul = 1;
    for (unsigned i = 0; i != 32; ++i)
    {
        if (mul == num)
        {
            return i;
        }
        mul *= 2;
    }
    return -1;
}

void OptimizerCalculateStrengthReduction::trans_mult(const Ir& origin)
{
    ASSERT(4, origin.head == IrHead::mult);
    Ir rlt = origin;
    bool cst1 = IrType::is_imm(origin.elem[1]);
    bool cst2 = IrType::is_imm(origin.elem[2]);
    if (cst1 && cst2)
    {
        rlt.head = IrHead::add;
        rlt.elem[1] = allocator_ptr->zero();
        int mult_rlt = allocator_ptr->imm_to_value(origin.elem[1]) * allocator_ptr->imm_to_value(origin.elem[2]);
        rlt.elem[2] = allocator_ptr->alloc_imm(mult_rlt);
        buffer.push_back(rlt);
        return;
    }
    if (cst1 || cst2)
    {
        irelem_t var;
        irelem_t imm;
        if (cst1)
        {
            imm = rlt.elem[1];
            var = rlt.elem[2];
        }
        else
        {
            imm = rlt.elem[2];
            var = rlt.elem[1];
        }
        int imm_value = allocator_ptr->imm_to_value(imm);
        if (imm_value == 1)
        {
            rlt.head = IrHead::add;
            rlt.elem[1] = allocator_ptr->zero();
            rlt.elem[2] = var;
            buffer.push_back(rlt);
            return;
        }
        if (imm_value == -1)
        {
            rlt.head = IrHead::sub;
            rlt.elem[1] = allocator_ptr->zero();
            rlt.elem[2] = var;
            buffer.push_back(rlt);
            return;
        }
        bool imm_gz = imm_value > 0;
        if (!imm_gz)
        {
            imm_value = -imm_value;
        }
        int pow = pow2(imm_value);
        if (pow < 0)
        {
            rlt.elem[1] = var;
            rlt.elem[2] = imm;
            buffer.push_back(rlt);
            return;
        }
        ASSERT(4, pow != 0);
        rlt.head = IrHead::sl;
        rlt.elem[1] = var;
        rlt.elem[2] = allocator_ptr->alloc_imm(pow);
        buffer.push_back(rlt);
        if (!imm_gz)
        {
            rlt.head = IrHead::sub;
            rlt.elem[1] = allocator_ptr->zero();
            rlt.elem[2] = rlt.elem[0];
            buffer.push_back(rlt);
        }
        return;
    }
    buffer.push_back(rlt);
}

void OptimizerCalculateStrengthReduction::trans_div(const Ir& origin)
{
    buffer.push_back(origin);
}

shared_ptr<IrTable> OptimizerCalculateStrengthReduction::parse(const IrTable& origin, shared_ptr<IrElemAllocator> allocator)
{
    allocator_ptr = allocator;
    buffer.clear();
    for (const auto& code : origin)
    {
        if (code.head == IrHead::mult)
        {
            trans_mult(code);
        }
        else if (code.head == IrHead::div)
        {
            trans_div(code);
        }
        else
        {
            buffer.push_back(code);
        }
    }
    return buffer.build();
}
