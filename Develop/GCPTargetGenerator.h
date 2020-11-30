#pragma once

#ifndef __GCP_TARGET_GENERATOR_H__
#define __GCP_TARGET_GENERATOR_H__

#include "TargetCodeGenerator.h"
#include "Detectors.h"
#include "global_control.h"
#include <memory>
#include <set>

using std::shared_ptr;
using std::unique_ptr;
using std::set;
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
	string func_name = "G";
	set<irelem_t> used_sregs;
	bool is_leaf = true;
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
	/// Ϊÿ��ȫ�ֱ�������ռ�, ����label, �����ֵ;
	/// ��дglobal_var_offset_table, string_label_table;
	/// </summary>
	/// <returns></returns>
	void init_global();

	/// <summary>
	/// ����func_beg_index, func_mid_index, func_end_index, func_name.
	/// </summary>
	void next_function_info();

	/// <summary>
	/// ɨ�赱ǰfunc_beg_index, func_mid_index, func_end_index��ע�ĺ���;
	/// ������д func_var_offset_table �� used_sregs;
	/// ��������ջ��С��д��stack_size.
	/// </summary>
	void init_func();

	/// <summary>
	/// ��ɽ��뺯����ǰ�ĳ�ʼ������, �����������ʼ��.
	/// </summary>
	void beg_func();

	/// <summary>
	/// ��ɷ��ع���
	/// </summary>
	/// <param name="stack_size"></param>
	void ret_func();

	/// <summary>
	/// ��ɽ���main������ǰ�ĳ�ʼ������.
	/// </summary>
	void beg_main();

	/// <summary>
	/// �˳�����
	/// </summary>
	void ret_main();


	/// <summary>
	/// ����������.
	/// </summary>
	void func_body();

	/// <summary>
	/// ��ջ�����, ���ػ�������ԭ�ȵ�����.
	/// </summary>
	string fresh_buffer();

public:
	GCPTargetGenerator(shared_ptr<IrElemAllocator> allocator, shared_ptr<const IrTable> ir_table);
	virtual ~GCPTargetGenerator() = default;
	virtual void translate(ostream& os);
};

class GCPRegisterAllocator
{
	// status
	size_t current_index = 0;
	size_t call_index = 0;		// �� call_index > current ʱ, ��ʾ�Ѿ���ʵ��ѹ��.
	size_t remain_push = 0;		// ��������push����, ����һ��call֮ǰ��pushָ�������
	// �ֱ�ָ��ͬһ�������Ķ�Ӧlabel
	size_t func_beg_index = 0;
	size_t func_mid_index = 0;
	size_t func_end_index = 0;
	// ��ǰ�����������
	unordered_set<size_t> block_begs;
	unordered_set<size_t> block_lasts;
	shared_ptr<const BlockDetectResult> block_detect_result;
	shared_ptr<const BlockVarActivetionAnalyzeResult> block_var_activition_analyze_result;
	shared_ptr<const VarActivetionAnalyzeResult> var_activition_analyze_result;
	// IR��Ϣ
	shared_ptr<IrElemAllocator> allocator_ptr;
	shared_ptr<const IrTable> origin_ir_table_ptr;
	IrTableBuilder buffer;
	IrFactory ir;
	const vector<irelem_t> tmp_regs;
	const vector<irelem_t> save_regs;
	// ��ʱ�Ĵ�����
	unordered_map<irelem_t, irelem_t> tmp_reg_pool;		// <reg> <var>
	unordered_map<irelem_t, bool> tmp_reg_dirty;		// <reg> bool	��� $tx �����䲢���¼��غ�, �Ƿ�����ش���ͬ��
	// ȫ�ּĴ����������
	unordered_set<irelem_t> keep_in_tx;		// <reg> ��Ҫ������ $tx �еı���, ��̭ $tx ʱ�����Ὣ����̭
	unordered_map<irelem_t, irelem_t> save_reg_alloc;	// <var> <reg> ����ȫ�ֱ�����param������������keys��, δ����Ĵ����ı�����ֵΪNIL, ���� $sx �� $ax �ķ�����
	vector<irelem_t> params;			// <var> ������
	unique_ptr<unordered_map<irelem_t, irelem_t>> var_status;	// �����ھֲ���ȫ�ֱ��� key: <var>; value: �������ڼĴ�����ʱ, Ϊ<reg>, ��ջ��ʱ, Ϊ<var>
	unordered_map<irelem_t, irelem_t> gvar_status;		// gvar key: <gvar>; value: �������ڼĴ�����ʱ, Ϊ<reg>, ��ջ��ʱ, Ϊ<var>

	/// <summary>
	/// ����
	/// func_beg_index, func_mid_index, func_end_index, func_name, 
	/// block_detect_result, block_var_activition_analyze_result, var_activition_analyze_result.
	/// ������ȫ�ֱ����Ĵ�������.
	/// </summary>
	void next_function_info();


	/// <summary>
	/// ɨ�貢����ȫ�ּĴ���.
	/// �����������ճ����������, ��$a0~$a3�е�ֵ��ֱ�ӱ��浽ջ�л������Ĵ�����.
	/// </summary>
	void alloc_all_save_reg();

	/// <summary>
	/// ����ʱ�Ĵ���������
	/// </summary>
	void init_tmp_reg_pool();

	/// <summary>
	/// ���� call_index �� remain_push
	/// </summary>
	void fresh_push_and_call_info();

	/// <summary>
	/// ����origin_ir_table�������з�protect/reload����еı������滻Ϊ�Ĵ�����.
	/// </summary>
	void walk();

	/// <summary>
	/// �� $tx �е�ֵͬ���� svar/gvar ��ջ��, �����ͷ� $tx.
	/// ֻ�� dirty ��λʱ�Ż����ͬ��.
	/// </summary>
	void sync_no_reg_svar_and_gvar_in_treg();

	/// <summary>
	/// �� dirty δ��λ�� svar/gvar ռ�õ� $tx �ͷ�.
	/// </summary>
	void free_treg_of_sync_no_reg_svar_and_gvar();

	void protect_all_vars_in_tmp_regs_to_stack();

	/// <summary>
	/// �������в���Ծ�� $tx, ����һ�������յ� $tx
	/// </summary>
	/// <returns></returns>
	irelem_t tmp_reg_gc();

	/// <summary>
	/// ����һ�� free �� $tx
	/// ����ʱ�Ĵ������мĴ���, ��ѡ��һ������, ������̭һ��.
	/// </summary>
	/// <returns></returns>
	irelem_t alloc_tmp_reg();

	/// <summary>
	/// varת��Ϊ������ļĴ���, cst����ԭ�����.
	/// </summary>
	/// <param name="val"></param>
	/// <returns></returns>
	irelem_t use_reg_or_cst_of_val(irelem_t val);


	/// <summary>
	/// ��ȡ�����ĳ�����ļĴ���, ����ǰ����δ������Ĵ���, ��Ϊ�����һ���Ĵ���, ��¼������.
	/// ���ᱣ֤�Ĵ����е�ֵΪ������ֵ.
	/// $sp, $gp, $ret, $zero�᷵����Ӧ�ļĴ���.
	/// </summary>
	/// <param name="var"></param>
	/// <returns></returns>
	irelem_t write_reg_of_var(irelem_t var);

	/// <summary>
	/// ȷ��ĳ�����ڼĴ�����, �����ر�����ļĴ���.
	/// $sp, $gp, $ret, $zero�᷵����Ӧ�ļĴ���.
	/// </summary>
	/// <param name="var"> ���� </param>
	/// <returns>�Ĵ���</returns>
	irelem_t use_reg_of_var(irelem_t var);

	shared_ptr<IrTable> fresh_code_builder()
	{
		shared_ptr<IrTable> ret = buffer.build();
		buffer.clear();
		return ret;
	}

public:
	GCPRegisterAllocator(shared_ptr<IrElemAllocator> allocator, shared_ptr<const IrTable> ir_table);

	shared_ptr<IrTable> build();
};

#endif // !__GCP_TARGET_GENERATOR_H__