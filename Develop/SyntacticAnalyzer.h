#pragma once


#ifndef __SYNTACTIC_ANALYZER_H__
#define __SYNTACTIC_ANALYZER_H__

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>
#include "global_control.h"
#include "SymbolType.h"
#include "LexicalAnalyzer.h"
#include "SymbolToken.h"
#include "IdentifierTable.h"
#include "AbstractSyntacticAnalyzeTactics.h"

using std::string;
using std::vector;
using std::shared_ptr;
using std::unique_ptr;
using std::unordered_set;
using std::ostream;

class SyntacticAnalyzerEnvironment;



class SyntacticAnalyzer
{
private:
	SyntacticAnalyzerEnvironment env;

public:
	SyntacticAnalyzer(unique_ptr<LexicalAnalyzer> lexical_analyzer)
		: env(std::move(lexical_analyzer)) { }

	void parse();

	void print_all(ostream& os);
};

#endif // !__SYNTACTIC_ANALYZER_H__



