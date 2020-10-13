#include "SyntacticAnalyzer.h"

void SyntacticAnalyzer::ensure_capacity(size_t size)
{
	while (size > symbols.size()) {
		if (!lexical_analyzer->has_next())
		{
			symbols.push_back(NIL);
		}
		SymbolType type = lexical_analyzer->next();
		shared_ptr<SymbolInfo> info;
		switch (type)
		{
			break;
		case SymbolType::character:
			info = make_shared<SymbolCharInfo>(*lexical_analyzer);
			break;
		case SymbolType::key_int:
			info = make_shared<SymbolUnsignedInfo>(*lexical_analyzer);
			break;
		case SymbolType::string:
			info = make_shared<SymbolStringInfo>(*lexical_analyzer);
		case SymbolType::identifier:
			info = make_shared<SymbolIdentifierInfo>(*lexical_analyzer);
			break;
		default:
			info = make_shared<SymbolInfo>(*lexical_analyzer);
			break;
		}
		symbols.push_back(info);
	}
}
