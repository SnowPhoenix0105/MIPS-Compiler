#pragma once

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
	friend void analyze_function(
		Env& env,
		shared_ptr<const string> function_id,
		BaseType return_type,
		const char* information);
	friend void analyze_inner_block(
		AbstractSyntacticAnalyzeTactics::Env& env,
		shared_ptr<const vector<shared_ptr<IdentifierInfo>>> param_list);

	virtual ~AbstractSyntacticAnalyzeTactics() = default;
	void operator()(Env & env);
private:
	static unordered_map<const std::type_info&, unordered_set<SymbolType>> first_sets;
protected:
	virtual void analyze(Env& env) = 0;

	// 完成 first_1 集的判断部分
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

	// 完成 first_n 集的判断部分, 不检查follow集
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

	static bool is_assigned_variable(Env& env)
	{
		return
			(
				env.peek(2) == SymbolType::assign
				|| (
					env.peek(2) == SymbolType::left_square
					&& (
						env.peek(5) == SymbolType::assign
						|| (
							env.peek(5) == SymbolType::left_square
							&& env.peek(8) == SymbolType::assign
							)
						)
					)
				);
	}

	template<>
	static bool in_branch_of<VariableDefinationWithInitializationAnalyze>(Env& env)
	{
		return 
			is_assigned_variable(env)
			&& in_first_set_of<VariableDefinationWithInitializationAnalyze>(env);
	}

	template<>
	static bool in_branch_of<VariableDefinationNoInitializationAnalyze>(Env& env)
	{
		return
			!is_assigned_variable(env)
			&& in_first_set_of<VariableDefinationNoInitializationAnalyze>(env);
	}

	template<>
	static bool in_branch_of<CallReturnFunctionStatementAnalyze>(Env& env)
	{
		if (!in_first_set_of<CallReturnFunctionStatementAnalyze>(env))
		{
			return false;
		}
		auto token = env.peek_info();
		auto id = dynamic_pointer_cast<IdentifierToken>(token)->id_name_content;
		auto id_info = env.get_identifier_info(id);
		return id_info->return_type->extern_type == ExternType::function
			&& id_info->return_type->base_type != BaseType::type_void;
	}

	template<>
	static bool in_branch_of<CallVoidFunctionStatementAnalyze>(Env& env)
	{
		if (!in_first_set_of<CallVoidFunctionStatementAnalyze>(env))
		{
			return false;
		}
		auto token = env.peek_info();
		auto id = dynamic_pointer_cast<IdentifierToken>(token)->id_name_content;
		auto id_info = env.get_identifier_info(id);
		return id_info->return_type->extern_type == ExternType::function
			&& id_info->return_type->base_type == BaseType::type_void;
	}

	template<>
	static bool in_branch_of<AssignmentStatementAnalyze>(Env& env)
	{
		if (!in_first_set_of<CallVoidFunctionStatementAnalyze>(env))
		{
			return false;
		}
		auto token = env.peek_info();
		auto id = dynamic_pointer_cast<IdentifierToken>(token)->id_name_content;
		auto id_info = env.get_identifier_info(id);
		return id_info->return_type->is_one_from(ExternType::variable, ExternType::l_array, ExternType::d_array)
			&& id_info->return_type->base_type != BaseType::type_void;
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

// 常量
struct ConstantAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::character, SymbolType::plus, SymbolType::minus, SymbolType::number
	};
	bool is_type_of(BaseType type);
	char get_char()
	{
		return ch;
	}
	int get_int()
	{
		return integer;
	}
protected:
	virtual void analyze(Env& env);
private:
	bool is_ch;
	char ch;
	int integer;
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

// 无符号整数
struct UnsignedIntegerAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::number
	};

	unsigned get_value()
	{
		return value;
	}
protected:
	virtual void analyze(Env& env);
private:
	unsigned value;
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

// 声明头部
struct FunctionHeaderAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_char, SymbolType::key_int
	};
	shared_ptr<const string> get_id()
	{
		return id;
	}
	// 只能是 type_int / type_char
	BaseType get_return_type()
	{
		return return_type;
	}
protected:
	virtual void analyze(Env& env);
private:
	shared_ptr<const string> id;
	BaseType return_type;
};

// 参数表
struct ParameterListAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_char, SymbolType::key_int
	};
	shared_ptr<const vector<BaseType>> get_param_type_list();
	shared_ptr<const vector<shared_ptr<IdentifierInfo>>> get_param_list()
	{
		return param_list;
	}
protected:
	virtual void analyze(Env& env);
private:
	shared_ptr<vector<shared_ptr<IdentifierInfo>>> param_list;
};

// 复合语句
struct CompoundStatementsAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_int, SymbolType::key_char,	// 常量/变量说明
		// 语句列
		SymbolType::key_while, SymbolType::key_for, // 循环语句
		SymbolType::key_if,							// 条件语句
		SymbolType::identifier,						// 有/无返回值函数调用/赋值语句
		SymbolType::key_scanf,						// 读语句
		SymbolType::key_printf,						// 写语句
		SymbolType::key_switch,						// 情况语句
		SymbolType::semicolon,						// 空语句
		SymbolType::key_return,						// 返回语句
		SymbolType::left_brance						// 语句列
	};

protected:
	virtual void analyze(Env& env);
};

// 语句列
struct StatementsListAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_while, SymbolType::key_for, // 循环语句
		SymbolType::key_if,							// 条件语句
		SymbolType::identifier,						// 有/无返回值函数调用/赋值语句
		SymbolType::key_scanf,						// 读语句
		SymbolType::key_printf,						// 写语句
		SymbolType::key_switch,						// 情况语句
		SymbolType::semicolon,						// 空语句
		SymbolType::key_return,						// 返回语句
		SymbolType::left_brance						// 语句列
	};

protected:
	virtual void analyze(Env& env);
};

// 语句
struct StatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_while, SymbolType::key_for, // 循环语句
		SymbolType::key_if,							// 条件语句
		SymbolType::identifier,						// 有/无返回值函数调用/赋值语句
		SymbolType::key_scanf,						// 读语句
		SymbolType::key_printf,						// 写语句
		SymbolType::key_switch,						// 情况语句
		SymbolType::semicolon,						// 空语句
		SymbolType::key_return,						// 返回语句
		SymbolType::left_brance						// 语句列
	};

protected:
	virtual void analyze(Env& env);
};

// 循环语句
struct LoopStatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_while, SymbolType::key_for
	};

protected:
	virtual void analyze(Env& env);
};

// 条件语句
struct ConditionStatementAnalyze: AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_if
	};

protected:
	virtual void analyze(Env& env);
};

// 有返回值函数调用语句
struct CallReturnFunctionStatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::identifier
	};

protected:
	virtual void analyze(Env& env);
};

// 无返回值函数调用语句
struct CallVoidFunctionStatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::identifier
	};

protected:
	virtual void analyze(Env& env);
};

// 赋值语句
struct AssignmentStatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::assign
	};

protected:
	virtual void analyze(Env& env);
};

// 读语句
struct ReadStatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_scanf
	};

protected:
	virtual void analyze(Env& env);
};

// 写语句
struct WriteStatementAnalyze: AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_printf
	};

protected:
	virtual void analyze(Env& env);
};

// 情况语句
struct SwitchStatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_switch
	};

protected:
	virtual void analyze(Env& env);
};

// 返回语句
struct ReturnStatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_return
	};

protected:
	virtual void analyze(Env& env);
};

// 
struct : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		// TODO
	};

protected:
	virtual void analyze(Env& env);
};

// 
struct : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		// TODO
	};

protected:
	virtual void analyze(Env& env);
};


#endif // !__ANSTRACT_SYNTACTIC_ANALYZER_TACTICS_H__

