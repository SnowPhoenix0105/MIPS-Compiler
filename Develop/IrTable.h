#pragma once

#ifndef __IR_TABLE_H__
#define __IR_TABLE_H__


enum class IrRegType
{
	var,	// 变量
	arr,	// 数组
	tmp,	// 临时变量
	cst,	// 常量
};

enum class IrLabelType
{
	_func, _for, _while, _else
};

enum class IrType
{
	_add,		// 加法
	_sub,		// 减法
	_mult,		// 乘法
	_div,		// 除法
	_less,		// 小于
	_leq,		// 小于等于
	_great,		// 大于
	_geq,		// 大于等于
	_equal,		// 等于
	_neq,		// 不等于
	_beq,		// 相等跳转
	_bne,		// 不等跳转
	_goto,		// 无条件跳转
	_param,		// 压参
	_call,		// 函数调用
	_ret		// 返回
};

struct IrLabel
{
	virtual ~IrLabel() = 0;
};

struct IrVar
{

};



class IrTable
{
};



#endif // !__IR_TABLE_H__

