#include "SyntacticAnalyzer.h"

void SyntacticAnalyzerEnvironment::ensure_capacity(size_t size)
{
	while (size > symbols.size()) {
		if (!lexical_analyzer->has_next())
		{
			symbols.push_back(NIL);
		}
		SymbolType type = lexical_analyzer->next();
		shared_ptr<const Token> info = lexical_analyzer->get_token();
		symbols.push_back(info);
	}
}
