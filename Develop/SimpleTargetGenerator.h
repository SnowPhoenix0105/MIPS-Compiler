#pragma once

#ifndef __SIMPLE_TARGET_GENERATOR_H__
#define __SIMPLE_TARGET_GENERATOR_H__

#include "global_control.h"
#include "IrTable.h"
#include <iostream>
#include <sstream>
#include <unordered_set>
#include "TargetCodeGenerator.h"


using std::ostream;
using std::ostringstream;
using std::endl;
using std::unordered_set;


class SimpleCodeGenerator : public ITargetCodeGenerator
{
private:
	// �ֱ�ָ��ͬһ�������Ķ�Ӧlabel
	size_t func_beg_index = 0;
	size_t func_mid_index = 0;
	size_t func_end_index = 0;
	// ������ǰ������Ϣ
	size_t stack_size = 0;
	string func_name = "__global";
	// IR��Ϣ
	shared_ptr<IrElemAllocator> allocator_ptr;
	shared_ptr<const IrTable> ir_table_ptr;
	// ����ƫ�Ʊ�
	unordered_map<irelem_t, unsigned> global_var_offset_table;
	unordered_map<irelem_t, unsigned> func_var_offset_table;
	unordered_map<irelem_t, string> string_label_table;
	// mips
	ostringstream buffer;	// Ŀ������buffer
	MipsInstructionFormatter mips;



	/// <summary>
	/// ����func_beg_index, func_mid_index, func_end_index, func_name.
	/// </summary>
	void next_function_info();

	/// <summary>
	/// Ϊÿ��ȫ�ֱ�������ռ�, ����label, �����ֵ;
	/// ��дglobal_var_offset_table, string_label_table;
	/// </summary>
	/// <returns></returns>
	void init_global();

	/// <summary>
	/// ɨ�赱ǰfunc_beg_index, func_mid_index, func_end_index��ע�ĺ���;
	/// ������дfunc_var_offset_table;
	/// ��������ջ��С��д��stack_size.
	/// </summary>
	void init_func();

	/// <summary>
	/// ��ɽ��뺯����ǰ�ĳ�ʼ������, �����������ʼ��.
	/// </summary>
	void beg_func();

	/// <summary>
	/// ��ɽ���main������ǰ�ĳ�ʼ������.
	/// </summary>
	void beg_main();

	/// <summary>
	/// ��ɷ��ع���
	/// </summary>
	/// <param name="stack_size"></param>
	void end_func();

	/// <summary>
	/// �˳�����
	/// </summary>
	void end_main();

	/// <summary>
	/// ��ջ�����, ���ػ�������ԭ�ȵ�����.
	/// </summary>
	string fresh_buffer();

	void load_val_to_reg(const string& reg, irelem_t val);

	void save_reg_to_var(const string& reg, irelem_t var);

	/// <summary>
	/// ����������.
	/// </summary>
	void func_body();
public:
	virtual ~SimpleCodeGenerator() = default;
	SimpleCodeGenerator(shared_ptr<IrElemAllocator> allocator, shared_ptr<const IrTable> ir_table)
		: allocator_ptr(allocator), ir_table_ptr(ir_table) { }

	/// <summary>
	/// ��allocatorָ���µ�irת��Ϊstring�����뵽os��.
	/// </summary>
	/// <param name="os"></param>
	virtual void translate(ostream& os);
};


#endif // !__SIMPLE_TARGET_GENERATOR_H__

