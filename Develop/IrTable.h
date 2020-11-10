#pragma once

#ifndef __IR_TABLE_H__
#define __IR_TABLE_H__

#include "global_control.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::pair;
using std::shared_ptr;
using std::make_shared;
using std::make_pair;
using std::unordered_map;
using std::to_string;


enum class IrHead
{
	label,		// 	<label>

	gvar,		//  <var> [<imm>]
	arr,		// 	<arr> <type> <imm>
	init,		//  <imm>
	func,		//  <type>
	param,		//  <var>


	add,		// 	<var> <val> <val>
	sub,		// 	<var> <val> <val>
	mult,		// 	<var> <val> <val>
	div,		// 	<var> <val> <val>
	sl,			//  <var> <val> <val>
	sr,			//  <var> <val> <val>
	less,		// 	<var> <val> <val>

	save,		// 	<var> <var> <arr>
	load,		// 	<var> <var> <arr>

	beq,		//  <val> <val> <label>
	bne,		//  <val> <val> <label>
	_goto,		//  <label>

	push,		//  <val>
	call,		//  <label>

	scanf,		//  <var>
	printf,		//  [<string>] [<var>]
};

using irelem_t = uint32_t;


struct IrType
{
	static const irelem_t _int	= 0xE000'0000;
	static const irelem_t _char = 0xE000'0001;
	static const irelem_t _void = 0xE000'0002;
	static bool is_val		(irelem_t v) { return (v >> 31) == 0b1; }
	static bool is_var		(irelem_t v) { return (v >> 29) == 0b101; }
	static bool is_tmp		(irelem_t v) { return (v >> 28) == 0b1010; }
	static bool is_named	(irelem_t v) { return (v >> 28) == 0b1011; }
	static bool is_cst		(irelem_t v) { return (v >> 29) == 0b100; }
	static bool is_imm		(irelem_t v) { return (v >> 28) == 0b1000; }
	static bool is_arr		(irelem_t v) { return (v >> 28) == 0b1001; }
	static bool is_pure_arr	(irelem_t v) { return (v >> 27) == 0b10010; }
	static bool is_fix_idx	(irelem_t v) { return (v >> 27) == 0b10011; }
	static bool is_string	(irelem_t v) { return (v >> 29) == 0b110; }
	static bool is_special	(irelem_t v) { return (v >> 29) == 0b111; }
	static bool is_label	(irelem_t v) { return (v >> 31) == 0b0; }
	static bool is_func		(irelem_t v) { return (v >> 28) == 0b0001; }
	static bool is_if		(irelem_t v) { return (v >> 28) == 0b0010; }
	static bool is_switch	(irelem_t v) { return (v >> 28) == 0b0011; }
	static bool is_for		(irelem_t v) { return (v >> 28) == 0b0100; }
	static bool is_while	(irelem_t v) { return (v >> 28) == 0b0101; }
	static bool is_beg		(irelem_t v) { return (v & 0x8C00'0000) == 0x0000'0000; }
	static bool is_start	(irelem_t v) { return (v & 0x8C00'0000) == 0x0400'0000; }
	static bool is_mid		(irelem_t v) { return (v & 0x8C00'0000) == 0x0800'0000; }
	static bool is_end		(irelem_t v) { return (v & 0x8C00'0000) == 0x0C00'0000; }
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
	static const shared_ptr<const string> __global;
	vector<const shared_ptr<const string>> tmps;
	vector<const pair<shared_ptr<const string>, shared_ptr<const string>>> nameds;
	shared_ptr<const string> current_func;
	irelem_t _sp;
	irelem_t _ret;
	irelem_t _zero;
public:
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
	string var_to_string(irelem_t var) const;
};

class CstAllocator
{
private:
	vector<const int> imms;
	unordered_map<int, irelem_t> imm_cache;
	vector<const pair<shared_ptr<const string>, shared_ptr<const string>>> arrs;
	vector<const pair<irelem_t, irelem_t>> incalculate_cst;
	unordered_map<irelem_t, const int> arr_value;

public:
	irelem_t alloc_arr(shared_ptr<const string> func, shared_ptr<const string> arr);
	irelem_t alloc_imm(int imm);
	irelem_t cst_add(irelem_t cst_1, irelem_t cst_2);
	CstAllocator& set_arr_value(irelem_t arr, int value)
	{
		arr_value.insert(make_pair(arr, value));
	}
	int imm_value(irelem_t imm) const;
	int value_of(irelem_t cst) const;
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
	
};

struct Ir
{
	IrHead head;
	irelem_t elem[3];
};

struct IrTable;

struct IrTableBuilder : vector<Ir>
{
	IrTable build();
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



#endif // !__IR_TABLE_H__

