
#ifndef __SYNTACTIC_ANALYZE_UTIL_H__
#define __SYNTACTIC_ANALYZE_UTIL_H__

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <unordered_set>
#include <functional>
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
using std::function;
using std::initializer_list;

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


	void print_all_error(ostream& os);

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

	void error_require(int line_number, SymbolType type)
	{
		switch (type)
		{
		case SymbolType::semicolon:
			error_back(line_number, ErrorType::need_semicolon);
			break;
		case SymbolType::right_paren:
			error_back(line_number, ErrorType::need_right_paren);
			break;
		case SymbolType::right_square:
			error_back(line_number, ErrorType::need_right_square);
			break;
		default:
			error_back(line_number, ErrorType::unknown_error);
		}
	}
};

class TokenEnvironment;

struct IsType
{
	IsType(SymbolType type) : type(type) { }
	bool operator()(TokenEnvironment& env) const
	{
		return env.peek() == type;
	}
private:
	SymbolType type;
};

struct TypeInsideSet
{
	template<typename T>
	TypeInsideSet(T&& set) : type_set(std::forward(set)) { }
	bool operator()(TokenEnvironment& env) const
	{
		return type_set.count(env.peek()) != 0;
	}
private:
	unordered_set<SymbolType> type_set;
};

struct OrCondition
{
	OrCondition(const function<bool(TokenEnvironment&)>& c1, const function<bool(TokenEnvironment&)>& c2)
		: c1(c1), c2(c2)
	{ }
	bool operator()(TokenEnvironment& env) const
	{
		return c1(env) || c2(env);
	}
private:
	function<bool(TokenEnvironment&)> c1;
	function<bool(TokenEnvironment&)> c2;
};


inline function<bool(TokenEnvironment&)> wrap_condition(function<bool(TokenEnvironment&)> func)
{
	return func;
}

inline IsType wrap_condition(SymbolType type)
{
	return IsType(type);
}

inline TypeInsideSet wrap_condition(std::initializer_list<SymbolType> type_set)
{
	return TypeInsideSet(type_set);
}



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

	token_ptr dequeue_certain(SymbolType type)
	{
		ensure_capacity(sym_index);
		if (symbols[sym_index]->type == type)
		{
			return dequeue();
		}
		error_require(symbols[sym_index]->line_number, type);
	}

	token_ptr dequeue_certain_and_message_back(SymbolType type)
	{
		ensure_capacity(sym_index);
		if (symbols[sym_index]->type == type)
		{
			return dequeue_and_message_back();
		}
		error_require(symbols[sym_index]->line_number, type);
	}

	token_ptr dequeue_and_message_back()
	{
		ensure_capacity(sym_index);
		message_back(symbols[sym_index]->to_print_string());
		return symbols[sym_index++];
	}

	bool ensure(
		const function<bool(TokenEnvironment&)>& success_condition,
		const function<bool(TokenEnvironment&)>& next_condition,
		ErrorType error_type = ErrorType::unknown_error,
		unsigned max_turn = MAXINT
	);
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

	using exception::exception;

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
