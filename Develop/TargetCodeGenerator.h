#pragma once

#ifndef __TARGET_CODE_GENERATOR_H__
#define __TARGET_CODE_GENERATOR_H__

#include "global_control.h"
#include "IrTable.h"
#include <iostream>
#include <sstream>
#include <unordered_set>

using std::ostream;
using std::ostringstream;
using std::endl;
using std::unordered_set;

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
	// 分别指向同一个函数的对应label
	size_t func_beg_index = 0;
	size_t func_mid_index = 0;
	size_t func_end_index = 0;
	// 其它当前函数信息
	size_t stack_size = 0;
	string func_name = "__global";
	// IR信息
	shared_ptr<const IrElemAllocator> allocator_ptr;
	shared_ptr<const IrTable> ir_table_ptr;
	ostringstream buffer;	// 目标代码的buffer
	// 变量偏移表
	unordered_map<irelem_t, unsigned> global_var_offset_table;	
	unordered_map<irelem_t, ArrayInfo> global_arr_info_table;
	unordered_map<irelem_t, unsigned> func_var_offset_table;
	unordered_map<irelem_t, ArrayInfo> func_arr_info_table;


	/// <summary>
	/// 更新func_beg_index, func_mid_index, func_end_index, func_name.
	/// </summary>
	void next_function_info();

	/// <summary>
	/// 为每个全局变量分配空间, 分配label, 填入初值;
	/// 填写global_var_offset_table和global_arr_info_table.
	/// </summary>
	/// <returns></returns>
	void init_global();
	 
	/// <summary>
	/// 扫描当前func_beg_index, func_mid_index, func_end_index标注的函数;
	/// 重新填写func_var_offset_table, arr_info_table;
	/// 计算运行栈大小并写入stack_size.
	/// </summary>
	void init_func();

	/// <summary>
	/// 完成进入函数体前的
	/// </summary>
	/// <param name="stack_size"></param>
	void beg_func();

	/// <summary>
	/// 完成返回工作
	/// </summary>
	/// <param name="stack_size"></param>
	void end_func();

	/// <summary>
	/// 将缓冲区内容写入, 并清空缓冲区.
	/// </summary>
	/// <param name="os"></param>
	void fresh_buffer(ostream& os);
public:
	virtual ~SimpleGenerator() = default;
	SimpleGenerator(shared_ptr<const IrElemAllocator> allocator, shared_ptr<const IrTable> ir)
		: allocator_ptr(allocator), ir_table_ptr(ir) { }

	/// <summary>
	/// 将allocator指导下的ir转换为string并输入到os中.
	/// </summary>
	/// <param name="allocator"></param>
	/// <param name="ir"></param>
	/// <param name="os"></param>
	virtual void translate(ostream& os);
};


#endif // !__TARGET_CODE_GENERATOR_H__
