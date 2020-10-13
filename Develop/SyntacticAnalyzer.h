#pragma once


#ifndef __SYNTACTIC_ANALYZER_H__
#define __SYNTACTIC_ANALYZER_H__

#include <memory>
#include <vector>
#include <string>
#include "global_control.h"
#include "SymbolType.h"
#include "LexicalAnalyzer.h"
#include "AbstractSyntaxTree.h"
#include "SymbolInfo.h"

using std::string;
using std::vector;
using std::shared_ptr;
using std::unique_ptr;



class SyntacticAnalyzer
{
private:
	static const shared_ptr<SymbolInfo> NIL;
	unique_ptr<LexicalAnalyzer> lexical_analyzer;
	shared_ptr<const IAbstractSyntaxTreeNode> ast;
	size_t sym_index = 0;
	vector<shared_ptr<const SymbolInfo>> symbols;

	shared_ptr<const SymbolInfo> peek(size_t offset = 0)
	{
		ensure_capacity(sym_index + offset);
		return symbols[sym_index + offset];
	}

	shared_ptr<const SymbolInfo> dequeue() 
	{
		ensure_capacity(sym_index);
		return symbols[sym_index++];
	}

	void ensure_capacity(size_t size);

	void error(bool trying, const SymbolInfo& info)
	{
		//TODO
	}

	shared_ptr<const ProgrameNode> analyze_program(bool trying)
	{

	}


public:
	SyntacticAnalyzer(unique_ptr<LexicalAnalyzer> lexical_analyzer)
		: lexical_analyzer(std::move(lexical_analyzer)) { }

	shared_ptr<const IAbstractSyntaxTreeNode> get_ast() 
	{
		if (ast == nullptr)
		{
			ast = analyze_program(false);
		}
		return ast;
	}

	
};

#endif // !__SYNTACTIC_ANALYZER_H__



