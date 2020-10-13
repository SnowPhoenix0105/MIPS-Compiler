#pragma once

#ifndef __SYMBOL_INFO_H__
#define __SYMBOL_INFO_H__


#include <memory>
#include <string>
#include "global_control.h"
#include "SymbolType.h"
#include "LexicalAnalyzer.h"
#include "SymbolInfo.h"


using std::string;
using std::unique_ptr;

struct SymbolInfo : IAbstractSyntaxTreeNode
{
	SymbolType type;
	unique_ptr<string> print_content;
	int line_number;

	SymbolInfo() = default;
	
	SymbolInfo(const LexicalAnalyzer& lexical_analyzer)
		: type(lexical_analyzer.get_symbol()),
		print_content(lexical_analyzer.formated_content()),
		line_number(lexical_analyzer.get_line_number()) { }

	virtual ~SymbolInfo() = default;

	virtual string print() const
	{
		string ret;
		ret += symboltype_output_dictionary.at(type);
		ret.push_back(' ');
		ret += *print_content;
		return ret;
	}
};

struct SymbolCharInfo : SymbolInfo
{
	char char_content;
	SymbolCharInfo() = default;
	SymbolCharInfo(const LexicalAnalyzer& lexical_analyzer)
		: SymbolInfo(lexical_analyzer),
		char_content((*lexical_analyzer.get_content())[1]) { }
};

struct SymbolUnsignedInfo : SymbolInfo
{
	unsigned int unsigned_content;
	SymbolUnsignedInfo() = default;
	SymbolUnsignedInfo(const LexicalAnalyzer& lexical_analyzer)
		: SymbolInfo(lexical_analyzer),
		unsigned_content(std::stoi(*lexical_analyzer.get_content())) { }
};

struct SymbolStringInfo : SymbolInfo
{
	shared_ptr<const string> string_content;
	SymbolStringInfo() = default;
	SymbolStringInfo(const LexicalAnalyzer& lexical_analyzer)
		: SymbolInfo(lexical_analyzer),
		string_content(lexical_analyzer.get_content()) { }
};

struct SymbolIdentifierInfo : SymbolInfo
{
	shared_ptr<const string> id_name_content;
	SymbolIdentifierInfo() = default;
	SymbolIdentifierInfo(const LexicalAnalyzer& lexical_analyzer)
		: SymbolInfo(lexical_analyzer),
		id_name_content(lexical_analyzer.get_lower_ident()) { }
};

#endif // !__SYMBOL_INFO_H__

