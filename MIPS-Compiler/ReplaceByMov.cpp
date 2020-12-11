#include "ReplaceByMov.h"


void ReplaceByMov::check_beq(const IrTable& origin)
{
    ASSERT(1, origin.at(current_index).head == IrHead::beq);
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
        irelem_t beg = origin.at(current_index + 1).elem[0];
        if (!IrType::is_if(beg) || !IrType::is_beg(beg))
        {
            break;
        }
        unique_ptr<IrDetectors::FromToInfo> cpyinfo = IrDetectors::detect_copy(origin.at(current_index + 2), *allocator_ptr);
        if (cpyinfo == nullptr)
        {
            break;
        }
        if (origin.at(current_index + 3).head == IrHead::label)
        {
            irelem_t mid = origin.at(current_index + 3).elem[0];
            if (origin.at(current_index).elem[2] != mid)
            {
                break;
            }
            if (!IrType::is_if(mid) || !IrType::is_mid(mid))
            {
                break;
            }
            if (origin.at(current_index + 4).head != IrHead::label)
            {
                break;
            }
            irelem_t end = origin.at(current_index + 4).elem[0];
            if (!IrType::is_if(end) || !IrType::is_end(end))
            {
                break;
            }
            if (IrType::get_ord(beg) != IrType::get_ord(mid) || IrType::get_ord(mid) != IrType::get_ord(end))
            {
                break;
            }
            builder.push_back(IrFactory(allocator_ptr).movn(cpyinfo->to, cpyinfo->from, cmp));
            current_index += 4;
            return;
        }
        else if (origin.at(current_index + 3).head == IrHead::_goto)
        {
            if (origin.at(current_index + 4).head != IrHead::label)
            {
                break;
            }
            irelem_t mid = origin.at(current_index + 4).elem[0];
            if (origin.at(current_index).elem[2] != mid)
            {
                break;
            }
            if (!IrType::is_if(mid) || !IrType::is_mid(mid))
            {
                break;
            }
            unique_ptr<IrDetectors::FromToInfo> cpyinfo2 = IrDetectors::detect_copy(origin.at(current_index + 5), *allocator_ptr);
            if (cpyinfo2 == nullptr)
            {
                break;
            }
            if (cpyinfo->to != cpyinfo2->to)
            {
                break;
            }
            if (origin.at(current_index + 6).head != IrHead::label)
            {
                break;
            }
            irelem_t end = origin.at(current_index + 6).elem[0];
            if (origin.at(current_index + 3).elem[0] != end)
            {
                break;
            }
            if (!IrType::is_if(end) || !IrType::is_end(end))
            {
                break;
            }
            if (IrType::get_ord(beg) != IrType::get_ord(mid) || IrType::get_ord(mid) != IrType::get_ord(end))
            {
                break;
            }
            builder.push_back(IrFactory(allocator_ptr).add(cpyinfo->to, allocator_ptr->zero(), cpyinfo->from));
            builder.push_back(IrFactory(allocator_ptr).movz(cpyinfo2->to, cpyinfo2->from, cmp));
            current_index += 6;
            return;
        }
    } while (false);
    builder.push_back(origin.at(current_index));
}

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
        irelem_t beg = origin.at(current_index + 1).elem[0];
        if (!IrType::is_if(beg) || !IrType::is_beg(beg))
        {
            break;
        }
        unique_ptr<IrDetectors::FromToInfo> cpyinfo = IrDetectors::detect_copy(origin.at(current_index + 2), *allocator_ptr);
        if (cpyinfo == nullptr)
        {
            break;
        }
        if (origin.at(current_index + 3).head == IrHead::label)
        {
            irelem_t mid = origin.at(current_index + 3).elem[0];
            if (origin.at(current_index).elem[2] != mid)
            {
                break;
            }
            if (!IrType::is_if(mid) || !IrType::is_mid(mid))
            {
                break;
            }
            if (origin.at(current_index + 4).head != IrHead::label)
            {
                break;
            }
            irelem_t end = origin.at(current_index + 4).elem[0];
            if (!IrType::is_if(end) || !IrType::is_end(end))
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
        }
        else if (origin.at(current_index + 3).head == IrHead::_goto)
        {
            if (origin.at(current_index + 4).head != IrHead::label)
            {
                break;
            }
            irelem_t mid = origin.at(current_index + 4).elem[0];
            if (origin.at(current_index).elem[2] != mid)
            {
                break;
            }
            if (!IrType::is_if(mid) || !IrType::is_mid(mid))
            {
                break;
            }
            unique_ptr<IrDetectors::FromToInfo> cpyinfo2 = IrDetectors::detect_copy(origin.at(current_index + 5), *allocator_ptr);
            if (cpyinfo2 == nullptr)
            {
                break;
            }
            if (cpyinfo->to != cpyinfo2->to)
            {
                break;
            }
            if (origin.at(current_index + 6).head != IrHead::label)
            {
                break;
            }
            irelem_t end = origin.at(current_index + 6).elem[0];
            if (origin.at(current_index + 3).elem[0] != end)
            {
                break;
            }
            if (!IrType::is_if(end) || !IrType::is_end(end))
            {
                break;
            }
            if (IrType::get_ord(beg) != IrType::get_ord(mid) || IrType::get_ord(mid) != IrType::get_ord(end))
            {
                break;
            }
            builder.push_back(IrFactory(allocator_ptr).add(cpyinfo->to, allocator_ptr->zero(), cpyinfo->from));
            builder.push_back(IrFactory(allocator_ptr).movn(cpyinfo2->to, cpyinfo2->from, cmp));
            current_index += 6;
            return;
        }
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
        if (code.head == IrHead::beq)
        {
            check_beq(origin);
            continue;
        }
        builder.push_back(code);
    }
    //builder.clear();
    //builder.push_back_all(origin);
    return builder.build();
}
