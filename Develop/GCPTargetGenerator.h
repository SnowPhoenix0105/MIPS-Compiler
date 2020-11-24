#pragma once

#ifndef __GCP_TARGET_GENERATOR_H__
#define __GCP_TARGET_GENERATOR_H__

#include "TargetCodeGenerator.h"


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
	// IR信息
	shared_ptr<IrElemAllocator> allocator_ptr;
	shared_ptr<const IrTable> origin_ir_table_ptr;
	IrTableBuilder buffer;
	IrFactory ir;
	// 临时寄存器池
	unordered_map<irelem_t, irelem_t> tmp_reg_pool;
	const vector<irelem_t> tmp_regs;
	// 目前被保存的变量
	unordered_set<irelem_t> protected_var;


	void init_tmp_reg_pool();

	void free_tmp_reg_of_var(irelem_t var);

	irelem_t alloc_tmp_reg_for_var(irelem_t var);

	irelem_t reload_var_to_reg(irelem_t var);

	void protect_var(irelem_t var);

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