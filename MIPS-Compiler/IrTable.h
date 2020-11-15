
#ifndef __IR_TABLE_H__
#define __IR_TABLE_H__

#include "global_control.h"
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::pair;
using std::shared_ptr;
using std::make_shared;
using std::make_pair;
using std::unordered_map;
using std::unordered_set;
using std::to_string;


enum class IrHead
{
	label,		// 	<label>

	gvar,		//  <var> 
	arr,		// 	<arr> <type> <imm>
	init,		//  <imm>

	func,		//  <type>
	param,		//  <var>


	add,		// 	<var> <val> <val>
	sub,		// 	<var> <val> <val>
	mult,		// 	<var> <val> <val>
	div,		// 	<var> <val> <val>
	_and,		//  <var> <val> <val>
	_or,		//  <var> <val> <val>
	_nor,		//  <var> <val> <val>
	_xor,		//  <var> <val> <val>
	sl,			//  <var> <val> <val>
	sr,			//  <var> <val> <val>
	less,		// 	<var> <val> <val>

	lw,			// 	<var> <var> <arr>
	lb,			// 	<var> <var> <arr>
	sw,			// 	<var> <var> <arr>
	sb,			// 	<var> <var> <arr>

	beq,		//  <val> <val> <label>
	bne,		//  <val> <val> <label>
	_goto,		//  <label>

	push,		//  <val>
	call,		//  <label>
	ret,		//  null

	scanf,		//  <var> <type>
	printf,		//  [<string>] [<var> <type>]		
};

using irelem_t = uint32_t;


struct IrType
{
	static const irelem_t NIL = 0xE000'0000;
	static const irelem_t _int = 0xE000'0001;
	static const irelem_t _char = 0xE000'0002;
	static const irelem_t _void = 0xE000'0003;
	static bool is_val(irelem_t v) { return (v >> 31) == 0b1; }
	static bool is_var(irelem_t v) { return (v >> 29) == 0b101; }
	static bool is_tmp(irelem_t v) { return (v >> 28) == 0b1010; }
	static bool is_named(irelem_t v) { return (v >> 28) == 0b1011; }
	static bool is_cst(irelem_t v) { return (v >> 29) == 0b100; }
	static bool is_imm(irelem_t v) { return (v >> 28) == 0b1000; }
	static bool is_arr(irelem_t v) { return (v >> 28) == 0b1001; }
	static bool is_pure_arr(irelem_t v) { return (v >> 27) == 0b10010; }
	static bool is_fix_idx(irelem_t v) { return (v >> 27) == 0b10011; }
	static bool is_string(irelem_t v) { return (v >> 29) == 0b110; }
	static bool is_special(irelem_t v) { return (v >> 29) == 0b111; }
	static bool is_label(irelem_t v) { return (v >> 31) == 0b0; }
	static bool is_func(irelem_t v) { return (v >> 28) == 0b0001; }
	static bool is_if(irelem_t v) { return (v >> 28) == 0b0010; }
	static bool is_switch(irelem_t v) { return (v >> 28) == 0b0011; }
	static bool is_for(irelem_t v) { return (v >> 28) == 0b0100; }
	static bool is_while(irelem_t v) { return (v >> 28) == 0b0101; }
	static bool is_beg(irelem_t v) { return (v & 0x8C00'0000) == 0x0000'0000; }
	static bool is_start(irelem_t v) { return (v & 0x8C00'0000) == 0x0400'0000; }
	static bool is_mid(irelem_t v) { return (v & 0x8C00'0000) == 0x0800'0000; }
	static bool is_end(irelem_t v) { return (v & 0x8C00'0000) == 0x0C00'0000; }
	static uint32_t  get_ord(irelem_t v) { return  v & 0x03FF'FFFF; }
};

class LabelAllocator
{
private:
	unsigned if_count = 0;
	unsigned switch_count = 0;
	unsigned for_count = 0;
	unsigned while_count = 0;
	irelem_t stored;
	vector<string> funcs;
public:
	string label_to_string(irelem_t label) const;
	string func_name(irelem_t label) const;
	LabelAllocator& alloc_func(const string& name);
	LabelAllocator& alloc_func(shared_ptr<const string> name) { return alloc_func(*name); }
	LabelAllocator& alloc_if();
	LabelAllocator& alloc_switch();
	LabelAllocator& alloc_for();
	LabelAllocator& alloc_while();
	irelem_t beg() const;
	irelem_t start() const;
	irelem_t mid() const;
	irelem_t end() const;
};

class VarAllocator
{
private:
	vector<shared_ptr<const string>> tmps;
	vector<pair<shared_ptr<const string>, shared_ptr<const string>>> nameds;
	shared_ptr<const string> current_func;
	irelem_t _sp;
	irelem_t _gp;
	irelem_t _ret;
	irelem_t _zero;
	unordered_set<irelem_t> _reserved_var;
public:
	static const shared_ptr<const string> __global;
	VarAllocator();
	VarAllocator& set_function(shared_ptr<const string> func_name)
	{
		current_func = func_name;
		return *this;
	}
	irelem_t alloc() { return alloc_tmp(); }
	irelem_t alloc(shared_ptr<const string> name) { return alloc_named(name); }
	irelem_t alloc_tmp();
	irelem_t alloc_named(shared_ptr<const string> name);
	irelem_t sp() const noexcept { return _sp; }
	irelem_t ret() const noexcept { return _ret; }
	irelem_t zero() const noexcept { return _zero; }
	irelem_t gp() const noexcept { return _gp; }
	bool is_reserved_var(irelem_t var) const;
	const unordered_set<irelem_t>& reversed_var() const noexcept { return _reserved_var; }
	bool is_global_var(irelem_t named) const;
	bool is_local_var(irelem_t var) const;
	string var_to_string(irelem_t var) const;
};

class CstAllocator
{
private:
	vector<int> imms;
	unordered_map<int, irelem_t> imm_cache;
	vector<pair<shared_ptr<const string>, shared_ptr<const string>>> arrs;
	vector<pair<irelem_t, irelem_t>> incalculate_cst;
	unordered_map<irelem_t, int> arr_value;
	shared_ptr<const string> current_func;

public:
	CstAllocator() : imms(), imm_cache(), arrs(), incalculate_cst(), arr_value(), current_func(VarAllocator::__global) { }
	CstAllocator& set_function(shared_ptr<const string> func_name)
	{
		current_func = func_name;
		return *this;
	}
	irelem_t alloc_arr(shared_ptr<const string> arr);
	irelem_t alloc_imm(int imm);
	irelem_t cst_add(irelem_t cst_1, irelem_t cst_2);
	irelem_t cst_add(irelem_t cst_1, int imm) { return cst_add(cst_1, alloc_imm(imm)); }
	CstAllocator& set_arr_value(irelem_t arr, int value)
	{
		arr_value.insert(make_pair(arr, value));
		return *this;
	}
	int imm_to_value(irelem_t imm) const;
	int cst_to_value(irelem_t cst) const;
};

class StringAllocator
{
private:
	unordered_map<string, irelem_t> map;
public:
	irelem_t alloc_string(shared_ptr<const string> str);
	irelem_t alloc_string(const string& str);
	const unordered_map<string, irelem_t>& get_map() const { return map; }
};

struct IrElemAllocator
	: LabelAllocator, VarAllocator, CstAllocator, StringAllocator
{
	IrElemAllocator& set_function(shared_ptr<const string> func_name)
	{
		VarAllocator::set_function(func_name);
		CstAllocator::set_function(func_name);
		return *this;
	}
};

struct Ir
{
	IrHead head;
	irelem_t elem[3] = { IrType::NIL, IrType::NIL, IrType::NIL };
};


struct IrTable
{
	IrTable(const vector<Ir>& table) : table(table) { }

	const Ir& operator[](size_t index) { return table.at(index); }
	const Ir& at(size_t index) const { return table.at(index); }
	vector<Ir>::const_iterator begin() const { return table.cbegin(); }
	vector<Ir>::const_iterator end() const { return table.cend(); }
	size_t size() const { return table.size(); }
private:
	const vector<Ir> table;
};

struct IrTableBuilder : vector<Ir>
{
	shared_ptr<IrTable> build();
	void push_back(const Ir& code)
	{
		vector<Ir>::push_back(code);
	}

	void push_back(IrHead head, irelem_t elem1 = IrType::NIL, irelem_t elem2 = IrType::NIL, irelem_t elem3 = IrType::NIL)
	{
		vector<Ir>::push_back(Ir{ head, {elem1, elem2, elem3} });
	}

	void push_back_all(std::initializer_list<Ir> list)
	{
		for (const auto& ir : list)
		{
			vector<Ir>::push_back(ir);
		}
	}

	void push_back_all(const IrTable& ir_table)
	{
		for (const auto& ir : ir_table)
		{
			push_back(ir);
		}
	}
};

struct IrFactory
{
	IrFactory(shared_ptr<IrElemAllocator> allocator_ptr) : allocator_ptr(allocator_ptr) { }
	shared_ptr<IrElemAllocator> allocator_ptr;
	IrElemAllocator& allocator()  const { return *allocator_ptr; }

	static constexpr irelem_t nil() { return IrType::NIL; }

	Ir label(irelem_t label) const
	{
		return Ir{ IrHead::label, {label, nil(), nil()} };
	}

	Ir gvar(irelem_t var) const
	{
		return Ir{ IrHead::gvar, {var, nil(), nil()} };
	}

	Ir arr(irelem_t arr, irelem_t type, int space) const
	{
		return Ir{ IrHead::arr, {arr, type, allocator().alloc_imm(space)} };
	}

	Ir init(int imm) const
	{
		return Ir{ IrHead::init, {allocator().alloc_imm(imm), nil(), nil()} };
	}

	Ir func(irelem_t type) const
	{
		return Ir{ IrHead::func, {type, nil(), nil()} };
	}

	Ir param(irelem_t var) const
	{
		return Ir{ IrHead::func, {var, nil(), nil()} };
	}

	Ir add(irelem_t var, irelem_t val1, irelem_t val2) const
	{
		return Ir{ IrHead::add, {var, val1, val2} };
	}

	Ir sub(irelem_t var, irelem_t val1, irelem_t val2) const
	{
		return Ir{ IrHead::sub, {var, val1, val2} };
	}

	Ir mult(irelem_t var, irelem_t val1, irelem_t val2) const
	{
		return Ir{ IrHead::mult, {var, val1, val2} };
	}

	Ir div(irelem_t var, irelem_t val1, irelem_t val2) const
	{
		return Ir{ IrHead::div, {var, val1, val2} };
	}

	Ir _and(irelem_t var, irelem_t val1, irelem_t val2) const
	{
		return Ir{ IrHead::_and, {var, val1, val2} };
	}

	Ir _or(irelem_t var, irelem_t val1, irelem_t val2) const
	{
		return Ir{ IrHead::_or, {var, val1, val2} };
	}

	Ir _nor(irelem_t var, irelem_t val1, irelem_t val2) const
	{
		return Ir{ IrHead::_nor, {var, val1, val2} };
	}

	Ir _xor(irelem_t var, irelem_t val1, irelem_t val2) const
	{
		return Ir{ IrHead::_xor, {var, val1, val2} };
	}

	Ir sl(irelem_t var, irelem_t val1, irelem_t val2) const
	{
		return Ir{ IrHead::sl, {var, val1, val2} };
	}

	Ir sr(irelem_t var, irelem_t val1, irelem_t val2) const
	{
		return Ir{ IrHead::sr, {var, val1, val2} };
	}

	Ir less(irelem_t var, irelem_t val1, irelem_t val2) const
	{
		return Ir{ IrHead::less, {var, val1, val2} };
	}

	Ir lw(irelem_t var, irelem_t base, irelem_t off) const
	{
		return Ir{ IrHead::lw, {var, base, off} };
	}

	Ir sw(irelem_t var, irelem_t base, irelem_t off) const
	{
		return Ir{ IrHead::sw, {var, base, off} };
	}

	Ir lb(irelem_t var, irelem_t base, irelem_t off) const
	{
		return Ir{ IrHead::lb, {var, base, off} };
	}

	Ir sb(irelem_t var, irelem_t base, irelem_t off) const
	{
		return Ir{ IrHead::sb, {var, base, off} };
	}

	Ir beq(irelem_t val1, irelem_t val2, irelem_t label) const
	{
		return Ir{ IrHead::beq, {val1, val2, label} };
	}

	Ir bne(irelem_t val1, irelem_t val2, irelem_t label) const
	{
		return Ir{ IrHead::bne, {val1, val2, label} };
	}

	Ir _goto(irelem_t label) const
	{
		return Ir{ IrHead::_goto, {label, nil(), nil()} };
	}

	Ir push(irelem_t val) const
	{
		return Ir{ IrHead::push, {val, nil(), nil()} };
	}

	Ir call(irelem_t label) const
	{
		return Ir{ IrHead::call, {label, nil(), nil()} };
	}

	Ir ret() const
	{
		return Ir{ IrHead::ret, {nil(), nil(), nil()} };
	}

	Ir scanf(irelem_t var, irelem_t type) const
	{
		return Ir{ IrHead::scanf, {var, type, nil()} };
	}

	Ir printf(shared_ptr<const string> str) const
	{
		return Ir{ IrHead::printf, {allocator().alloc_string(str), nil(), nil()} };
	}

	Ir printf(const string& str) const
	{
		return Ir{ IrHead::printf, {allocator().alloc_string(str), nil(), nil()} };
	}

	Ir printf(irelem_t var, irelem_t type) const
	{
		return Ir{ IrHead::printf, {nil(), var, type} };
	}

	Ir printf(shared_ptr<const string> str, irelem_t var, irelem_t type) const
	{
		return Ir{ IrHead::printf, {allocator().alloc_string(str), var, type} };
	}

	Ir printf(const string& str, irelem_t var, irelem_t type) const
	{
		return Ir{ IrHead::printf, {allocator().alloc_string(str), var, type} };
	}
};



#endif // !__IR_TABLE_H__

