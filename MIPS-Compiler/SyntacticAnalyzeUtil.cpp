#include "SyntacticAnalyzeUtil.h"

void ErrorEnvironment::print_all_error(ostream& os)
{
	for (const auto& p : errors)
	{
		os << p.first << ' ' << p.second << std::endl;
	}
}


void MessageEnvironment::print_all_message(ostream& os)
{
	for (size_t i = 0; i != msg_index; ++i)
	{
		os << messages[i] << std::endl;
	}
}


const shared_ptr<Token> TokenEnvironment::NIL = make_shared<Token>();

void TokenEnvironment::ensure_capacity(size_t size)
{
	while (size >= symbols.size()) {
		if (!lexical_analyzer->has_next())
		{
			symbols.push_back(NIL);
			continue;
		}
		SymbolType next_type = lexical_analyzer->next();
		if (lexical_analyzer->is_wrong())
		{
			error_back(lexical_analyzer->get_line_number(), ErrorType::lexical_error);
		}
		shared_ptr<const Token> info = lexical_analyzer->get_token();
		symbols.push_back(info);
	}
}
