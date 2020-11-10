#pragma once

#ifndef __TARGET_CODE_GENERATOR_H__
#define __TARGET_CODE_GENERATOR_H__

#include "global_control.h"
#include "IrTable.h"
#include <iostream>
#include <sstream>

using std::ostream;
using std::ostringstream;

struct ITargetCodeGenerator
{
	virtual ~ITargetCodeGenerator() = 0;
	virtual void translate(const IrTable& ir, const ostream& os) = 0;
};

struct ArrayInfo
{
	unsigned offset;
	bool is_int;
};

class SimpleGenerator : public ITargetCodeGenerator
{
private:
	size_t func_beg_index;
	size_t func_mid_index;
	size_t func_end_index;
	string func_name;
	ostringstream buffer;	// 目标代码的buffer
	unordered_map<irelem_t, const string> global_label_table;		// 全局变量的label
	unordered_map<irelem_t, const unsigned> func_var_offset_table;
	unordered_map<irelem_t, const ArrayInfo> arr_info_table;


	/// <summary>
	/// 更新func_beg_index, func_mid_index, func_end_index, func_name.
	/// </summary>
	void next_function_info();

	/// <summary>
	/// 为每个全局变量分配空间, 分配label, 填入初值;
	/// 填写global_label_table.
	/// </summary>
	/// <returns></returns>
	unsigned init_total();
	 
	/// <summary>
	/// 扫描当前func_beg_index, func_mid_index, func_end_index标注的函数;
	/// 重新填写func_var_offset_table, arr_info_table;
	/// 返回需求的运行栈大小
	/// </summary>
	unsigned init_func();

	/// <summary>
	/// 完成进入函数体前的
	/// </summary>
	/// <param name="stack_size"></param>
	void beg_func(unsigned stack_size);

	/// <summary>
	/// 完成返回工作
	/// </summary>
	/// <param name="stack_size"></param>
	void end_func(unsigned stack_size);
public:
	virtual ~SimpleGenerator() = default;

	/// <summary>
	/// 将allocator指导下的ir转换为string并输入到os中.
	/// </summary>
	/// <param name="allocator"></param>
	/// <param name="ir"></param>
	/// <param name="os"></param>
	virtual void translate(const IrElemAllocator& allocator, const IrTable& ir, const ostream& os);
};


#endif // !__TARGET_CODE_GENERATOR_H__
