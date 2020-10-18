
#ifndef __ANSTRACT_SYNTACTIC_ANALYZER_TACTICS_H__
#define __ANSTRACT_SYNTACTIC_ANALYZER_TACTICS_H__

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include "global_control.h"
#include "SyntacticAnalyzeUtil.h"
#include "IdentifierTable.h"

using std::endl;
using std::unordered_set;
using std::unordered_map;
using std::initializer_list;
using std::unordered_set;
using std::make_pair;


struct VariableDefinationAnalyze;
struct VoidFunctionDefinationAnalyze;
struct ReturnFunctionDefinationAnalyze;
struct MainFunctionAnalyze;
struct VariableDefinationWithInitializationAnalyze;
struct VariableDefinationNoInitializationAnalyze;
struct CallReturnFunctionStatementAnalyze;
struct CallVoidFunctionStatementAnalyze;
struct AssignmentStatementAnalyze;

struct FirstSetJudgement
{
	unordered_map<string, unordered_set<SymbolType>> first_sets;

	// 完成 first_1 集的判断部分
	template<class T>
	bool in_first_set_of(SyntacticAnalyzerEnvironment& SyntacticAnalyzerEnvironment)
	{
		const auto& type_id = typeid(T).name();
		auto it = first_sets.find(type_id);
		if (it == first_sets.end())
		{
			first_sets[type_id] = unordered_set<SymbolType>(T::first_set);
			it = first_sets.find(type_id);
		}
		const auto& set = it->second;
		return set.find(SyntacticAnalyzerEnvironment.peek()) != set.end();
	}

	// 完成 first_n 集的判断部分, 不检查follow集
	template<class T>
	bool in_branch_of(SyntacticAnalyzerEnvironment& SyntacticAnalyzerEnvironment)
	{
		return in_first_set_of<T>(SyntacticAnalyzerEnvironment);
	}

	template<>
	bool in_branch_of<VariableDefinationAnalyze>(SyntacticAnalyzerEnvironment& SyntacticAnalyzerEnvironment)
	{
		return SyntacticAnalyzerEnvironment.peek(2) != SymbolType::left_paren && in_first_set_of<VariableDefinationAnalyze>(SyntacticAnalyzerEnvironment);
	}

	template<>
	bool in_branch_of<ReturnFunctionDefinationAnalyze>(SyntacticAnalyzerEnvironment& SyntacticAnalyzerEnvironment)
	{
		return SyntacticAnalyzerEnvironment.peek(2) == SymbolType::left_paren && in_first_set_of<ReturnFunctionDefinationAnalyze>(SyntacticAnalyzerEnvironment);
	}

	template<>
	bool in_branch_of<VoidFunctionDefinationAnalyze>(SyntacticAnalyzerEnvironment& SyntacticAnalyzerEnvironment)
	{
		return SyntacticAnalyzerEnvironment.peek(1) != SymbolType::key_main && in_first_set_of<VoidFunctionDefinationAnalyze>(SyntacticAnalyzerEnvironment);
	}

	template<>
	bool in_branch_of<MainFunctionAnalyze>(SyntacticAnalyzerEnvironment& SyntacticAnalyzerEnvironment)
	{
		return SyntacticAnalyzerEnvironment.peek(1) == SymbolType::key_main && in_first_set_of<MainFunctionAnalyze>(SyntacticAnalyzerEnvironment);
	}

	inline bool is_assigned_variable(SyntacticAnalyzerEnvironment& SyntacticAnalyzerEnvironment)
	{
		return
			(
				SyntacticAnalyzerEnvironment.peek(2) == SymbolType::assign
				|| (
					SyntacticAnalyzerEnvironment.peek(2) == SymbolType::left_square
					&& (
						SyntacticAnalyzerEnvironment.peek(5) == SymbolType::assign
						|| (
							SyntacticAnalyzerEnvironment.peek(5) == SymbolType::left_square
							&& SyntacticAnalyzerEnvironment.peek(8) == SymbolType::assign
							)
						)
					)
				);
	}

	template<>
	bool in_branch_of<VariableDefinationWithInitializationAnalyze>(SyntacticAnalyzerEnvironment& SyntacticAnalyzerEnvironment)
	{
		return
			is_assigned_variable(SyntacticAnalyzerEnvironment)
			&& in_first_set_of<VariableDefinationWithInitializationAnalyze>(SyntacticAnalyzerEnvironment);
	}

	template<>
	bool in_branch_of<VariableDefinationNoInitializationAnalyze>(SyntacticAnalyzerEnvironment& SyntacticAnalyzerEnvironment)
	{
		return
			!is_assigned_variable(SyntacticAnalyzerEnvironment)
			&& in_first_set_of<VariableDefinationNoInitializationAnalyze>(SyntacticAnalyzerEnvironment);
	}

	template<>
	bool in_branch_of<CallReturnFunctionStatementAnalyze>(SyntacticAnalyzerEnvironment& SyntacticAnalyzerEnvironment)
	{
		if (!in_first_set_of<CallReturnFunctionStatementAnalyze>(SyntacticAnalyzerEnvironment))
		{
			return false;
		}
		auto token = SyntacticAnalyzerEnvironment.peek_info();
		auto id = dynamic_pointer_cast<const IdentifierToken>(token)->id_name_content;
		auto id_info = SyntacticAnalyzerEnvironment.get_identifier_info(id);
		return id_info->return_type->extern_type == ExternType::function
			&& id_info->return_type->base_type != BaseType::type_void;
	}

	template<>
	bool in_branch_of<CallVoidFunctionStatementAnalyze>(SyntacticAnalyzerEnvironment& SyntacticAnalyzerEnvironment)
	{
		if (!in_first_set_of<CallVoidFunctionStatementAnalyze>(SyntacticAnalyzerEnvironment))
		{
			return false;
		}
		auto token = SyntacticAnalyzerEnvironment.peek_info();
		auto id = dynamic_pointer_cast<const IdentifierToken>(token)->id_name_content;
		auto id_info = SyntacticAnalyzerEnvironment.get_identifier_info(id);
		return id_info->return_type->extern_type == ExternType::function
			&& id_info->return_type->base_type == BaseType::type_void;
	}

	template<>
	bool in_branch_of<AssignmentStatementAnalyze>(SyntacticAnalyzerEnvironment& SyntacticAnalyzerEnvironment)
	{
		if (!in_first_set_of<CallVoidFunctionStatementAnalyze>(SyntacticAnalyzerEnvironment))
		{
			return false;
		}
		auto token = SyntacticAnalyzerEnvironment.peek_info();
		auto id = dynamic_pointer_cast<const IdentifierToken>(token)->id_name_content;
		auto id_info = SyntacticAnalyzerEnvironment.get_identifier_info(id);
		return id_info->return_type->is_one_from(ExternType::variable, ExternType::l_array, ExternType::d_array)
			&& id_info->return_type->base_type != BaseType::type_void;
	}

	/*

	template<>
	bool in_branch_of<>(Env& env)
	{
		return  && in_first_set_of<>(env);
	}

	*/
};



class AbstractSyntacticAnalyzeTactics
{
public:
	using symset_ptr = syntax_exception::symset_ptr;
	using Env = SyntacticAnalyzerEnvironment;
	using state_t = SyntacticAnalyzerEnvironment::state_t;
	using token_ptr = SyntacticAnalyzerEnvironment::token_ptr;

	friend void analyze_function(
		Env& env,
		shared_ptr<const string> function_id,
		BaseType return_type,
		const char* information);
	friend void analyze_inner_block(
		AbstractSyntacticAnalyzeTactics::Env& env,
		shared_ptr<const vector<shared_ptr<IdentifierInfo>>> param_list);

	virtual ~AbstractSyntacticAnalyzeTactics() = default;
	void operator()(Env& env);
private:
	FirstSetJudgement judgment;
protected:
	virtual void analyze(Env& env) = 0;

	template<class T>
	bool in_first_set_of(Env& env)
	{
		return judgment.in_first_set_of<T>(env);
	}
	
	template<class T>
	bool in_branch_of(Env& env)
	{
		return judgment.in_branch_of<T>(env);
	}
};

// 因子
struct FactorAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::identifier,				// 标识符/一维数组/二维数组/有返回值函数调用
		SymbolType::left_paren,				// (表达式)
		SymbolType::plus, SymbolType::minus, SymbolType::number,	//整数
		SymbolType::character				// 字符
	};

protected:
	virtual void analyze(Env& env);
};

// 项
struct TermAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::identifier,				// 标识符/一维数组/二维数组/有返回值函数调用
		SymbolType::left_paren,				// (表达式)
		SymbolType::plus, SymbolType::minus, SymbolType::number,	//整数
		SymbolType::character				// 字符
	};

protected:
	virtual void analyze(Env& env);
};

// 表达式
struct ExpressionAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::identifier,				// 标识符/一维数组/二维数组/有返回值函数调用
		SymbolType::left_paren,				// (表达式)
		SymbolType::plus, SymbolType::minus, SymbolType::number,	//整数
		SymbolType::character				// 字符
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

// 缺省
struct DefaultCaseAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_default
	};

protected:
	virtual void analyze(Env& env);
};

// 情况子语句
struct CaseStatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_case
	};
	int get_case_value()
	{
		return case_value;
	}
protected:
	virtual void analyze(Env& env);
	int case_value;
};

// 情况表
struct SwitchTableAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_case
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

// 写语句
struct WriteStatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_printf
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

// 赋值语句
struct AssignmentStatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::identifier
	};

protected:
	virtual void analyze(Env& env);
};

// 值参数表
struct ParameterValueListAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::identifier,				// 标识符/一维数组/二维数组/有返回值函数调用
		SymbolType::left_paren,				// (表达式)
		SymbolType::plus, SymbolType::minus, SymbolType::number,	//整数
		SymbolType::character				// 字符
	};
	ParameterValueListAnalyze(shared_ptr<const vector<BaseType>> param_type_list)
		:param_type_list(param_type_list) { }
protected:
	virtual void analyze(Env& env);
private:
	shared_ptr<const vector<BaseType>> param_type_list;
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

// 条件
struct ConditionAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::identifier,				// 标识符/一维数组/二维数组/有返回值函数调用
		SymbolType::left_paren,				// (表达式)
		SymbolType::plus, SymbolType::minus, SymbolType::number,	//整数
		SymbolType::character				// 字符
	};

protected:
	virtual void analyze(Env& env);
};

// 条件语句
struct ConditionStatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_if
	};

protected:
	virtual void analyze(Env& env);
};

// 步长
struct StepLengthAnalyze : AbstractSyntacticAnalyzeTactics
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

/*

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


*/

#endif // !__ANSTRACT_SYNTACTIC_ANALYZER_TACTICS_H__

