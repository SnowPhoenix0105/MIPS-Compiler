#pragma once

#ifndef __IR_TABLE_H__
#define __IR_TABLE_H__

#include "global_control.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

enum class IrType
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
	beq,		// 相等转移
	bne,		// 不等转移
	_goto,		// 无条件转移
	push,		// 函数压参
	call,		// 函数调用
	scanf,		// 读操作
	printf,		// 写操作
};



#endif // !__IR_TABLE_H__

