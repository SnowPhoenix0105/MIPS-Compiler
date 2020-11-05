#pragma once

#ifndef __IR_TABLE_H__
#define __IR_TABLE_H__

#include "global_control.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

using std::vector;
using std::string;
using std::shared_ptr;
using std::dynamic_pointer_cast;
using std::make_shared;
using std::unordered_map;

struct IStringable
{
	virtual string to_string() = 0;
};

struct IrVal : IStringable
{
	virtual ~IrVal() = 0;
};

struct IrConst : IrVal
{
	virtual ~IrConst() = 0;
	virtual int get_val() = 0;
};

struct IrInteger : IrConst
{
	virtual ~IrInteger() = default;
	virtual int get_val();	// TODO
	virtual string to_string();	// TODO
	int value;
};

enum class IrArrayElementType
{
	_char, _int
};

struct IrArray : IrConst
{
	virtual ~IrArray() = default;
	virtual int get_val();	// TODO
	virtual string to_string();	// TODO
	int offset;
	int base_index;
	size_t size;
	IrArrayElementType type;
	shared_ptr<const string> function;
	shared_ptr<const string> name;
};

enum class IrVarType
{
	tmp, var
};

struct IrVar : IrVal
{
	virtual ~IrVar() = default;
	virtual string to_string();	// TODO
	IrVarType type;
	shared_ptr<const string> function;
	shared_ptr<const string> name;
};

struct IrTableElement : IStringable
{
	virtual ~IrTableElement() = 0;
};

enum class IrLabelType
{
	_func, _for, _while, _else
};

enum class IrLabelPosition
{
	beg, mid, end
};

struct IrLabel : IrTableElement
{
	virtual ~IrLabel() = default;
	virtual string to_string();	// TODO
	IrLabelType type;
	IrLabelPosition position;
	shared_ptr<const string> name;
};

struct IrInstruction : IrTableElement
{
	virtual ~IrInstruction() = 0;
};

enum class IrQuaternionType
{
	add,		// 加法
	sub,		// 减法
	mult,		// 乘法
	div,		// 除法
	sl,		// 向左位移
	sr,		// 向右位移
	less,		// 小于
	lw,		// 加载int数组元素
	sw,		// 保存int数组元素
	lb,		// 加载char数组元素
	sb,		// 保存char数组元素
};

using val_ptr = shared_ptr<IrVal>;
using var_ptr = shared_ptr<IrVar>;
using label_ptr = shared_ptr<IrLabel>;

struct IrQuaternion : IrInstruction
{
	virtual ~IrQuaternion() = default;
	virtual string to_string();		// TODO
	IrQuaternionType type;
	var_ptr dst;
	val_ptr src1;
	val_ptr src2;
};

enum class IrBranchType
{
	beq,		// 相等时转移 
	bne,		// 不等时转移
};

struct IrBranch : IrInstruction
{
	virtual ~IrBranch() = default;
	virtual string to_string();			// TODO
	IrBranchType type;
	var_ptr expr1;
	var_ptr expr2;
	label_ptr label;
};

enum class IrJmpType
{
	_goto,			// 无条件跳转
	_call,			// 函数调用
	_ret,			// 函数返回
};

struct IrJmp : IrInstruction
{
	virtual ~IrJmp() = default;
	virtual string to_string();			// TODO
	label_ptr label;
};

struct IrParam
{
	virtual ~IrParam() = default;
	virtual string to_string();			// TODO
	val_ptr value_param;
};

class IrTable : public vector<shared_ptr<IrTableElement>>
{
public:
	using vector<shared_ptr<IrTableElement>>::vector;
};

class IrTableLinker
{
public:
	using table_ptr = shared_ptr<IrTable>;
private:
	vector<table_ptr> list;
public:
	table_ptr link_all();
	void push_back(table_ptr table)
	{
		list.push_back(table);
	}
};

#endif // !__IR_TABLE_H__

