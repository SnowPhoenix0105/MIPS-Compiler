#pragma once

#ifndef __GCP_TARGET_GENERATOR_H__
#define __GCP_TARGET_GENERATOR_H__

#include "TargetCodeGenerator.h"
#include "Detectors.h"
#include "global_control.h"
#include <memory>

using std::shared_ptr;
using IrDetectors::VarActivetionAnalyzeResult;
using IrDetectors::BlockDetectResult;
using IrDetectors::BlockVarActivetionAnalyzeResult;

class GCPTargetGenerator : public ITargetCodeGenerator
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
	/// ��ջ�����, ���ػ�������ԭ�ȵ�����.
	/// </summary>
	string fresh_buffer();

	/// <summary>
	/// ����func_beg_index, func_mid_index, func_end_index, func_name.
	/// </summary>
	void next_function_info();

public:
	GCPTargetGenerator(shared_ptr<IrElemAllocator> allocator, shared_ptr<const IrTable> ir_table);
	virtual ~GCPTargetGenerator() = default;
	virtual void translate(ostream& os);
};

class GCPRegisterAllocator
{
	// �ֱ�ָ��ͬһ�������Ķ�Ӧlabel
	size_t func_beg_index = 0;
	size_t func_mid_index = 0;
	size_t func_end_index = 0;
	// ������ǰ������Ϣ
	size_t stack_size = 0;
	string func_name = "__global";
	shared_ptr<const BlockDetectResult> block_detect_result;
	shared_ptr<const BlockVarActivetionAnalyzeResult> block_var_activition_analyze_result;
	shared_ptr<const VarActivetionAnalyzeResult> var_activition_analyze_result;
	// IR��Ϣ
	shared_ptr<IrElemAllocator> allocator_ptr;
	shared_ptr<const IrTable> origin_ir_table_ptr;
	IrTableBuilder buffer;
	IrFactory ir;
	// ��ʱ�Ĵ�����
	unordered_map<irelem_t, irelem_t> tmp_reg_pool;		// <reg> <var>
	const vector<irelem_t> tmp_regs;
	// ȫ�ּĴ����������
	unordered_map<irelem_t, irelem_t> save_reg_alloc;	// <var> <reg> ����ȫ�ּĴ�����������keys��, δ����Ĵ����ı�����ֵΪNIL
	// Ŀǰ������ı���
	unordered_set<irelem_t> protected_var;
	unordered_map<irelem_t, irelem_t> using_global;		// <g-var> <reg>

	void init_global();

	/// <summary>
	/// ����func_beg_index, func_mid_index, func_end_index, func_name.
	/// </summary>
	void next_function_info();


	/// <summary>
	/// ɨ�貢����ȫ�ּĴ���.
	/// �����������ճ����������, ��$a0~$a3�е�ֵ��ֱ�ӱ��浽ջ�л������Ĵ�����.
	/// </summary>
	void alloc_save_reg();

	/// <summary>
	/// ����ʱ�Ĵ������ÿ�
	/// </summary>
	void init_tmp_reg_pool();

	void analyze_func();

	/// <summary>
	/// ��ȡ�����ĳ�����ļĴ���, ����ǰ����δ������Ĵ���, ��Ϊ�����һ��������.
	/// ���ᱣ֤�Ĵ����е�ֵΪ������ֵ.
	/// </summary>
	/// <param name="var"></param>
	/// <returns></returns>
	irelem_t get_reg_of_var(irelem_t var);

	/// <summary>
	/// ȷ��ĳ�����ڼĴ�����, �����ر�����ļĴ���
	/// </summary>
	/// <param name="var"> ���� </param>
	/// <returns>�Ĵ���</returns>
	irelem_t ensure_var_in_reg(irelem_t var);

	void free_reg_and_protect_content(irelem_t reg);

	shared_ptr<IrTable> fresh_code_builder()
	{
		shared_ptr<IrTable> ret = buffer.build();
		buffer.clear();
		return ret;
	}

public:
	GCPRegisterAllocator(shared_ptr<IrElemAllocator> allocator, shared_ptr<const IrTable> ir_table);

	shared_ptr<const IrTable> build();
};

#endif // !__GCP_TARGET_GENERATOR_H__