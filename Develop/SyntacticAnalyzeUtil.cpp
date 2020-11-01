#include "SyntacticAnalyzeUtil.h"

void ErrorEnvironment::print_all_error(ostream& os)
{
	for (const auto& p : errors)
	{
		os << p.first.first << ' ' << p.first.second << std::endl;
	}
}


void MessageEnvironment::print_all_message(ostream& os)
{
	for (size_t i = 0; i != msg_index; ++i)
	{
		os << messages[i] << std::endl;
	}
}


const TokenEnvironment::token_ptr TokenEnvironment::NIL = make_shared<Token>();

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

TokenEnvironment::token_ptr TokenEnvironment::dequeue_certain(SymbolType type)
{
	ensure_capacity(sym_index);
	if (symbols[sym_index]->type == type)
	{
		return dequeue();
	}
	if (type == SymbolType::semicolon)
	{
		error_require(symbols[sym_index - 1]->line_number, type);
	}
	else
	{
		error_require(symbols[sym_index]->line_number, type);
	}
	return nullptr;
}

TokenEnvironment::token_ptr TokenEnvironment::dequeue_certain_and_message_back(SymbolType type)
{
	ensure_capacity(sym_index);
	if (symbols[sym_index]->type == type)
	{
		return dequeue_and_message_back();
	}
	if (type == SymbolType::semicolon)
	{
		error_require(symbols[sym_index - 1]->line_number, type);
	}
	else
	{
		error_require(symbols[sym_index]->line_number, type);
	}
	return nullptr;
}

bool SyntacticAnalyzerEnvironment::ensure_func(
	function<bool(SyntacticAnalyzerEnvironment&)> success_condition,
	function<bool(SyntacticAnalyzerEnvironment&)> next_condition,
	ErrorType error_type, 
	unsigned max_turn
) 
{
	if (success_condition(*this))
	{
		return true;
	}
	error_back(symbols[sym_index]->line_number, error_type);
	size_t protec_index = sym_index;
	for (unsigned i = 0; i < max_turn; ++i)
	{
		if (success_condition(*this))
		{
			return true;
		}
		if (next_condition(*this))
		{
			return false;
		}
		if (peek() == SymbolType::end)
		{
			break;
		}
		dequeue_and_message_back();
	}
	sym_index = protec_index;
	throw reach_end_exception();
}