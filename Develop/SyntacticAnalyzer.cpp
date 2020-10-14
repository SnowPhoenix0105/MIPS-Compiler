#include "SyntacticAnalyzer.h"

void SyntacticAnalyzerEnvironment::ensure_capacity(size_t size)
{
	while (size > symbols.size()) {
		if (!lexical_analyzer->has_next())
		{
			symbols.push_back(NIL);
		}
		SymbolType type = lexical_analyzer->next();
		shared_ptr<Token> info;
		switch (type)
		{
		case SymbolType::character:
			info = make_shared<CharToken>(*lexical_analyzer);
			break;
		case SymbolType::key_int:
			info = make_shared<UnsignedToken>(*lexical_analyzer);
			break;
		case SymbolType::string:
			info = make_shared<StringToken>(*lexical_analyzer);
			break;
		case SymbolType::identifier:
			info = make_shared<IdentifierToken>(*lexical_analyzer);
			break;
		default:
			info = make_shared<Token>(*lexical_analyzer);
			break;
		}
		symbols.push_back(info);
	}
}
