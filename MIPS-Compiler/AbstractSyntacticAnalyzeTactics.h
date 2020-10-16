
#ifndef __ANSTRACT_SYNTACTIC_ANALYZER_TACTICS_H__
#define __ANSTRACT_SYNTACTIC_ANALYZER_TACTICS_H__

#include <unordered_map>
#include <utility>
#include "global_control.h"
#include "SyntacticAnalyzer.h"
#include "IdentifierTable.h"

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

	template<class T>
	static bool in_first_set_of(Env& env)
	{
		auto it = first_sets.find(typeid(T));
		if (it == first_sets.end())
		{
			first_sets[type_id] = unordered_set<SymbolType>(T::first_set);
			it = first_sets.find(type_id);
		}
		const auto& set = it->second;
		return set.find(env.peek()) != set.end();
	}


	template<class T>
	static bool in_branch_of(Env& env)
	{
		return in_first_set_of(typeid(T), env, T::first_set);
	}

	template<>
	static bool in_branch_of<VariableDefinationAnalyze>(Env& env)
	{
		return env.peek(2) != SymbolType::left_paren && in_first_set_of<VariableDefinationAnalyze>(env);
	}

	template<>
	static bool in_branch_of<ReturnFunctionDefinationAnalyze>(Env& env)
	{
		return env.peek(2) == SymbolType::left_paren && in_first_set_of<ReturnFunctionDefinationAnalyze>(env);
	}

	template<>
	static bool in_branch_of<VoidFunctionDefinationAnalyze>(Env& env)
	{
		return env.peek(1) != SymbolType::key_main && in_first_set_of<VoidFunctionDefinationAnalyze>(env);
	}

	template<>
	static bool in_branch_of<MainFunctionAnalyze>(Env& env)
	{
		return env.peek(1) == SymbolType::key_main && in_first_set_of<MainFunctionAnalyze>(env);
	}

	template<>
	static bool in_branch_of<VariableDefinationWithInitializationAnalyze>(Env& env)
	{
		return env.peek(2) == SymbolType::assign && in_first_set_of<VariableDefinationWithInitializationAnalyze>(env);
	}

	template<>
	static bool in_branch_of<VariableDefinationNoInitializationAnalyze>(Env& env)
	{
		return env.peek(2) != SymbolType::assign && in_first_set_of<VariableDefinationNoInitializationAnalyze>(env);
	}

	/*

	template<>
	static bool in_branch_of<>(Env& env)
	{
		return  && in_first_set_of<>(env);
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

// 变量定义及初始化
struct VariableDefinationWithInitializationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_int, SymbolType::key_char
	};

protected:
	virtual void analyze(Env& env);
};

// 变量定义无初始化
struct VariableDefinationNoInitializationAnalyze : AbstractSyntacticAnalyzeTactics
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

	int get_value()
	{
		return value;
	}
protected:
	virtual void analyze(Env& env);
private:
	int value;
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

