#pragma once

#ifndef __GCP_TARGET_GENERATOR_H__
#define __GCP_TARGET_GENERATOR_H__

#include "TargetCodeGenerator.h"
#include "Detectors.h"
#include "global_control.h"
#include <memory>

using std::shared_ptr;
using std::unique_ptr;
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
	string func_name = "__global";
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
	/// 清空缓冲区, 返回缓冲区中原先的内容.
	/// </summary>
	string fresh_buffer();

	/// <summary>
	/// 更新func_beg_index, func_mid_index, func_end_index, func_name.
	/// </summary>
	void next_function_info();

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
	irelem_t trans_val_to_reg_or_cst(irelem_t val);

	/// <summary>
	/// 更新 call_index 和 remain_push
	/// </summary>
	void fresh_push_and_call_info();

	/// <summary>
	/// 遍历origin_ir_table并将所有非protect/reload语句中的变量名替换为寄存器名.
	/// </summary>
	void walk();

	/// <summary>
	/// 获取分配给某变量的寄存器, 若当前变量未被分配寄存器, 则为其分配一个寄存器, 记录并返回.
	/// 不会保证寄存器中的值为变量的值.
	/// $sp, $gp, $ret, $zero会返回相应的寄存器.
	/// </summary>
	/// <param name="var"></param>
	/// <returns></returns>
	irelem_t get_reg_of_var(irelem_t var);

	/// <summary>
	/// 确保某变量在寄存器中, 并返回保存其的寄存器.
	/// $sp, $gp, $ret, $zero会返回相应的寄存器.
	/// </summary>
	/// <param name="var"> 变量 </param>
	/// <returns>寄存器</returns>
	irelem_t ensure_var_in_reg(irelem_t var);

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