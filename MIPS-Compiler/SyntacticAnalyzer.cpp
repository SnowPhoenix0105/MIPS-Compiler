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

void SyntacticAnalyzerEnvironment::print_all(ostream& os)
{
	for (size_t i = 0; i != current_state.msg_index; ++i)
	{
		os << messages[i] << std::endl;
	}
}

void SyntacticAnalyzer::parse()
{
	ProgramAnalyze()(env);
}


void SyntacticAnalyzer::print_all()
{
	env.print_all(*os);
}