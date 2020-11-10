#include "IrTable.h"

string LabelAllocator::label_to_string(irelem_t label) const
{
	ASSERT(0, IrType::is_label(label));
    string ret;
	if (IrType::is_func(label))
	{
		ret += "func_";
	}
	else if (IrType::is_if(label))
	{
		ret += "if_";
	}
	else if (IrType::is_switch(label))
	{
		ret += "switch_";
	}
	else if (IrType::is_for(label))
	{
		ret += "for_";
	}
	else if (IrType::is_while(label))
	{
		ret += "while_";
	}
	else
	{
		string msg = "unknown label";
		msg += label;
		DEBUG_LOG_MSG(4, msg);
		ret += "unknow_";
	}
	
	if (IrType::is_beg(label))
	{
		ret += "beg_";
	}
	else if (IrType::is_mid(label))
	{
		ret += "mid_";
	}
	else if (IrType::is_end(label))
	{
		ret += "end_";
	}
	else
	{
		ret += "start_";
	}

	auto ord = IrType::get_ord(label);
	if (IrType::is_func)
	{
		ret += funcs.at(ord);
	}
	else
	{
		ret += ord;
	}

	return ret;
}

string LabelAllocator::func_name(irelem_t label) const
{
	ASSERT(5, IrType::is_func(label));
	return funcs.at(IrType::get_ord(label));
}

LabelAllocator& LabelAllocator::alloc_func(const string& name)
{
	stored = funcs.size() | 0x1000'0000;
	funcs.push_back(name);
	return *this;
}

LabelAllocator& LabelAllocator::alloc_if()
{
	stored = if_count++ | 0x2000'0000;
	return *this;
}

LabelAllocator& LabelAllocator::alloc_switch()
{
	stored = switch_count++ | 0x3000'0000;
	return *this;
}

LabelAllocator& LabelAllocator::alloc_for()
{
	stored =for_count++ | 0x4000'0000;
	return *this;
}

LabelAllocator& LabelAllocator::alloc_while()
{
	stored = while_count++ | 0x5000'0000;
	return *this;
}

irelem_t LabelAllocator::beg() const
{
	return stored;
}

irelem_t LabelAllocator::start() const
{
	return stored | 0x0400'0000;
}

irelem_t LabelAllocator::mid() const
{
	return stored | 0x0800'0000;
}

irelem_t LabelAllocator::end() const
{
	return stored | 0x0C00'0000;
}

VarAllocator::VarAllocator() : tmps(), nameds()
{
	_sp = alloc_named(make_shared<const string>("_sp"));
	_ret = alloc_named(make_shared<const string>("_ret"));
	_zero = alloc_named(make_shared<const string>("_zero"));
}

irelem_t VarAllocator::alloc_tmp()
{
	irelem_t ret = tmps.size() | 0xA000'0000;
	tmps.push_back(current_func);
	return ret;
}

irelem_t VarAllocator::alloc_named(shared_ptr<const string> name)
{
	irelem_t ret = nameds.size() | 0xA000'0000;
	nameds.push_back(make_pair(current_func, name));
	return ret;
}

string VarAllocator::var_to_string(irelem_t var)
{
	ASSERT(4, IrType::is_var(var));
	if (var == _sp)
	{
		return "$sp";
	}
	if (var == _ret)
	{
		return "$ret";
	}
	if (var == _zero)
	{
		return "$0";
	}
	size_t ord = IrType::get_ord(var);
	if (IrType::is_tmp(var))
	{
		return *tmps.at(ord) + "_tmp_" + to_string(ord);
	}
	const auto& pair = nameds.at(ord);
	return *pair.first + "_var_" + *pair.second;
}

irelem_t CstAllocator::alloc_imm(int imm)
{
	auto idx = imm_cache.find(imm);
	if (idx != imm_cache.end())
	{
		return *idx;
	}
	size_t ord = imms.size();
	irelem_t ret = 0x8000'0000 | ord;
	imms.push_back(imm);
	return ret;
}

irelem_t CstAllocator::alloc_arr(shared_ptr<const string> func, shared_ptr<const string> arr)
{
	size_t ord = arrs.size();
	irelem_t ret = 0x9000'0000 | ord;
	arrs.push_back(make_pair(func, arr));
	return ret;
}

irelem_t CstAllocator::cst_add(irelem_t cst_1, irelem_t cst_2)
{
	ASSERT(4, IrType::is_cst(cst_1));
	ASSERT(4, IrType::is_cst(cst_2));
	if (IrType::is_imm(cst_1) && IrType::is_imm(cst_2))
	{
		int val = imm_value(cst_1) + imm_value(cst_2);
		return alloc_imm(val);
	}
	size_t ord = incalculate_cst.size();
	irelem_t ret = 0x9800'0000 | ord;
	incalculate_cst.push_back(make_pair(cst_1, cst_2));
	return ret;
}

int CstAllocator::imm_value(irelem_t imm) const
{
	ASSERT(0, IrType::is_imm(imm));
	size_t ord = IrType::get_ord(imm);
	return imms.at(ord);
}

int CstAllocator::value_of(irelem_t cst) const
{
	ASSERT(0, IrType::is_cst(cst));
	if (IrType::is_imm(cst))
	{
		return imm_value(cst);
	}
	size_t ord = IrType::get_ord(cst);
	if (IrType::is_pure_arr(cst))
	{
		return arr_value.at(ord);
	}
	const auto& pair = incalculate_cst.at(ord);
	int val1 = value_of(pair.first);
	int val2 = value_of(pair.second);
	return val1 + val2;
}

IrTable IrTableBuilder::build()
{
	return IrTable(*this);
}
