#pragma once

#ifndef __ANSTRACT_SYNTACTIC_ANALYZER_TACTICS_H__
#define __ANSTRACT_SYNTACTIC_ANALYZER_TACTICS_H__

#include <unordered_map>
#include <utility>
#include "global_control.h"
#include "SyntacticAnalyzer.h"

using std::endl;
using std::unordered_map;
using std::initializer_list;
using std::unordered_set;
using std::make_pair;

class AbstractSyntacticAnalyzeTactics
{
public:
	using Env = SyntacticAnalyzerEnvironment;
	using state_t = Env::state_t;
	using symset_ptr = shared_ptr<const unordered_set<SymbolType>>;
	using token_ptr = Env::token_ptr;
	virtual ~AbstractSyntacticAnalyzeTactics() = default;
	void operator()(Env & env);
private:
	static unordered_map<const std::type_info&, unordered_set<SymbolType>> first_sets;
protected:
	virtual void analyze(Env& env) = 0;

	static bool in_first_set_of(
		const std::type_info& type_id, 
		Env& env, 
		const initializer_list<SymbolType>& first_set
	);


	template<class T>
	static bool in_branch_of(Env& env)
	{
		return in_first_set_of(typeid(T), env, T::first_set);
	}

	template<>
	static bool in_branch_of<VariableDefinationAnalyze>(Env& env)
	{
		return env.peek(2) != SymbolType::left_paren && in_first_set_of(typeid(VariableDefinationAnalyze), env, VariableDefinationAnalyze::first_set);
	}

	template<>
	static bool in_branch_of<ReturnFunctionDefinationAnalyze>(Env& env)
	{
		return env.peek(2) == SymbolType::left_paren && in_first_set_of(typeid(ReturnFunctionDefinationAnalyze), env, ReturnFunctionDefinationAnalyze::first_set);
	}

	template<>
	static bool in_branch_of<VoidFunctionDefinationAnalyze>(Env& env)
	{
		return env.peek(1) != SymbolType::key_main && in_first_set_of(typeid(VoidFunctionDefinationAnalyze), env, VoidFunctionDefinationAnalyze::first_set);
	}

	template<>
	static bool in_branch_of<MainFunctionAnalyze>(Env& env)
	{
		return env.peek(1) == SymbolType::key_main && in_first_set_of(typeid(MainFunctionAnalyze), env, MainFunctionAnalyze::first_set);
	}

	/*

	template<>
	static bool in_branch_of<>(Env& env)
	{
		return  && in_first_set_of(typeid(), env);
	}
	
	*/
};

// 程序
struct ProgramAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set = 
	{ 
		SymbolType::key_const, SymbolType::key_int, SymbolType::key_char, SymbolType::key_void
	};
protected:
	void analyze(Env& env);
};

// 常量说明
struct ConstantDeclarationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =  
	{
		SymbolType::key_const
	};
protected:
	virtual void analyze(Env& env);
};

// 常量定义
struct ConstantDefinationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set = 
	{ 
		SymbolType::key_int, SymbolType::key_char
	};
protected:
	virtual void analyze(Env& env);
};

// 变量说明
struct VariableDeclarationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set = 
	{ 
		SymbolType::key_int, SymbolType::key_char
	};
protected:
	virtual void analyze(Env& env);
};

// 变量定义
struct VariableDefinationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set = 
	{
		SymbolType::key_int, SymbolType::key_char
	};

protected:
	virtual void analyze(Env& env);
};

// 有返回值函数定义
struct ReturnFunctionDefinationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_int, SymbolType::key_char
	};

protected:
	virtual void analyze(Env& env);
};

// 无返回值函数定义
struct VoidFunctionDefinationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_void
	};

protected:
	virtual void analyze(Env& env);
};

// 主函数
struct MainFunctionAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_void
	};

protected:
	virtual void analyze(Env& env);
};

// 整数
struct IntegerAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::plus, SymbolType::minus, SymbolType::number
	};

	static const symset_ptr first_set;
protected:
	virtual void analyze(Env& env);
};

//
struct : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		//TODO 
	};

protected:
	virtual void analyze(Env& env);
};

//
struct : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		//TODO 
	};

protected:
	virtual void analyze(Env& env);
};

//
struct : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		//TODO 
	};

protected:
	virtual void analyze(Env& env);
};

//
struct : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		//TODO 
	};

protected:
	virtual void analyze(Env& env);
};


#endif // !__ANSTRACT_SYNTACTIC_ANALYZER_TACTICS_H__

