

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

class SyntacticAnalyzer
{
private:
	SyntacticAnalyzerEnvironment env;

public:
	SyntacticAnalyzer(unique_ptr<LexicalAnalyzer> lexical_analyzer)
		: env(std::move(lexical_analyzer)) { }
	void parse();
	void print_all_message(ostream& os);
	void print_all_error(ostream& os);
	shared_ptr<IrElemAllocator> get_allocator_ptr() const { return env.get_allocator_ptr(); }
	shared_ptr<IrTable> get_ir_table() { return env.fresh_code_builder(); }
};

#endif // !__SYNTACTIC_ANALYZER_H__



