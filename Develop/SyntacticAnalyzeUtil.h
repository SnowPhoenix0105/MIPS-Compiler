#pragma once

#ifndef __SYNTACTIC_ANALYZE_UTIL_H__
#define __SYNTACTIC_ANALYZE_UTIL_H__

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <unordered_set>
#include <functional>
#include <limits>
#include "global_control.h"
#include "SymbolType.h"
#include "LexicalAnalyzer.h"
#include "SymbolToken.h"
#include "IdentifierTable.h"
#include "ErrorType.h"
#include "IrTable.h"
#undef max

using std::exception;
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
using std::numeric_limits;

class SyntacticAnalyzerEnvironment;
class TokenEnvironment;

template<class Env = SyntacticAnalyzerEnvironment>
struct OrCondition
{
	OrCondition(const function<bool(Env&)>& c1, const function<bool(Env&)>& c2)
		: c1(c1), c2(c2)
	{ }

	bool operator()(Env& env) const
	{
		return c1(env) || c2(env);
	}

	template<typename T>
	OrCondition<Env> operator||(T&& other)
	{
		return OrCondition<Env>(*this, std::forward<T>(other));
	}
private:
	function<bool(Env&)> c1;
	function<bool(Env&)> c2;
};

template<class Env = SyntacticAnalyzerEnvironment>
struct IsType
{
	IsType(SymbolType type) : type(type) { }
	bool operator()(Env& env) const
	{
		return env.peek() == type;
	}

	template<typename T>
	OrCondition<Env> operator||(T&& other)
	{
		return OrCondition<Env>(*this, std::forward<T>(other));
	}
private:
	SymbolType type;
};

template<class Env = SyntacticAnalyzerEnvironment>
struct TypeInsideSet
{
	TypeInsideSet(std::initializer_list<SymbolType> set) : type_set(set) { }
	bool operator()(Env& env) const
	{
		return type_set.count(env.peek()) != 0;
	}

	template<typename T>
	OrCondition<Env> operator||(T&& other)
	{
		return OrCondition<Env>(*this, std::forward<T>(other));
	}
private:
	unordered_set<SymbolType> type_set;
};

template<class Env = SyntacticAnalyzerEnvironment>
struct SatisfyCondition
{
	SatisfyCondition(const function<bool(Env&)>& c)
		: c(c)
	{ }

	bool operator()(Env& env) const
	{
		return c(env);
	}

	template<typename T>
	OrCondition<Env> operator||(T&& other)
	{
		return OrCondition<Env>(*this, std::forward<T>(other));
	}
private:
	function<bool(Env&)> c;
};

class Enviromentstate
{
public:
	friend class SyntacticAnalyzerEnvironment;
	Enviromentstate() = default;
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
	multimap<pair<int, string>, char> errors;
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
		string t(type);
		DEBUG_LOG_VAL(5, "syntactic-analyzer error", type);
		errors.insert(pair<pair<int, string>, char>(make_pair(line_number, t), 'a'));
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



class TokenEnvironment : public ErrorEnvironment, public MessageEnvironment
{
public:
	using token_ptr = shared_ptr<const Token>;
	static const token_ptr NIL;
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

	token_ptr dequeue_certain(SymbolType type);

	token_ptr dequeue_certain_and_message_back(SymbolType type);

	token_ptr dequeue_and_message_back()
	{
		ensure_capacity(sym_index);
		message_back(symbols[sym_index]->to_print_string());
		return symbols[sym_index++];
	}

	static bool always_true(TokenEnvironment& env)
	{
		return true;
	}

	static bool always_false(TokenEnvironment& env)
	{
		return false;
	}
};


class IrCodeEnvironment
{
private:
	shared_ptr<IrElemAllocator> allocator_ptr;
	IrFactory factory;
	IrTableBuilder builder;
public:
	IrCodeEnvironment() : allocator_ptr(make_shared<IrElemAllocator>()), factory(allocator_ptr) { }
	IrElemAllocator& elem() const { return *allocator_ptr; }
	const IrFactory& ir() const { return factory; }
	IrTableBuilder& global_ir_table() { return builder; }
	shared_ptr<IrElemAllocator> get_allocator_ptr() { return allocator_ptr; }
};

class SymbolTableEnvironment : public IrCodeEnvironment
{
private:
	IdentifierTable global_table;
	IdentifierTable local_table;
	bool using_global_table = true;
public:
	BaseType current_return_type;
	int return_count = 0;
	virtual ~SymbolTableEnvironment() = default;

	bool is_using_global_table() { return using_global_table; }

	void change_to_local_table()
	{
		using_global_table = false;
		local_table.clear();
	}

	void change_to_global_table()
	{
		using_global_table = true;
	}

	irelem_t insert_identifier(shared_ptr<IdentifierInfo> id)
	{
		if (id->return_type->is_one_from(ExternType::l_array, ExternType::d_array))
		{
			shared_ptr<const ArrayIdentifierType> arr_type = dynamic_pointer_cast<const ArrayIdentifierType>(id->return_type);
			int size = arr_type->total_size();
			irelem_t arr = elem().alloc_arr(id->id);
			id->ir_id = arr;
			irelem_t type = arr_type->base_type == BaseType::type_char ? IrType::_char : IrType::_int;
			global_ir_table().push_back(ir().arr(arr, type, size));
		}
		else if (id->return_type->is_one_from(ExternType::constant))
		{
			shared_ptr<ConstantIdentifierInfo> const_id = dynamic_pointer_cast<ConstantIdentifierInfo>(id);
			irelem_t imm = elem().alloc_imm(const_id->get_value());
			id->ir_id = imm;
		}
		else if (id->return_type->is_one_from(ExternType::variable))
		{
			irelem_t var = elem().alloc_named(id->id);
			id->ir_id = var;
			if (using_global_table)
			{
				global_ir_table().push_back(ir().gvar(var));
			}
		}
		else if (id->return_type->is_one_from(ExternType::function))
		{
			shared_ptr<FunctionIdentifierInfo> func_type = dynamic_pointer_cast<FunctionIdentifierInfo>(id);
			irelem_t ret_type = func_type->return_type->base_type == BaseType::type_char ? IrType::_char : IrType::_int;
			id->ir_id = elem().alloc_func(id->id).beg();
			func_type->mid_label = elem().mid();
			func_type->end_label = elem().end();
			global_ir_table().push_back(ir().func(ret_type));
			// TODO param 声明
		}

		if (using_global_table)
		{
			global_table.insert_identifier(id);
		}
		else
		{
			local_table.insert_identifier(id);
		}
		return id->ir_id;
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

	// true 表示存在于当前作用域
	template<typename T>
	pair<shared_ptr<const IdentifierInfo>, bool> get_identifier_info(T id)
	{
		shared_ptr<const IdentifierInfo> ret;
		if (!using_global_table)
		{
			try
			{
				ret = local_table.get_identifier(id);
				return make_pair(ret, true);
			}
			catch (const std::out_of_range&) { }
		}
		try
		{
			ret = global_table.get_identifier(id);
		}
		catch (const std::out_of_range&)
		{
			return make_pair(nullptr, false);
		}
		return make_pair(ret, using_global_table);
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

	bool ensure_func(
		function<bool(SyntacticAnalyzerEnvironment&)> success_condition,
		function<bool(SyntacticAnalyzerEnvironment&)> next_condition,
		ErrorType error_type = ErrorType::unknown_error,
		unsigned max_turn = numeric_limits<unsigned>::max()
	);

	bool ensure(
		function<bool(SyntacticAnalyzerEnvironment&)> success_condition,
		function<bool(SyntacticAnalyzerEnvironment&)> next_condition,
		ErrorType error_type = ErrorType::unknown_error,
		unsigned max_turn = numeric_limits<unsigned>::max()
	)
	{
		return ensure_func(success_condition, next_condition, error_type, max_turn);
	}

	bool ensure(
		function<bool(SyntacticAnalyzerEnvironment&)> success_condition,
		initializer_list<SymbolType> next_condition,
		ErrorType error_type = ErrorType::unknown_error,
		unsigned max_turn = numeric_limits<unsigned>::max()
	)
	{
		return ensure_func(success_condition, TypeInsideSet<SyntacticAnalyzerEnvironment>(next_condition), error_type, max_turn);
	}

	bool ensure(
		initializer_list<SymbolType> success_condition,
		initializer_list<SymbolType> next_condition,
		ErrorType error_type = ErrorType::unknown_error,
		unsigned max_turn = numeric_limits<unsigned>::max()
	)
	{
		return ensure_func(TypeInsideSet<SyntacticAnalyzerEnvironment>(success_condition), TypeInsideSet<SyntacticAnalyzerEnvironment>(next_condition), error_type, max_turn);
	}

	bool ensure(
		initializer_list<SymbolType> success_condition,
		function<bool(SyntacticAnalyzerEnvironment&)> next_condition,
		ErrorType error_type = ErrorType::unknown_error,
		unsigned max_turn = numeric_limits<unsigned>::max()
	)
	{
		return ensure_func(TypeInsideSet<SyntacticAnalyzerEnvironment>(success_condition), next_condition, error_type, max_turn);
	}
};

class reach_end_exception : public exception
{
	using exception::exception;
};

class syntax_exception : public exception
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
