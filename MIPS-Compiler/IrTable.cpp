#include "IrTable.h"
#include <sstream>

using std::ostringstream;
using std::endl;

const unordered_map<Reg, string> reg_name_trans =
{
	{	Reg::zero	, "$0"			},
	{	Reg::at		, "$at"			},
	{	Reg::v0		, "$v0"			},
	{	Reg::v1		, "$v1"			},
	{	Reg::a0		, "$a0"			},
	{	Reg::a1		, "$a1"			},
	{	Reg::a2		, "$a2"			},
	{	Reg::a3		, "$a3"			},
	{	Reg::t0		, "$t0"			},
	{	Reg::t1		, "$t1"			},
	{	Reg::t2		, "$t2"			},
	{	Reg::t3		, "$t3"			},
	{	Reg::t4		, "$t4"			},
	{	Reg::t5		, "$t5"			},
	{	Reg::t6		, "$t6"			},
	{	Reg::t7		, "$t7"			},
	{	Reg::s0		, "$s0"			},
	{	Reg::s1		, "$s1"			},
	{	Reg::s2		, "$s2"			},
	{	Reg::s3		, "$s3"			},
	{	Reg::s4		, "$s4"			},
	{	Reg::s5		, "$s5"			},
	{	Reg::s6		, "$s6"			},
	{	Reg::s7		, "$s7"			},
	{	Reg::t8		, "$t8"			},
	{	Reg::t9		, "$t9"			},
	{	Reg::k0		, "$k0"			},
	{	Reg::k1		, "$k1"			},
	{	Reg::gp		, "$gp"			},
	{	Reg::sp		, "$sp"			},
	{	Reg::fp		, "$fp"			},
	{	Reg::ra		, "$ra"			}
};


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
	else if (IrType::is_case(label))
	{
		ret += "case_";
		ret += to_string(IrType::get_ord(label));
		return ret;
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
	if (IrType::is_func(label))
	{
		ret += funcs.at(ord);
	}
	else
	{
		ret += to_string(ord);
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
	stored = funcs.size() | 0x10000000;
	funcs.push_back(name);
	return *this;
}

LabelAllocator& LabelAllocator::alloc_if()
{
	stored = if_count++ | 0x20000000;
	return *this;
}

LabelAllocator& LabelAllocator::alloc_switch()
{
	stored = switch_count++ | 0x30000000;
	return *this;
}

LabelAllocator& LabelAllocator::alloc_for()
{
	stored =for_count++ | 0x40000000;
	return *this;
}

LabelAllocator& LabelAllocator::alloc_while()
{
	stored = while_count++ | 0x50000000;
	return *this;
}

irelem_t LabelAllocator::alloc_case_label()
{
	return case_count++ | 0x70000000;
}

irelem_t LabelAllocator::beg() const
{
	return stored;
}

irelem_t LabelAllocator::start() const
{
	return stored | 0x04000000;
}

irelem_t LabelAllocator::mid() const
{
	return stored | 0x08000000;
}

irelem_t LabelAllocator::end() const
{
	return stored | 0x0C000000;
}

const shared_ptr<const string> VarAllocator::__global = make_shared<const string>("G");

VarAllocator::VarAllocator() : tmps(), nameds(), current_func(__global), _reserved_var(), regs()
{
	_sp = alloc_named(make_shared<const string>("_sp"));
	_gp = alloc_named(make_shared<const string>("_gp"));
	_ret = alloc_named(make_shared<const string>("_ret"));
	_zero = alloc_named(make_shared<const string>("_zero"));
	_reserved_var.insert(_sp);
	_reserved_var.insert(_gp);
	_reserved_var.insert(_ret);
	_reserved_var.insert(_zero);
	for (unsigned i = 0; i != 32; ++i)
	{
		Reg reg = Reg(i);
		irelem_t code = alloc_named(make_shared<const string>("_" + reg_name_trans.at(reg)));
		_reserved_var.insert(code);
		regs.insert(make_pair(reg, code));
		reg_trans.insert(make_pair(code, reg));
	}
}

irelem_t VarAllocator::alloc_tmp()
{
	irelem_t ret = tmps.size() | 0xA0000000;
	tmps.push_back(current_func);
	return ret;
}

irelem_t VarAllocator::alloc_named(shared_ptr<const string> name)
{
	irelem_t ret = nameds.size() | 0xB0000000;
	nameds.push_back(make_pair(current_func, name));
	return ret;
}

bool VarAllocator::is_reserved_var(irelem_t var) const
{
	return _reserved_var.count(var) != 0;
}

bool VarAllocator::is_global_var(irelem_t named) const
{
	if (!IrType::is_named(named))
	{
		return false;
	}
	return nameds.at(IrType::get_ord(named)).first == __global && !is_reserved_var(named);
}

bool VarAllocator::is_local_var(irelem_t var) const
{
	if (!IrType::is_var(var))
	{
		return false;
	}
	if (IrType::is_tmp(var))
	{
		return true;
	}
	return nameds.at(IrType::get_ord(var)).first != __global;
}

string VarAllocator::var_to_string(irelem_t var) const
{
	ASSERT(4, IrType::is_var(var));
	if (is_reserved_var(var))
	{
		if (var == _sp)
		{
			return "#sp";
		}
		if (var == _gp)
		{
			return "#gp";
		}
		if (var == _ret)
		{
			return "#ret";
		}
		if (var == _zero)
		{
			return "#zero";
		}
		return reg_name_trans.at(reg_trans.at(var));
	}
	size_t ord = IrType::get_ord(var);
	if (IrType::is_tmp(var))
	{
		return "t" + to_string(ord);
		// return *tmps.at(ord) + "_t" + to_string(ord);
	}
	const auto& pair = nameds.at(ord);
	return *pair.first + "_" + *pair.second;
}

irelem_t CstAllocator::alloc_imm(int imm)
{
	auto idx = imm_cache.find(imm);
	if (idx != imm_cache.end())
	{
		return idx->second;
	}
	size_t ord = imms.size();
	irelem_t ret = 0x80000000 | ord;
	imms.push_back(imm);
	imm_cache.insert(make_pair(imm, ret));
	return ret;
}

irelem_t CstAllocator::alloc_arr(shared_ptr<const string> arr)
{
	size_t ord = arrs.size();
	irelem_t ret = 0x90000000 | ord;
	arrs.push_back(make_pair(current_func, arr));
	return ret;
}

irelem_t CstAllocator::arr_root(irelem_t arr) const
{
	ASSERT(4, IrType::is_arr(arr));
	if (IrType::is_pure_arr(arr))
	{
		return arr;
	}
	auto ord = IrType::get_ord(arr);
	return arrs_with_offset.at(ord).first;
}

irelem_t CstAllocator::cst_add(irelem_t cst_1, irelem_t cst_2)
{
	ASSERT(4, IrType::is_cst(cst_1));
	ASSERT(4, IrType::is_cst(cst_2));
	if (IrType::is_imm(cst_1) && IrType::is_imm(cst_2))
	{
		int val = imm_to_value(cst_1) + imm_to_value(cst_2);
		return alloc_imm(val);
	}
	size_t ord = arrs_with_offset.size();
	irelem_t ret = 0x98000000 | ord;
	if (IrType::is_arr(cst_1))
	{
		ASSERT(4, IrType::is_imm(cst_2));
		irelem_t root = arr_root(cst_1);
		int offset = imm_to_value(cst_2);
		if (!IrType::is_pure_arr(cst_1))
		{
			size_t ord = IrType::get_ord(cst_1);
			const auto& pair = arrs_with_offset.at(ord);
			offset += pair.second;
		}
		arrs_with_offset.push_back(make_pair(root, offset));
	}
	else
	{
		ASSERT(1, IrType::is_arr(cst_2));
		ASSERT(4, IrType::is_imm(cst_1));
		irelem_t root = arr_root(cst_2);
		int offset = imm_to_value(cst_1);
		if (!IrType::is_pure_arr(cst_2))
		{
			size_t ord = IrType::get_ord(cst_2);
			const auto& pair = arrs_with_offset.at(ord);
			offset += pair.second;
		}
		arrs_with_offset.push_back(make_pair(root, offset));
	}
	return ret;
}

int CstAllocator::imm_to_value(irelem_t imm) const
{
	ASSERT(0, IrType::is_imm(imm));
	size_t ord = IrType::get_ord(imm);
	return imms.at(ord);
}

int CstAllocator::cst_to_value(irelem_t cst) const
{
	ASSERT(0, IrType::is_cst(cst));
	if (IrType::is_imm(cst))
	{
		return imm_to_value(cst);
	}
	if (IrType::is_pure_arr(cst))
	{
		return arr_value.at(cst);
	}
	size_t ord = IrType::get_ord(cst);
	const auto& pair = arrs_with_offset.at(ord);
	int arr = arr_value.at(pair.first);
	int off = pair.second;
	return arr + off;
}

string CstAllocator::cst_to_string(irelem_t cst) const
{
	using std::to_string;
	ASSERT(0, IrType::is_cst(cst));
	if (IrType::is_imm(cst))
	{
		return to_string(imm_to_value(cst));
	}
	size_t ord = IrType::get_ord(cst);
	if (IrType::is_pure_arr(cst))
	{
		const auto& arr = arrs.at(ord);
		return *(arr.first) + "_a_" + *(arr.second);
	}
	const auto& pair = arrs_with_offset.at(ord);
	irelem_t arr = pair.first;
	int off = pair.second;
	return "(" + cst_to_string(arr) + " + " + to_string(off) + ")";
}

irelem_t StringAllocator::alloc_string(shared_ptr<const string> str)
{
	return alloc_string(*str);
}

irelem_t StringAllocator::alloc_string(const string& str)
{
	const auto& it = str_map.find(str);
	if (it != str_map.end())
	{
		return it->second;
	}
	irelem_t ret = str_map.size() | 0xC0000000;
	string nstr;
	for (char c : str)
	{
		if (c == '\\')
		{
			nstr.push_back('\\');
		}
		nstr.push_back(c);
	}
	str_map.insert(make_pair(nstr, ret));
	elem_map.insert(make_pair(ret, nstr));
	return ret;
}

string IrElemAllocator::val_to_string(irelem_t elem) const
{
	if (IrType::is_cst(elem))
	{
		return cst_to_string(elem);
	}
	if (IrType::is_var(elem))
	{
		return var_to_string(elem);
	}
}

shared_ptr<IrTable> IrTableBuilder::build()
{
	return make_shared<IrTable>(*this);
}

string IrTable::to_string(const IrElemAllocator& allocator) const
{
	ostringstream builder;
	for (size_t i = 0; i != table.size(); ++i)
	{
		const auto& code = table.at(i);
	// int count = 0;
	//for (const auto& code : table)
	//{
		// ++count;
		to_string(builder, allocator, code);
	}
	return builder.str();
}

string IrTable::to_string(const IrElemAllocator& allocator, const Ir& code) const
{
	ostringstream builder;
	to_string(builder, allocator, code);
	return builder.str();
}

string IrTable::to_string_without_tab(const IrElemAllocator& allocator, size_t index) const
{
	string origin = to_string(allocator, index);
	ostringstream builder;
	for (char ch : origin)
	{
		switch (ch)
		{
		case '\t':
			builder << ' ';
			break;
		case '\n':
			break;
		default:
			builder << ch;
			break;
		}
	}
	return builder.str();
}

void IrTable::to_string(ostringstream& builder, const IrElemAllocator& allocator, const Ir& code) const
{
	switch (code.head)
	{
	case IrHead::label:
		builder << allocator.label_to_string(code.elem[0]) << ':' << endl;
		if (IrType::is_func(code.elem[0]) && IrType::is_end(code.elem[0]))
		{
			builder << "\n\n\n" << endl;
		}
		break;
	case IrHead::gvar:
		builder << "\tgvar\t" << allocator.var_to_string(code.elem[0]) << endl;
		break;
	case IrHead::arr:
		builder << "\taar\t" << allocator.cst_to_string(code.elem[0]) << ",\t"
			<< (code.elem[1] == IrType::_int ? "int,\t" : "char,\t")
			<< allocator.imm_to_value(code.elem[2]) << endl;
		break;
	case IrHead::init:
		builder << "\tinit\t" << allocator.imm_to_value(code.elem[0]) << endl;
		break;
	case IrHead::func:
		builder << "\tfunc\t" << (code.elem[0] == IrType::_int ? "int" : code.elem[0] == IrType::_char ? "char" : "void") << endl;
		break;
	case IrHead::param:
		builder << "\tparam\t"
			<< allocator.var_to_string(code.elem[0])
			// << '\t' 
			// << (code.elem[1] == IrType::NIL ? "NIL" : allocator.var_to_string(code.elem[1])) 
			<< endl;
		break;
	case IrHead::add:
		builder << '\t' << allocator.var_to_string(code.elem[0])
			<< "\t=\t"
			<< allocator.val_to_string(code.elem[1])
			<< "\t+\t"
			<< allocator.val_to_string(code.elem[2])
			<< endl;
		break;
	case IrHead::sub:
		builder << '\t' << allocator.var_to_string(code.elem[0])
			<< "\t=\t"
			<< allocator.val_to_string(code.elem[1])
			<< "\t-\t"
			<< allocator.val_to_string(code.elem[2])
			<< endl;
		break;
	case IrHead::mult:
		builder << '\t' << allocator.var_to_string(code.elem[0])
			<< "\t=\t"
			<< allocator.val_to_string(code.elem[1])
			<< "\t*\t"
			<< allocator.val_to_string(code.elem[2])
			<< endl;
		break;
	case IrHead::div:
		builder << '\t' << allocator.var_to_string(code.elem[0])
			<< "\t=\t"
			<< allocator.val_to_string(code.elem[1])
			<< "\t/\t"
			<< allocator.val_to_string(code.elem[2])
			<< endl;
		break;
	case IrHead::_and:
		builder << '\t' << allocator.var_to_string(code.elem[0])
			<< "\t=\t"
			<< allocator.val_to_string(code.elem[1])
			<< "\t&\t"
			<< allocator.val_to_string(code.elem[2])
			<< endl;
		break;
	case IrHead::_or:
		builder << '\t' << allocator.var_to_string(code.elem[0])
			<< "\t=\t"
			<< allocator.val_to_string(code.elem[1])
			<< "\t|\t"
			<< allocator.val_to_string(code.elem[2])
			<< endl;
		break;
	case IrHead::_nor:
		builder << '\t' << allocator.var_to_string(code.elem[0])
			<< "\t=\t"
			<< allocator.val_to_string(code.elem[1])
			<< "\tnor\t"
			<< allocator.val_to_string(code.elem[2])
			<< endl;
		break;
	case IrHead::_xor:
		builder << '\t' << allocator.var_to_string(code.elem[0])
			<< "\t=\t"
			<< allocator.val_to_string(code.elem[1])
			<< "\t^\t"
			<< allocator.val_to_string(code.elem[2])
			<< endl;
		break;
	case IrHead::sl:
		builder << '\t' << allocator.var_to_string(code.elem[0])
			<< "\t=\t"
			<< allocator.val_to_string(code.elem[1])
			<< "\t<<\t"
			<< allocator.val_to_string(code.elem[2])
			<< endl;
		break;
	case IrHead::sr:
		builder << '\t' << allocator.var_to_string(code.elem[0])
			<< "\t=\t"
			<< allocator.val_to_string(code.elem[1])
			<< "\t>>\t"
			<< allocator.val_to_string(code.elem[2])
			<< endl;
		break;
	case IrHead::less:
		builder << '\t' << allocator.var_to_string(code.elem[0])
			<< "\t=\t"
			<< allocator.val_to_string(code.elem[1])
			<< "\t<\t"
			<< allocator.val_to_string(code.elem[2])
			<< endl;
		break;
	case IrHead::lw:
		builder << '\t' << allocator.var_to_string(code.elem[0])
			<< "\t=\t"
			<< allocator.val_to_string(code.elem[1])
			<< '['
			<< allocator.cst_to_string(code.elem[2])
			<< ']'
			<< endl;
		break;
	case IrHead::lb:
		builder << '\t' << allocator.var_to_string(code.elem[0])
			<< "\t|=\t"
			<< allocator.val_to_string(code.elem[1])
			<< '['
			<< allocator.cst_to_string(code.elem[2])
			<< ']'
			<< endl;
		break;
	case IrHead::sw:
		builder << '\t' << allocator.val_to_string(code.elem[1])
			<< '['
			<< allocator.cst_to_string(code.elem[2])
			<< ']'
			<< "\t=\t"
			<< allocator.val_to_string(code.elem[0])
			<< endl;
		break;
	case IrHead::sb:
		builder << '\t' << allocator.val_to_string(code.elem[1])
			<< '['
			<< allocator.cst_to_string(code.elem[2])
			<< ']'
			<< "\t|=\t"
			<< allocator.val_to_string(code.elem[0])
			<< endl;
		break;
	case IrHead::beq:
		builder << "\tif " << allocator.val_to_string(code.elem[0])
			<< " == "
			<< allocator.val_to_string(code.elem[1])
			<< "\tgoto\t"
			<< allocator.label_to_string(code.elem[2])
			<< endl;
		break;
	case IrHead::bne:
		builder << "\tif " << allocator.val_to_string(code.elem[0])
			<< " != "
			<< allocator.val_to_string(code.elem[1])
			<< "\tgoto\t"
			<< allocator.label_to_string(code.elem[2])
			<< endl;
		break;
	case IrHead::_goto:
		builder << "\tgoto\t"
			<< allocator.label_to_string(code.elem[0])
			<< endl;
		break;
	case IrHead::push:
		builder << "\tpush\t" << allocator.val_to_string(code.elem[0]) << endl;
		break;
	case IrHead::call:
		builder << "\tcall\t"
			<< allocator.label_to_string(code.elem[0])
			<< endl;
		break;
	case IrHead::ret:
		builder << "\tret" << endl;
		break;
	case IrHead::scanf:
		builder << "\tscanf\t" << allocator.var_to_string(code.elem[0])
			<< ",\t"
			<< (code.elem[1] == IrType::_int ? "int" : "char")
			<< endl;
		break;
	case IrHead::printf:
		builder << "\tprintf\t"
			<< (code.elem[0] == IrType::NIL ? "NIL" : allocator.str_to_string(code.elem[0]))
			<< ",\t"
			<< (code.elem[1] == IrType::NIL ? "NIL" : allocator.val_to_string(code.elem[1]))
			<< ",\t"
			<< (code.elem[2] == IrType::_int ? "int" : "char")
			<< endl;
		break;
	case IrHead::movn:
		builder << "\tif " << allocator.var_to_string(code.elem[2])
			<< " != 0"
			<< allocator.var_to_string(code.elem[0])
			<< "\t=\t"
			<< allocator.var_to_string(code.elem[1])
			<< endl;
		break;
	case IrHead::movz:
		builder << "\tif " << allocator.var_to_string(code.elem[2])
			<< " == 0"
			<< allocator.var_to_string(code.elem[0])
			<< "\t=\t"
			<< allocator.var_to_string(code.elem[1])
			<< endl;
		break;
	case IrHead::protect:
		builder << "\tprotect\t"
			<< (code.elem[0] == IrType::NIL ? "NIL" : allocator.var_to_string(code.elem[0]))
			<< ",\t"
			<< (code.elem[1] == IrType::NIL ? "NIL" : allocator.var_to_string(code.elem[1]))
			<< endl;
		break;
	case IrHead::reload:
		builder << "\treload\t"
			<< (code.elem[0] == IrType::NIL ? "NIL" : allocator.var_to_string(code.elem[0]))
			<< ",\t"
			<< (code.elem[1] == IrType::NIL ? "NIL" : allocator.var_to_string(code.elem[1]))
			<< endl;
		break;
	default:
		break;
	}
}
