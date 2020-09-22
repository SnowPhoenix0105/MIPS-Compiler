#include "LexicalAnalyzer.h"
#include <sstream>


///////////////////////////////////////							///////////////////////////////////////
///////////////////////////////////////  base control function  ///////////////////////////////////////
///////////////////////////////////////							///////////////////////////////////////

LexicalAnalyzer::LexicalAnalyzer(LexicalAnalyzer&& mov) noexcept
	:input_stream(mov.input_stream.release()), last_symbol(), last_content()
{
	*input_stream >> std::noskipws;
	read_until_not_space();
}

LexicalAnalyzer& LexicalAnalyzer::operator=(LexicalAnalyzer&& mov) noexcept
{
	input_stream.reset(mov.input_stream.release());
	last_content = std::move(mov.last_content);
	last_symbol = mov.last_symbol;
	last_ch = mov.last_ch;
	return *this;
}

LexicalAnalyzer::LexicalAnalyzer(const string& input_file_name)
	:input_stream(new std::ifstream(input_file_name)), last_symbol(), last_content()
{
	*input_stream >> std::noskipws;
	read_until_not_space();
}

LexicalAnalyzer::LexicalAnalyzer(unique_ptr<istream>&& input_file)
	:input_stream(std::move(input_file)), last_symbol(), last_content()
{
	*input_stream >> std::noskipws;
	read_until_not_space();
}


///////////////////////////////////////							///////////////////////////////////////
///////////////////////////////////////  private tool function  ///////////////////////////////////////
///////////////////////////////////////							///////////////////////////////////////


void LexicalAnalyzer::read_until_not_space()
{
	if (!has_next())
	{
		return;
	}
	while (std::isspace(last_ch))
	{
		if (last_ch == '\n')
		{
			++line_number;
		}
		if (!(*input_stream >> last_ch))
		{
			return;
		}
	}
}

void LexicalAnalyzer::check_compare_symbol(SymbolType long_type, SymbolType short_type)
{
	if (*input_stream >> last_ch && last_ch == '=')
	{
		last_symbol = long_type;
		last_content.push_back('=');
		*input_stream >> last_ch;
	}
	else
	{
		last_symbol = short_type;
	}
}

void LexicalAnalyzer::check_character_symbol()
{
	*input_stream >> last_ch;
	if (std::isdigit(last_ch) || is_legal_alpha(last_ch) || last_ch == '*' || last_ch == '+')
	{
		last_content.push_back(last_ch);
		*input_stream >> last_ch;
		if (last_ch == '\'')
		{
			last_content.push_back('\'');
			last_symbol = SymbolType::character;
			*input_stream >> last_ch;
			return;
		}
	}
	last_symbol = SymbolType::wrong;
}

void LexicalAnalyzer::check_string_symbol()
{
	while (*input_stream >> last_ch)
	{
		if (last_ch >= ' ' && last_ch <= '~')
		{
			last_content.push_back(last_ch);
			if (last_ch == '\"')
			{
				last_symbol = SymbolType::string;
				*input_stream >> last_ch;
				return;
			}
		}
		else
		{
			last_symbol = SymbolType::wrong;
			return;
		}
	}
	last_symbol = SymbolType::wrong;
}

void LexicalAnalyzer::check_number_symbol()
{
	while (*input_stream >> last_ch && std::isdigit(last_ch))
	{
		last_content.push_back(last_ch);
	}
	last_symbol = SymbolType::number;
}

void LexicalAnalyzer::check_name_symbol()
{
	while (*input_stream >> last_ch && (is_legal_alpha(last_ch) || std::isdigit(last_ch)))
	{
		last_content.push_back(last_ch);
	}
	for (char& ch : last_content)
	{
		if (std::isupper(ch))
		{
			ch = std::tolower(ch);
		}
	}
	auto result = saved_key_words_dictionary.find(last_content);
	if (result == saved_key_words_dictionary.end())
	{
		last_symbol = SymbolType::names;
	}
	else
	{
		last_symbol = result->second;
	}
}

///////////////////////////////////////						///////////////////////////////////////
/////////////////////////////////////// public main method  ///////////////////////////////////////
///////////////////////////////////////						///////////////////////////////////////


SymbolType LexicalAnalyzer::next()
{
	if (!has_next())
	{
		throw TryToGetUnexistNextException("try to call next() at a LexicalAnalyzer with has_next() == false");
	}
	last_content.clear();
	last_content.push_back(last_ch);
	switch (last_ch)
	{
	case '\'':
		check_character_symbol();
		break;
	case '\"':
		check_string_symbol();
		break;
	case '<':
		check_compare_symbol(SymbolType::less_equal, SymbolType::less);
		break;
	case '>':
		check_compare_symbol(SymbolType::greater_equal, SymbolType::greater);
		break;
	case '=':
		check_compare_symbol(SymbolType::equal, SymbolType::assign);
		break;
	case '!':
		check_compare_symbol(SymbolType::not_equal, SymbolType::wrong);
		break;
	default:
		auto result = simple_symbol_dictionary.find(last_ch);
		if (result != simple_symbol_dictionary.end())
		{
			last_symbol = result->second;
			*input_stream >> last_ch;
			break;
		}
		if (std::isdigit(last_ch))
		{
			check_number_symbol();
			break;
		}
		if (is_legal_alpha(last_ch))
		{
			check_name_symbol();
			break;
		}
		last_symbol = SymbolType::wrong;
	}

	// 结束时要保证 last_symbol 与 last_content 被更新, 并且 last_ch 指向下一个未被添加进 last_content 的非空字符.
	read_until_not_space();
	return last_symbol;
}
