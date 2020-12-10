#include "ReplaceByMov.h"

void ReplaceByMov::check_bne(const IrTable& origin)
{
    ASSERT(1, origin.at(current_index).head == IrHead::bne);
    do
    {
        irelem_t cmp;
        if (origin.at(current_index).elem[0] == allocator_ptr->zero())
        {
            cmp = origin.at(current_index).elem[1];
        }
        else if (origin.at(current_index).elem[1] == allocator_ptr->zero())
        {
            cmp = origin.at(current_index).elem[0];
        }
        else
        {
            break;
        }
        if (origin.at(current_index + 1).head != IrHead::label)
        {
            break;
        }
        unique_ptr<IrDetectors::FromToInfo> cpyinfo = IrDetectors::detect_copy(origin.at(current_index + 2), *allocator_ptr);
        if (cpyinfo == nullptr)
        {
            break;
        }
        if (origin.at(current_index + 3).head != IrHead::label)
        {
            break;
        }
        if (origin.at(current_index + 4).head != IrHead::label)
        {
            break;
        }
        irelem_t beg = origin.at(current_index + 1).elem[0];
        irelem_t mid = origin.at(current_index + 3).elem[0];
        irelem_t end = origin.at(current_index + 4).elem[0];
        if (origin.at(current_index).elem[2] != mid)
        {
            break;
        }
        if (!IrType::is_if(beg))
        {
            break;
        }
        if (!IrType::is_if(mid))
        {
            break;
        }
        if (!IrType::is_if(end))
        {
            break;
        }
        if (!IrType::is_beg(beg))
        {
            break;
        }
        if (!IrType::is_mid(mid))
        {
            break;
        }
        if (!IrType::is_end(end))
        {
            break;
        }
        if (IrType::get_ord(beg) != IrType::get_ord(mid) || IrType::get_ord(mid) != IrType::get_ord(end))
        {
            break;
        }
        builder.push_back(IrFactory(allocator_ptr).movz(cpyinfo->to, cpyinfo->from, cmp));
        current_index += 4;
        return;
    } while (false);
    builder.push_back(origin.at(current_index));
}

shared_ptr<IrTable> ReplaceByMov::parse(const IrTable& origin, shared_ptr<IrElemAllocator> allocator)
{
    allocator_ptr = allocator;
    builder.clear();
    for (current_index = 0; current_index != origin.size(); ++current_index)
    {
        const auto& code = origin.at(current_index);
        if (code.head == IrHead::bne)
        {
            check_bne(origin);
            continue;
        }
        builder.push_back(code);
    }
    return builder.build();
}
