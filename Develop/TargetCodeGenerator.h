#pragma once

#ifndef __TARGET_CODE_GENERATOR_H__
#define __TARGET_CODE_GENERATOR_H__

#include "global_control.h"
#include "IrTable.h"
#include <iostream>
#include <sstream>
#include <unordered_set>

using std::ostream;
using std::ostringstream;
using std::endl;
using std::unordered_set;
using std::to_string;

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

struct MipsInstructionFormatter
{
	string mem_op(const string& op, const string& target, const string& base, int offset) const
	{
		ostringstream oss;
		oss << '\t' << op << '\t' << target << ',' << offset << '(' << base << ')';
		return oss.str();
	}

	string quaternary(const string& op, const string& r1, const string& r2, const string& r3) const
	{
		ostringstream oss;
		oss << '\t' << op << '\t' << r1 << ',' << r2 << ',' << r3;
		return oss.str();
	}

	string triple(const string& op, const string& r1, const string& r2) const
	{
		ostringstream oss;
		oss << '\t' << op << '\t' << r1 << ',' << r2;
		return oss.str();
	}

	string tuple(const string& op, const string& reg) const
	{
		ostringstream oss;
		oss << '\t' << op << '\t' << reg;
		return oss.str();
	}

	string label(const string& label) const
	{
		ostringstream oss;
		oss <<  label << ':';
		return oss.str();
	}

	// 四元组
	string addu(const string& target, const string& source1, string source2) const
	{
		return quaternary("addu", target, source1, source2);
	}

	string addu(const string& target, const string& source1, int source2) const
	{
		return quaternary("addiu", target, source1, to_string(source2));
	}

	string subu(const string& target, const string& source1, string source2) const
	{
		return quaternary("subu", target, source1, source2);
	}

	string subu(const string& target, const string& source1, int source2) const
	{
		return quaternary("addiu", target, source1, to_string(-source2));
	}

	string _and(const string& target, const string& source1, string source2) const
	{
		return quaternary("and", target, source1, source2);
	}

	string _and(const string& target, const string& source1, int source2) const
	{
		return quaternary("andi", target, source1, to_string(source2));
	}

	string _or(const string& target, const string& source1, string source2) const
	{
		return quaternary("or", target, source1, source2);
	}

	string _or(const string& target, const string& source1, int source2) const
	{
		return quaternary("ori", target, source1, to_string(source2));
	}

	string _nor(const string& target, const string& source1, string source2) const
	{
		return quaternary("nor", target, source1, source2);
	}

	string _nor(const string& target, const string& source1, int source2) const
	{
		return quaternary("nori", target, source1, to_string(source2));
	}

	string _xor(const string& target, const string& source1, string source2) const
	{
		return quaternary("xor", target, source1, source2);
	}

	string _xori(const string& target, const string& source1, int source2) const
	{
		return quaternary("xori", target, source1, to_string(source2));
	}

	string sll(const string& target, const string& source1, string source2) const
	{
		return quaternary("sllv", target, source1, source2);
	}

	string sll(const string& target, const string& source1, int source2) const
	{
		return quaternary("sll", target, source1, to_string(source2));
	}

	string sra(const string& target, const string& source1, string source2) const
	{
		return quaternary("srav", target, source1, source2);
	}

	string sra(const string& target, const string& source1, int source2) const
	{
		return quaternary("sra", target, source1, to_string(source2));
	}

	string slt(const string& target, const string& source1, string source2) const
	{
		return quaternary("slt", target, source1, source2);
	}

	string slt(const string& target, const string& source1, int source2) const
	{
		return quaternary("slti", target, source1, to_string(source2));
	}

	string mulo(const string& target, const string& source1, string source2) const
	{
		return quaternary("mulo", target, source1, source2);
	}

	string mulo(const string& target, const string& source1, int source2) const
	{
		return quaternary("mulo", target, source1, to_string(source2));
	}

	string div(const string& target, const string& source1, string source2) const
	{
		return quaternary("div", target, source1, source2);
	}

	string div(const string& target, const string& source1, int source2) const
	{
		return quaternary("div", target, source1, to_string(source2));
	}

	string beq(const string& reg1, const string& reg2, string label) const
	{
		return quaternary("beq", reg1, reg2, label);
	}

	string bne(const string& reg1, const string& reg2, string label) const
	{
		return quaternary("bne", reg1, reg2, label);
	}

	

	// 间接寻址型四元组
	string lw(const string& target, const string& base, int offset) const
	{
		return mem_op("lw", target, base, offset);
	}

	string sw(const string& target, const string& base, int offset) const
	{
		return mem_op("sw", target, base, offset);
	}

	string lb(const string& target, const string& base, int offset) const
	{
		return mem_op("lb", target, base, offset);
	}

	string sb(const string& target, const string& base, int offset) const
	{
		return mem_op("sb", target, base, offset);
	}




	// 三元组
	string mul(const string& source1, const string& source2) const
	{
		return triple("mul", source1, source2);
	}

	string mul(const string& source1, int source2) const
	{
		return triple("mul", source1, to_string(source2));
	}

	string div(const string& source1, const string& source2) const
	{
		return triple("div", source1, source2);
	}

	string div(const string& source1, int source2) const
	{
		return triple("div", source1, to_string(source2));
	}

	string la(const string& target, const string& label) const
	{
		return triple("la", target, label);
	}

	string li(const string& target, int value) const
	{
		return triple("li", target, to_string(value));
	}




	// 二元组

	string mfhi(const string& target) const
	{
		return tuple("mfhi", target);
	}

	string mflo(const string& target) const
	{
		return tuple("mflo", target);
	}

	string jr(const string& target) const
	{
		return tuple("jr", target);
	}

	string jr() const
	{
		return tuple("jr", "$ra");
	}

	string j(const string& target) const
	{
		return tuple("j", target);
	}

	string jal(const string& target) const
	{
		return tuple("jal", target);
	}

};



#endif // !__TARGET_CODE_GENERATOR_H__
