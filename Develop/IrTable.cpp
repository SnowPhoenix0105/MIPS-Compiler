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
