#pragma once

#ifndef __IR_TABLE_H__
#define __IR_TABLE_H__

#include "global_control.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

using std::string;
using std::vector;
using std::pair;
using std::shared_ptr;
using std::make_shared;
using std::make_pair;

enum class IrHead
{
	label,		// 放置标签	<label>
	func,		// 定义函数返回值 <type>
	param,		// 定义函数的形参 <var>
	arr,		// 数组声明	<arr> <type> <imm>
	add,		// 加法	<var> <val> <val>
	sub,		// 减法	<var> <val> <val>
	mult,		// 乘法	<var> <val> <val>
	div,		// 除法	<var> <val> <val>
	sl,			// 左位移 <var> <val> <val>
	sr,			// 右位移 <var> <val> <val>
	less,		// 小于	<var> <val> <val>
	save,		// 保存数组元素	<var> <var> <arr>
	load,		// 加载数组元素	<var> <var> <arr>
	beq,		// 相等转移 <val> <val> <label>
	bne,		// 不等转移 <val> <val> <label>
	_goto,		// 无条件转移 <label>
	push,		// 函数压参 <val>
	call,		// 函数调用 <label>
	scanf,		// 读操作 <string> <var>
	printf,		// 写操作 <var>
};

using irelem_t = uint32_t;

struct IrType
{
	static bool is_val		(irelem_t v) { return (v >> 31) == 0b1		; }
	static bool is_var		(irelem_t v) { return (v >> 29) == 0b101	; }
	static bool is_tmp		(irelem_t v) { return (v >> 28) == 0b1010; }
	static bool is_named	(irelem_t v) { return (v >> 28) == 0b1011; }
	static bool is_cst		(irelem_t v) { return (v >> 29) == 0b100	; }
	static bool is_imm		(irelem_t v) { return (v >> 28) == 0b1000	; }
	static bool is_arr		(irelem_t v) { return (v >> 28) == 0b1001	; }
	static bool is_pure_arr	(irelem_t v) { return (v >> 27) == 0b10010	; }
	static bool is_fix_idx	(irelem_t v) { return (v >> 27) == 0b10011	; }
	static bool is_special	(irelem_t v) { return (v >> 30) == 0b11		; }
	static bool is_label	(irelem_t v) { return (v >> 31) == 0b0		; }
	static bool is_func		(irelem_t v) { return (v >> 28) == 0b0001	; }
	static bool is_if		(irelem_t v) { return (v >> 28) == 0b0010	; }
	static bool is_switch	(irelem_t v) { return (v >> 28) == 0b0011	; }
	static bool is_for		(irelem_t v) { return (v >> 28) == 0b0100	; }
	static bool is_while	(irelem_t v) { return (v >> 28) == 0b0101	; }
	static bool is_beg		(irelem_t v) { return (v & 0x8C00'0000) == 0; }
	static bool is_start	(irelem_t v) { return (v & 0x8C00'0000) == 1; }
	static bool is_mid		(irelem_t v) { return (v & 0x8C00'0000) == 2; }
	static bool is_end		(irelem_t v) { return (v & 0x8C00'0000) == 3; }
	static uint32_t  get_ord(irelem_t v) { return  v & 0x0300'0000; }
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
	vector<shared_ptr<const string>> tmps;
	vector<pair<shared_ptr<const string>, shared_ptr<const string>>> nameds;
	shared_ptr<const string> current_func = make_shared<const string>("__global");
public:
	VarAllocator& set_function(shared_ptr<const string> func_name)
	{
		current_func = func_name;
		return *this;
	}
	irelem_t alloc() { return alloc_tmp(); }
	irelem_t alloc(shared_ptr<const string> name) { return alloc_named(name); }
	irelem_t alloc_tmp();
	irelem_t alloc_named(shared_ptr<const string> name);
};



#endif // !__IR_TABLE_H__

