#pragma once

#include <unordered_map>
#include "../Develop/SimpleTargetGenerator.h"
#include "../Develop/TargetCodeGenerator.h"


using std::pair;
using std::make_pair;

inline shared_ptr<string> to_ptr(const char* s)
{
    return make_shared<string>(s);
}

inline pair<shared_ptr<IrElemAllocator>, shared_ptr<IrTable>> get_sample1()
{
    
    shared_ptr<IrElemAllocator> allocator_ptr = make_shared<IrElemAllocator>();
    IrElemAllocator& allocator = *allocator_ptr;
    IrFactory ir(allocator_ptr);
    IrTableBuilder builder;
    irelem_t gvar1 = allocator.alloc_named(to_ptr("gvar1"));
    irelem_t gvar2 = allocator.alloc_named(to_ptr("gvar2"));
    irelem_t garr1 = allocator.alloc_arr(to_ptr("garr1"));
    irelem_t garr2 = allocator.alloc_arr(to_ptr("garr2"));
    builder.push_back_all(
        {
            ir.gvar(gvar1),
            ir.arr(garr1, IrType::_char, 5),
            ir.init('a'),
            ir.init('b'),
            ir.init('c'),
            ir.init('d'),
            ir.init('e'),
            ir.arr(garr2, IrType::_int, 4),
            ir.gvar(gvar2),
            ir.init(18231045),
        }
    );
    allocator.set_function(to_ptr("main"));
    irelem_t var1 = allocator.alloc_named(to_ptr("var1"));
    irelem_t var2 = allocator.alloc_named(to_ptr("var2"));
    irelem_t tmp1 = allocator.alloc_tmp();
    irelem_t tmp2 = allocator.alloc_tmp();
    irelem_t arr1 = allocator.alloc_arr(to_ptr("arr1"));
    irelem_t arr2 = allocator.alloc_arr(to_ptr("arr2"));
    irelem_t arr3 = allocator.alloc_arr(to_ptr("arr3"));
    builder.push_back_all(
        {
            ir.label(allocator.alloc_func("main").beg()),
            ir.arr(arr1, IrType::_char, 5),
            ir.init('f'),
            ir.init('g'),
            ir.init('h'),
            ir.init('i'),
            ir.init('j'),
            ir.arr(arr2, IrType::_int, 2),
            ir.arr(arr3, IrType::_int, 4),
            ir.init(18231045),
            ir.init(10451823),
            ir.init(188111987),
            ir.init(0xFFFFFFFF),
            ir.label(allocator.alloc_func("main").mid()),
            ir.add(tmp1, allocator.sp(), allocator.alloc_imm(1)),
            ir.lb(var1, tmp1, allocator.cst_add(arr1, allocator.alloc_imm(1))),
            ir.printf(var1, IrType::_char),
            ir.scanf(var2, IrType::_int),
            ir.sw(var2, allocator.sp(), allocator.alloc_imm(8)),
            ir.add(tmp2, allocator.sp(), allocator.alloc_imm(8)),
            ir.lw(tmp2, tmp2, allocator.alloc_imm(0)),
            ir.printf(to_ptr("your input is\t"), tmp2, IrType::_int),
            ir.label(allocator.alloc_func("main").end()),
        }
    );
    shared_ptr<IrTable> ir_table = builder.build();
    return make_pair(allocator_ptr, ir_table);
}