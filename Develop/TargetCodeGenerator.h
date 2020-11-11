#pragma once

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
	shared_ptr<const IrTable> ir_table_ptr;
	string func_name = "__global";
	ostringstream buffer;	// Ŀ������buffer
	unordered_map<irelem_t, unsigned> global_var_offset_table;		// ȫ�ֱ�����label
	unordered_map<irelem_t, ArrayInfo> global_arr_info_table;
	unordered_map<irelem_t, unsigned> func_var_offset_table;
	unordered_map<irelem_t, ArrayInfo> func_arr_info_table;


	/// <summary>
	/// ����func_beg_index, func_mid_index, func_end_index, func_name.
	/// </summary>
	void next_function_info();

	/// <summary>
	/// Ϊÿ��ȫ�ֱ�������ռ�, ����label, �����ֵ;
	/// ��дglobal_label_table.
	/// </summary>
	/// <returns></returns>
	void init_total();
	 
	/// <summary>
	/// ɨ�赱ǰfunc_beg_index, func_mid_index, func_end_index��ע�ĺ���;
	/// ������дfunc_var_offset_table, arr_info_table;
	/// ��������ջ��С��д��stack_size.
	/// </summary>
	void init_func();

	/// <summary>
	/// ��ɽ��뺯����ǰ��
	/// </summary>
	/// <param name="stack_size"></param>
	void beg_func();

	/// <summary>
	/// ��ɷ��ع���
	/// </summary>
	/// <param name="stack_size"></param>
	void end_func();

	/// <summary>
	/// ������������д��, ����ջ�����.
	/// </summary>
	/// <param name="os"></param>
	void fresh_buffer(ostream& os);
public:
	virtual ~SimpleGenerator() = default;
	SimpleGenerator(shared_ptr<const IrElemAllocator> allocator, shared_ptr<const IrTable> ir)
		: allocator_ptr(allocator), ir_table_ptr(ir) { }

	/// <summary>
	/// ��allocatorָ���µ�irת��Ϊstring�����뵽os��.
	/// </summary>
	/// <param name="allocator"></param>
	/// <param name="ir"></param>
	/// <param name="os"></param>
	virtual void translate(ostream& os);
};


#endif // !__TARGET_CODE_GENERATOR_H__
