#pragma once


#ifndef __SYNTACTIC_ANALYZER_H__
#define __SYNTACTIC_ANALYZER_H__

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>
#include "global_control.h"
#include "SymbolType.h"
#include "LexicalAnalyzer.h"
#include "SymbolToken.h"
#include "IdentifierTable.h"

using std::string;
using std::vector;
using std::shared_ptr;
using std::unique_ptr;
using std::unordered_set;
using std::ostream;

class Enviromentstate
{
public:
	friend class SyntacticAnalyzerEnvironment;
private:
	size_t sym_index = 0;
	size_t msg_index = 0;
};

class SyntacticAnalyzerEnvironment
{
public:
	using state_t = Enviromentstate;
	using token_ptr = shared_ptr<const Token>;
	friend class SyntacticAnalyzer;
private:
	static const shared_ptr<Token> NIL;

	unique_ptr<LexicalAnalyzer> lexical_analyzer;
	state_t state;
	vector<shared_ptr<const Token>> symbols;
	vector<string> messages;
	IdentifierTable global_table;
	IdentifierTable local_table;
	bool using_global_table = true;

	void ensure_capacity(size_t size);
public:
	SyntacticAnalyzerEnvironment(unique_ptr<LexicalAnalyzer> lexical_analyzer)
		: lexical_analyzer(std::move(lexical_analyzer))
	{ }

	state_t state() { return state; }

	state_t state(const state_t& state)
	{
		state_t old = this->state;
		this->state = state;
		return old;
	}

	SymbolType peek(size_t offset = 0)
	{
		return peek_info(offset)->type;
	}

	token_ptr peek_info(size_t offset = 0)
	{
		ensure_capacity(state.sym_index + offset);
		return symbols[state.sym_index + offset];
	}

	token_ptr dequeue()
	{
		ensure_capacity(state.sym_index);
		return symbols[state.sym_index++];
	}

	token_ptr dequeue_and_push_message()
	{
		ensure_capacity(state.sym_index);
		push_message(symbols[state.sym_index]->to_print_string());
		return symbols[state.sym_index++];
	}

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

	template<class T>
	shared_ptr<const IdentifierInfo> get_identifier_info(T id, bool all=true)
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

	template<typename T>
	void push_message(T value) 
	{
		if (state.msg_index >= messages.size())
		{
			messages.emplace_back(value);
			++state.msg_index;
		}
		messages[state.msg_index++] = value;
		return *this;
	}
};

class syntax_exception : public std::exception
{
public:
	using symset_ptr = AbstractSyntacticAnalyzeTactics::symset_ptr;
	const SyntacticAnalyzerEnvironment::state_t state;
	const shared_ptr<const Token> token;
	const symset_ptr expect_symbols;
	const shared_ptr<syntax_exception> inner_exception;

	syntax_exception(const syntax_exception& cpy)
		: exception(cpy),
		state(cpy.state),
		token(cpy.token),
		expect_symbols(cpy.expect_symbols),
		inner_exception(cpy.inner_exception)
	{ }

	syntax_exception(
		SyntacticAnalyzerEnvironment::state_t state,
		shared_ptr<const Token> token,
		symset_ptr expect_symbols
	) :
		state(state),
		token(token),
		expect_symbols(expect_symbols)
	{ }

	syntax_exception(
		SyntacticAnalyzerEnvironment::state_t state,
		shared_ptr<const Token> token,
		std::initializer_list<SymbolType> expect_symbols
	) :
		state(state),
		token(token),
		expect_symbols(new unordered_set<SymbolType>(expect_symbols))
	{ }

	syntax_exception(
		SyntacticAnalyzerEnvironment::state_t state,
		shared_ptr<const Token> token,
		symset_ptr expect_symbols,
		syntax_exception inner
	) :
		state(state),
		token(token),
		expect_symbols(expect_symbols),
		inner_exception(new syntax_exception(inner))
	{ }

	syntax_exception(
		SyntacticAnalyzerEnvironment::state_t state,
		shared_ptr<const Token> token,
		std::initializer_list<SymbolType> expect_symbols,
		syntax_exception inner
	) :
		state(state),
		token(token),
		expect_symbols(new unordered_set<SymbolType>(expect_symbols)),
		inner_exception(new syntax_exception(inner))
	{ }
};

class SyntacticAnalyzer
{
private:
	SyntacticAnalyzerEnvironment env;
	unique_ptr<ostream> os;

public:
	SyntacticAnalyzer(unique_ptr<LexicalAnalyzer> lexical_analyzer, unique_ptr<ostream> os)
		: env(std::move(lexical_analyzer)), os(std::move(os)) { }

	void parse();

	
};

#endif // !__SYNTACTIC_ANALYZER_H__



