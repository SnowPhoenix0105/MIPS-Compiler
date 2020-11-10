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
	ostringstream buffer;	// Ŀ������buffer
	unordered_map<irelem_t, const string> global_label_table;		// ȫ�ֱ�����label
	unordered_map<irelem_t, const unsigned> func_var_offset_table;
	unordered_map<irelem_t, const ArrayInfo> arr_info_table;


	/// <summary>
	/// ����func_beg_index, func_mid_index, func_end_index, func_name.
	/// </summary>
	void next_function_info();

	/// <summary>
	/// Ϊÿ��ȫ�ֱ�������ռ�, ����label, �����ֵ;
	/// ��дglobal_label_table.
	/// </summary>
	/// <returns></returns>
	unsigned init_total();
	 
	/// <summary>
	/// ɨ�赱ǰfunc_beg_index, func_mid_index, func_end_index��ע�ĺ���;
	/// ������дfunc_var_offset_table, arr_info_table;
	/// �������������ջ��С
	/// </summary>
	unsigned init_func();

	/// <summary>
	/// ��ɽ��뺯����ǰ��
	/// </summary>
	/// <param name="stack_size"></param>
	void beg_func(unsigned stack_size);

	/// <summary>
	/// ��ɷ��ع���
	/// </summary>
	/// <param name="stack_size"></param>
	void end_func(unsigned stack_size);
public:
	virtual ~SimpleGenerator() = default;

	/// <summary>
	/// ��allocatorָ���µ�irת��Ϊstring�����뵽os��.
	/// </summary>
	/// <param name="allocator"></param>
	/// <param name="ir"></param>
	/// <param name="os"></param>
	virtual void translate(const IrElemAllocator& allocator, const IrTable& ir, const ostream& os);
};


#endif // !__TARGET_CODE_GENERATOR_H__
