
#ifndef __TARGET_CODE_GENERATOR_H__
#define __TARGET_CODE_GENERATOR_H__

#include "global_control.h"
#include "IrTable.h"
#include <iostream>
#include <sstream>

using std::ostream;
using std::ostringstream;
using std::endl;

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
	size_t func_beg_index = 0;
	size_t func_mid_index = 0;
	size_t func_end_index = 0;
	size_t stack_size = 0;
	shared_ptr<const IrElemAllocator> allocator_ptr;
	shared_ptr<const IrTable> ir_table;
	string func_name = "__global";
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
	void init_total();
	 
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
		: allocator_ptr(allocator), ir_table(ir) { }

	/// <summary>
	/// 将allocator指导下的ir转换为string并输入到os中.
	/// </summary>
	/// <param name="allocator"></param>
	/// <param name="ir"></param>
	/// <param name="os"></param>
	virtual void translate(const IrTable& ir, ostream& os);
};


#endif // !__TARGET_CODE_GENERATOR_H__