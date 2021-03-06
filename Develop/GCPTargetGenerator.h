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
	// 分别指向同一个函数的对应label
	size_t func_beg_index = 0;
	size_t func_mid_index = 0;
	size_t func_end_index = 0;
	// 其它当前函数信息
	size_t stack_size = 0;
	string func_name = "G";
	set<irelem_t> used_sregs;
	bool is_leaf = true;
	// IR信息
	shared_ptr<IrElemAllocator> allocator_ptr;
	shared_ptr<const IrTable> ir_table_ptr;
	// 变量偏移表
	unordered_map<irelem_t, unsigned> global_var_offset_table;
	unordered_map<irelem_t, unsigned> func_var_offset_table;
	unordered_map<irelem_t, string> string_label_table;
	// mips
	ostringstream buffer;	// 目标代码的buffer
	MipsInstructionFormatter mips;

	/// <summary>
	/// 为每个全局变量分配空间, 分配label, 填入初值;
	/// 填写global_var_offset_table, string_label_table;
	/// </summary>
	/// <returns></returns>
	void init_global();

	/// <summary>
	/// 更新func_beg_index, func_mid_index, func_end_index, func_name.
	/// </summary>
	void next_function_info();

	/// <summary>
	/// 扫描当前func_beg_index, func_mid_index, func_end_index标注的函数;
	/// 重新填写 func_var_offset_table 和 used_sregs;
	/// 计算运行栈大小并写入stack_size.
	/// </summary>
	void init_func();

	/// <summary>
	/// 完成进入函数体前的初始化工作, 不包括数组初始化.
	/// </summary>
	void beg_func();

	/// <summary>
	/// 完成返回工作
	/// </summary>
	/// <param name="stack_size"></param>
	void ret_func();

	/// <summary>
	/// 完成进入main函数体前的初始化工作.
	/// </summary>
	void beg_main();

	/// <summary>
	/// 退出程序
	/// </summary>
	void ret_main();


	/// <summary>
	/// 分析函数体.
	/// </summary>
	void func_body();

	/// <summary>
	/// 清空缓冲区, 返回缓冲区中原先的内容.
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
	size_t call_index = 0;		// 当 call_index > current 时, 表示已经有实参压参.
	size_t remain_push = 0;		// 包括本条push在内, 在下一个call之前的push指令的数量
	// 分别指向同一个函数的对应label
	size_t func_beg_index = 0;
	size_t func_mid_index = 0;
	size_t func_end_index = 0;
	// 当前函数分析结果
	unordered_set<size_t> block_begs;
	unordered_set<size_t> block_lasts;
	shared_ptr<const BlockDetectResult> block_detect_result;
	shared_ptr<const BlockVarActivetionAnalyzeResult> block_var_activition_analyze_result;
	shared_ptr<const VarActivetionAnalyzeResult> var_activition_analyze_result;
	// IR信息
	shared_ptr<IrElemAllocator> allocator_ptr;
	shared_ptr<const IrTable> origin_ir_table_ptr;
	IrTableBuilder buffer;
	IrFactory ir;
	const vector<irelem_t> tmp_regs;
	const vector<irelem_t> save_regs;
	// 临时寄存器池
	unordered_map<irelem_t, irelem_t> tmp_reg_pool;		// <reg> <var>
	unordered_map<irelem_t, bool> tmp_reg_dirty;		// <reg> bool	标记 $tx 被分配并重新加载后, 是否与加载处不同步
	// 全局寄存器分配情况
	unordered_set<irelem_t> keep_in_tx;		// <reg> 需要保持在 $tx 中的变量, 淘汰 $tx 时将不会将其淘汰
	unordered_map<irelem_t, irelem_t> save_reg_alloc;	// <var> <reg> 所有全局变量和param变量都必须在keys中, 未分配寄存器的变量的值为NIL, 包括 $sx 和 $ax 的分配结果
	vector<irelem_t> params;			// <var> 参数名
	unique_ptr<unordered_map<irelem_t, irelem_t>> var_status;	// 函数内局部和全局变量 key: <var>; value: 当变量在寄存器中时, 为<reg>, 在栈上时, 为<var>
	unordered_map<irelem_t, irelem_t> gvar_status;		// gvar key: <gvar>; value: 当变量在寄存器中时, 为<reg>, 在栈上时, 为<var>

	/// <summary>
	/// 更新
	/// func_beg_index, func_mid_index, func_end_index, func_name, 
	/// block_detect_result, block_var_activition_analyze_result, var_activition_analyze_result.
	/// 并进行全局变量寄存器分配.
	/// </summary>
	void next_function_info();


	/// <summary>
	/// 扫描并分配全局寄存器.
	/// 函数参数按照常规变量处理, 即$a0~$a3中的值会直接保存到栈中或其它寄存器中.
	/// </summary>
	void alloc_all_save_reg();

	/// <summary>
	/// 将临时寄存器池重置
	/// </summary>
	void init_tmp_reg_pool();

	/// <summary>
	/// 更新 call_index 和 remain_push
	/// </summary>
	void fresh_push_and_call_info();

	/// <summary>
	/// 遍历origin_ir_table并将所有非protect/reload语句中的变量名替换为寄存器名.
	/// </summary>
	void walk();

	/// <summary>
	/// 将 $tx 中的值同步到 svar/gvar 的栈上, 不会释放 $tx.
	/// 只有 dirty 置位时才会进行同步.
	/// </summary>
	void sync_no_reg_svar_and_gvar_in_treg();

	/// <summary>
	/// 将 dirty 未置位的 svar/gvar 占用的 $tx 释放.
	/// </summary>
	void free_treg_of_sync_no_reg_svar_and_gvar();

	void protect_all_vars_in_tmp_regs_to_stack();

	/// <summary>
	/// 回收所有不活跃的 $tx, 返回一个被回收的 $tx
	/// </summary>
	/// <returns></returns>
	irelem_t tmp_reg_gc();

	/// <summary>
	/// 返回一个 free 的 $tx
	/// 若临时寄存器池有寄存器, 则选择一个返回, 否则淘汰一个.
	/// </summary>
	/// <returns></returns>
	irelem_t alloc_tmp_reg();

	/// <summary>
	/// var转换为保存其的寄存器, cst保持原样输出.
	/// </summary>
	/// <param name="val"></param>
	/// <returns></returns>
	irelem_t use_reg_or_cst_of_val(irelem_t val);


	/// <summary>
	/// 获取分配给某变量的寄存器, 若当前变量未被分配寄存器, 则为其分配一个寄存器, 记录并返回.
	/// 不会保证寄存器中的值为变量的值.
	/// $sp, $gp, $ret, $zero会返回相应的寄存器.
	/// </summary>
	/// <param name="var"></param>
	/// <returns></returns>
	irelem_t write_reg_of_var(irelem_t var);

	/// <summary>
	/// 确保某变量在寄存器中, 并返回保存其的寄存器.
	/// $sp, $gp, $ret, $zero会返回相应的寄存器.
	/// </summary>
	/// <param name="var"> 变量 </param>
	/// <returns>寄存器</returns>
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