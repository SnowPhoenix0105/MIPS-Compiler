
#ifndef __SYMBOL_TOKEN_H__
#define __SYMBOL_TOKEN_H__

#include <memory>
#include <string>
#include "global_control.h"
#include "SymbolType.h"

using std::string;
using std::shared_ptr;

struct Token
{
	SymbolType type = SymbolType::end;
	shared_ptr<const string> print_content;
	int line_number;

	virtual ~Token() = default;
	virtual string to_print_string() const
	{
		string ret = symboltype_output_dictionary.at(type);
		ret.push_back(' ');
		ret += *print_content;
		return ret;
	}
};

struct CharToken : Token
{
	char char_content;
};

struct UnsignedToken : Token
{
	unsigned int unsigned_content;
};

struct StringToken : Token
{
	shared_ptr<const string> string_content;
};

struct IdentifierToken : Token
{
	shared_ptr<const string> id_name_content;
};

#endif // !__SYMBOL_TOKEN_H__

