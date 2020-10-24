
#ifndef __SYNTACTIC_ANALYZE_UTIL_H__
#define __SYNTACTIC_ANALYZE_UTIL_H__

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <unordered_set>
#include "global_control.h"
#include "SymbolType.h"
#include "LexicalAnalyzer.h"
#include "SymbolToken.h"
#include "IdentifierTable.h"
#include "ErrorType.h"

using std::string;
using std::vector;
using std::shared_ptr;
using std::unique_ptr;
using std::unordered_set;
using std::ostream;
using std::multimap;
using std::pair;

class SyntacticAnalyzerEnvironment;

class Enviromentstate
{
public:
	friend class SyntacticAnalyzerEnvironment;
private:
	size_t sym_index = 0;
	size_t msg_index = 0;
	Enviromentstate(size_t sym_index, size_t msg_index)
		: sym_index(sym_index), msg_index(msg_index)
	{ }
};


class MessageEnvironment
{
protected:
	size_t msg_index = 0;
	vector<string> messages;

public:
	virtual ~MessageEnvironment() = default;
	template<typename T>
	void message_back(T value)
	{
		DEBUG_LOG_VAL(5, "syntactic-analyzer message", value);
		if (msg_index >= messages.size())
		{
			messages.emplace_back(value);
			++msg_index;
			return;
		}
		messages[msg_index++] = value;
	}
	void print_all_message(ostream& os);
};


class ErrorEnvironment
{
protected:
	multimap<int, string> errors;
public:
	virtual ~ErrorEnvironment() = default;


	void error_back(int line_number, char type)
	{
		string str;
		str.push_back(type);
		error_back(line_number, str);
	}

	void error_back(int line_number, ErrorType type)
	{
		error_back(line_number, error_code_dictionary.at(type));
	}

	template<typename T>
	void error_back(int line_number, T type)
	{
		DEBUG_LOG_VAL(5, "syntactic-analyzer error", type);
		errors.insert(pair<int, string>(line_number, type));
	}

	void print_all_error(ostream& os);
};


class TokenEnvironment : public ErrorEnvironment, public MessageEnvironment
{
public:
	using token_ptr = shared_ptr<const Token>;
	static const shared_ptr<Token> NIL;
protected:
	unique_ptr<LexicalAnalyzer> lexical_analyzer;
	vector<shared_ptr<const Token>> symbols;
	size_t sym_index = 0;

	TokenEnvironment(unique_ptr<LexicalAnalyzer> lexical_analyzer)
		: lexical_analyzer(std::move(lexical_analyzer))
	{ }

	void ensure_capacity(size_t size);
public:
	virtual ~TokenEnvironment() = default;

	SymbolType peek(size_t offset = 0)
	{
		return peek_info(offset)->type;
	}

	token_ptr peek_info(size_t offset = 0)
	{
		ensure_capacity(sym_index + offset);
		return symbols[sym_index + offset];
	}

	token_ptr dequeue()
	{
		ensure_capacity(sym_index);
		return symbols[sym_index++];
	}

	token_ptr dequeue_and_push_message()
	{
		ensure_capacity(sym_index);
		message_back(symbols[sym_index]->to_print_string());
		return symbols[sym_index++];
	}
};


class SymbolTableEnvironment
{
private:
	IdentifierTable global_table;
	IdentifierTable local_table;
	bool using_global_table = true;
public:
	virtual ~SymbolTableEnvironment() = default;
	void change_to_local_table()
	{
		using_global_table = false;
		local_table.clear();
	}

	void change_to_global_table()
	{
		using_global_table = true;
	}

	void insert_identifier(shared_ptr<const IdentifierInfo> id)
	{
		if (using_global_table)
		{
			global_table.insert_identifier(id);
		}
		else
		{
			local_table.insert_identifier(id);
		}
	}

	void remove_identifier(shared_ptr<const IdentifierInfo> id)
	{
		if (using_global_table)
		{
			global_table.remove_identifier(id);
		}
		else
		{
			local_table.remove_identifier(id);
		}
	}

	template<typename T>
	shared_ptr<const IdentifierInfo> get_identifier_info(T id, bool all = true)
	{
		shared_ptr<const IdentifierInfo> ret;
		if (!using_global_table)
		{
			try
			{
				ret = local_table.get_identifier(id);
				return ret;
			}
			catch (const std::out_of_range&)
			{
				if (!all)
				{
					return nullptr;
				}
			}
		}
		try
		{
			ret = global_table.get_identifier(id);
		}
		catch (const std::out_of_range&)
		{
			return nullptr;
		}
		return ret;
	}
};


class SyntacticAnalyzerEnvironment : public TokenEnvironment, public SymbolTableEnvironment
{
public:
	using state_t = Enviromentstate;
	friend class SyntacticAnalyzer;
private:

public:
	virtual ~SyntacticAnalyzerEnvironment() = default;

	SyntacticAnalyzerEnvironment(unique_ptr<LexicalAnalyzer> lexical_analyzer)
		: TokenEnvironment(std::move(lexical_analyzer))
	{ }

	state_t state() { return state_t(sym_index, msg_index); }

	state_t state(const state_t& state)
	{
		state_t ret(sym_index, msg_index);
		sym_index = state.sym_index;
		msg_index = state.msg_index;
		return ret;
	}
};

class syntax_exception : public std::exception
{
public:
	using symset_ptr = shared_ptr<const unordered_set<SymbolType>>;
	const SyntacticAnalyzerEnvironment::state_t current_state;
	const shared_ptr<const Token> token;
	const symset_ptr expect_symbols;
	const shared_ptr<syntax_exception> inner_exception;

	syntax_exception(const syntax_exception& cpy)
		: exception(cpy),
		current_state(cpy.current_state),
		token(cpy.token),
		expect_symbols(cpy.expect_symbols),
		inner_exception(cpy.inner_exception)
	{ }

	syntax_exception(
		SyntacticAnalyzerEnvironment::state_t current_state,
		shared_ptr<const Token> token,
		symset_ptr expect_symbols
	) :
		current_state(current_state),
		token(token),
		expect_symbols(expect_symbols),
		inner_exception()
	{ }

	syntax_exception(
		SyntacticAnalyzerEnvironment::state_t current_state,
		shared_ptr<const Token> token,
		std::initializer_list<SymbolType> expect_symbols
	) :
		current_state(current_state),
		token(token),
		expect_symbols(new unordered_set<SymbolType>(expect_symbols)),
		inner_exception()
	{ }

	syntax_exception(
		SyntacticAnalyzerEnvironment::state_t current_state,
		shared_ptr<const Token> token,
		symset_ptr expect_symbols,
		syntax_exception inner
	) :
		current_state(current_state),
		token(token),
		expect_symbols(expect_symbols),
		inner_exception(new syntax_exception(inner))
	{ }

	syntax_exception(
		SyntacticAnalyzerEnvironment::state_t current_state,
		shared_ptr<const Token> token,
		std::initializer_list<SymbolType> expect_symbols,
		syntax_exception inner
	) :
		current_state(current_state),
		token(token),
		expect_symbols(new unordered_set<SymbolType>(expect_symbols)),
		inner_exception(new syntax_exception(inner))
	{ }
};



#endif // !__SYNTACTIC_ANALYZE_UTIL_H__
