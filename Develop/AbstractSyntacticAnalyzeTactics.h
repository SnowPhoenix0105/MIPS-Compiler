#pragma once

#ifndef __ANSTRACT_SYNTACTIC_ANALYZER_TACTICS_H__
#define __ANSTRACT_SYNTACTIC_ANALYZER_TACTICS_H__

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include "global_control.h"
#include "ErrorType.h"
#include "SyntacticAnalyzeUtil.h"
#include "IdentifierTable.h"
#include "SymbolType.h"
#include "InitializeListAnalyze.h"

using std::endl;
using std::unordered_set;
using std::unordered_map;
using std::initializer_list;
using std::unordered_set;
using std::make_pair;
using std::bad_cast;

struct VariableDeclarationAnalyze;
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
private:
	unordered_map<string, unordered_set<SymbolType>> first_sets;

public:
	// ��� first_1 �����жϲ���
	template<class T>
	bool in_first_set_of(SyntacticAnalyzerEnvironment& env)
	{
		string type_id(typeid(T).name());
		auto it = first_sets.find(type_id);
		if (it == first_sets.end())
		{
			auto s = T::first_set();
			first_sets.insert(make_pair(type_id, s));
			it = first_sets.find(type_id);
		}
		const auto& set = it->second;
		SymbolType type = env.peek();
		auto find = set.count(type);
		return find != 0;
	}

	// ��� first_n �����жϲ���, �����follow��
	template<class T>
	bool in_branch_of(SyntacticAnalyzerEnvironment& SyntacticAnalyzerEnvironment)
	{
		return in_first_set_of<T>(SyntacticAnalyzerEnvironment);
	}

	template<>
	bool in_branch_of<VariableDeclarationAnalyze>(SyntacticAnalyzerEnvironment& SyntacticAnalyzerEnvironment)
	{
		return SyntacticAnalyzerEnvironment.peek(2) != SymbolType::left_paren && in_first_set_of<VariableDeclarationAnalyze>(SyntacticAnalyzerEnvironment);
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
		bool b1 = SyntacticAnalyzerEnvironment.peek(1) != SymbolType::key_main;
		bool b2 = in_first_set_of<VoidFunctionDefinationAnalyze>(SyntacticAnalyzerEnvironment);
		return b1 && b2;
	}

	template<>
	bool in_branch_of<MainFunctionAnalyze>(SyntacticAnalyzerEnvironment& SyntacticAnalyzerEnvironment)
	{
		bool b1 = SyntacticAnalyzerEnvironment.peek(1) == SymbolType::key_main;
		bool b2 = in_first_set_of<MainFunctionAnalyze>(SyntacticAnalyzerEnvironment);
		return b1 && b2;
	}

	inline bool is_assigned_variable(SyntacticAnalyzerEnvironment& env, int offset)
	{
		if (env.peek(offset + 1) == SymbolType::assign)
		{
			return true;
		}
		if (env.peek(offset + 1) == SymbolType::left_square && env.peek(offset + 4) == SymbolType::assign)
		{
			return true;
		}
		if (env.peek(offset + 1) == SymbolType::left_square && env.peek(offset + 3) == SymbolType::assign)
		{
			return true;
		}
		if (env.peek(offset + 1) == SymbolType::left_square && env.peek(offset + 4) == SymbolType::left_square && env.peek(offset + 7) == SymbolType::assign)
		{
			return true;
		}
		if (env.peek(offset + 1) == SymbolType::left_square && env.peek(offset + 4) == SymbolType::left_square && env.peek(offset + 6) == SymbolType::assign)
		{
			return true;
		}
		if (env.peek(offset + 1) == SymbolType::left_square && env.peek(offset + 3) == SymbolType::left_square && env.peek(offset + 6) == SymbolType::assign)
		{
			return true;
		}
		if (env.peek(offset + 1) == SymbolType::left_square && env.peek(offset + 3) == SymbolType::left_square && env.peek(offset + 5) == SymbolType::assign)
		{
			return true;
		}
		return false;
	}

	template<>
	bool in_branch_of<VariableDefinationWithInitializationAnalyze>(SyntacticAnalyzerEnvironment& env)
	{
		return
			is_assigned_variable(env, 1)
			&& in_first_set_of<VariableDefinationWithInitializationAnalyze>(env);
	}

	template<>
	bool in_branch_of<VariableDefinationNoInitializationAnalyze>(SyntacticAnalyzerEnvironment& env)
	{
		return
			!is_assigned_variable(env, 1)
			&& in_first_set_of<VariableDefinationNoInitializationAnalyze>(env);
	}

	template<>
	bool in_branch_of<CallReturnFunctionStatementAnalyze>(SyntacticAnalyzerEnvironment& env)
	{
		if (!in_first_set_of<CallReturnFunctionStatementAnalyze>(env))
		{
			return false;
		}
		if (env.peek(1) != SymbolType::left_paren)
		{
			return false;
		}
		auto token = env.peek_info();
		auto id = dynamic_pointer_cast<const IdentifierToken>(token)->id_name_content;
		auto id_info = env.get_identifier_info(id);
		return id_info.first == nullptr || id_info.first->return_type->base_type != BaseType::type_void;
	}

	template<>
	bool in_branch_of<CallVoidFunctionStatementAnalyze>(SyntacticAnalyzerEnvironment& env)
	{
		if (!in_first_set_of<CallVoidFunctionStatementAnalyze>(env))
		{
			return false;
		}
		if (env.peek(1) != SymbolType::left_paren)
		{
			return false;
		}
		auto token = env.peek_info();
		auto id = dynamic_pointer_cast<const IdentifierToken>(token)->id_name_content;
		auto id_info = env.get_identifier_info(id);
		return id_info.first != nullptr && id_info.first->return_type->base_type == BaseType::type_void;
	}

	template<>
	bool in_branch_of<AssignmentStatementAnalyze>(SyntacticAnalyzerEnvironment& env)
	{
		return in_first_set_of<AssignmentStatementAnalyze>(env) && env.peek(1) != SymbolType::left_paren;
	}

	/*

	template<>
	bool in_branch_of<>(Env& env)
	{
		return  && in_first_set_of<>(env);
	}

	*/
};

class ErrorJudgment
{
public:
	using Env = SyntacticAnalyzerEnvironment;
	using token_ptr = SyntacticAnalyzerEnvironment::token_ptr;

	//void fail(Env& env, ErrorType error_type, token_ptr token, initializer_list<SymbolType> expect_symbols)
	//{
	//	env.error_back(token->line_number, error_type);
	//	throw syntax_exception(env.state(), token, expect_symbols);
	//}

	//template<SymbolType T>
	//void symbol_assert(Env& env)
	//{
	//	if (env.peek() != T)
	//	{
	//		token_ptr token = env.dequeue();
	//		env.error_back(token->line_number, ErrorType::unknown_error);
	//		throw syntax_exception(env.state(), token, { T });
	//	}
	//}

	//template<>
	//void symbol_assert<SymbolType::semicolon>(Env& env)
	//{
	//	if (env.peek() != SymbolType::semicolon)
	//	{
	//		token_ptr token = env.dequeue();
	//		env.error_back(token->line_number, ErrorType::need_semicolon);
	//		throw syntax_exception(env.state(), token, { SymbolType::semicolon });
	//	}
	//}

	//template<>
	//void symbol_assert<SymbolType::right_paren>(Env& env)
	//{
	//	if (env.peek() != SymbolType::right_paren)
	//	{
	//		token_ptr token = env.dequeue();
	//		env.error_back(token->line_number, ErrorType::need_right_paren);
	//		throw syntax_exception(env.state(), token, { SymbolType::right_paren });
	//	}
	//}

	//template<>
	//void symbol_assert<SymbolType::right_square>(Env& env)
	//{
	//	if (env.peek() != SymbolType::right_square)
	//	{
	//		token_ptr token = env.dequeue();
	//		env.error_back(token->line_number, ErrorType::need_right_square);
	//		throw syntax_exception(env.state(), token, { SymbolType::right_square });
	//	}
	//}
};



class AbstractSyntacticAnalyzeTactics
{
public:
	using symset_ptr = syntax_exception::symset_ptr;
	using Env = SyntacticAnalyzerEnvironment;
	using state_t = SyntacticAnalyzerEnvironment::state_t;
	using token_ptr = SyntacticAnalyzerEnvironment::token_ptr;

	friend int analyze_function(
		Env& env,
		int line_number,
		shared_ptr<const string> function_id,
		BaseType return_type,
		const char* information);
	friend int analyze_inner_block(
		AbstractSyntacticAnalyzeTactics::Env& env,
		shared_ptr<const vector<shared_ptr<IdentifierInfo>>> param_list);

	virtual ~AbstractSyntacticAnalyzeTactics() = default;
	bool operator()(Env& env);
	void operator()(Env& env, int sign);
private:
	static FirstSetJudgement first_set_judgment;
	static ErrorJudgment error_judgment;
protected:
	virtual void analyze(Env& env) = 0;

	template<class T>
	static bool in_first_set_of(SyntacticAnalyzerEnvironment& env)
	{
		return first_set_judgment.in_first_set_of<T>(env);
	}

	template<class T>
	static bool in_branch_of(SyntacticAnalyzerEnvironment& env)
	{
		return first_set_judgment.in_branch_of<T>(env);
	}

	//template<SymbolType T>
	//static void symbol_assert(Env& env)
	//{
	//	return error_judgment.symbol_assert<T>(env);
	//}

	//static void fail(Env& env, ErrorType error_type, token_ptr token, initializer_list<SymbolType> expect_symbols)
	//{
	//	error_judgment.fail(env, error_type, token, expect_symbols);
	//}

	//static void fail(Env& env, ErrorType error_type, token_ptr token)
	//{
	//	error_judgment.fail(env, error_type, token, {});
	//}
};

// ����
struct FactorAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::identifier,				// ��ʶ��/һά����/��ά����/�з���ֵ��������
			SymbolType::left_paren,				// (���ʽ)
			SymbolType::plus, SymbolType::minus, SymbolType::number,	//����
			SymbolType::character				// �ַ�
		};
	};
	BaseType get_type()
	{
		return type;
	}
	irelem_t get_res() { return res; }
protected:
	virtual void analyze(Env& env);
private:
	BaseType type;
	irelem_t res;
};

// ��
struct TermAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return FactorAnalyze::first_set();
	};
	BaseType get_type()
	{
		return type;
	}
	irelem_t get_res() { return res; }
protected:
	virtual void analyze(Env& env);
private:
	BaseType type;
	irelem_t res;
};

// ���ʽ
struct ExpressionAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return FactorAnalyze::first_set();
	};
	BaseType get_type()
	{
		return type;
	}
	irelem_t get_res() { return res; }
protected:
	virtual void analyze(Env& env);
private:
	BaseType type;
	irelem_t res;
};

// �������
struct ReturnStatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_return
		};
	};

protected:
	virtual void analyze(Env& env);
};

// ȱʡ
struct DefaultCaseAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_default
		};
	};

protected:
	virtual void analyze(Env& env);
};

// ��������
struct CaseStatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	CaseStatementAnalyze(BaseType type) : switch_type(type) { }
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_case
		};
	};
	int get_case_value()
	{
		return case_value;
	}
protected:
	virtual void analyze(Env& env);
	int case_value = 0;
private:
	BaseType switch_type;
};

// �����
struct SwitchTableAnalyze : AbstractSyntacticAnalyzeTactics
{
	SwitchTableAnalyze(BaseType type) : switch_type(type) { }
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_case
		};
	};
protected:
	virtual void analyze(Env& env);
private:
	BaseType switch_type;
};

// ������
struct SwitchStatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_switch
		};
	};

protected:
	virtual void analyze(Env& env);
};

// д���
struct WriteStatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_printf
		};
	};

protected:
	virtual void analyze(Env& env);
};

// �����
struct ReadStatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_scanf
		};
	};

protected:
	virtual void analyze(Env& env);
};

// ��ֵ���
struct AssignmentStatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::identifier
		};
	};

protected:
	virtual void analyze(Env& env);
};

// ֵ������
struct ParameterValueListAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::identifier,				// ��ʶ��/һά����/��ά����/�з���ֵ��������
			SymbolType::left_paren,				// (���ʽ)
			SymbolType::plus, SymbolType::minus, SymbolType::number,	//����
			SymbolType::character				// �ַ�
		};
	};
	ParameterValueListAnalyze(shared_ptr<const vector<BaseType>> param_type_list)
		:param_type_list(param_type_list) { }
protected:
	virtual void analyze(Env& env);
private:
	shared_ptr<const vector<BaseType>> param_type_list;
};

// �޷���ֵ�����������
struct CallVoidFunctionStatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::identifier
		};
	};

protected:
	virtual void analyze(Env& env);
};

// �з���ֵ�����������
struct CallReturnFunctionStatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::identifier
		};
	};
	BaseType get_type()
	{
		return type;
	}
	irelem_t get_res() { return res; }
protected:
	virtual void analyze(Env& env);
private:
	BaseType type;
	irelem_t res;
};

// ����
struct ConditionAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return ExpressionAnalyze::first_set();
	};

protected:
	virtual void analyze(Env& env);
};

// �������
struct ConditionStatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_if
		};
	};

protected:
	virtual void analyze(Env& env);
};

// ����
struct StepLengthAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::number
		};
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

// ѭ�����
struct LoopStatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_while, SymbolType::key_for
		};
	};

protected:
	virtual void analyze(Env& env);
};

// ���
struct StatementAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_while, SymbolType::key_for, // ѭ�����
			SymbolType::key_if,							// �������
			SymbolType::identifier,						// ��/�޷���ֵ��������/��ֵ���
			SymbolType::key_scanf,						// �����
			SymbolType::key_printf,						// д���
			SymbolType::key_switch,						// ������
			SymbolType::semicolon,						// �����
			SymbolType::key_return,						// �������
			SymbolType::left_brance						// �����
		};
	};

protected:
	virtual void analyze(Env& env);
};

// �����
struct StatementsListAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return StatementAnalyze::first_set();
	};

protected:
	virtual void analyze(Env& env);
};

// �������
struct CompoundStatementsAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		unordered_set<SymbolType> ret =
		{
			SymbolType::key_const, SymbolType::key_int, SymbolType::key_char,	// ����/����˵��
		};
		for (SymbolType t : StatementsListAnalyze::first_set())
		{
			ret.insert(t);
		}
		return ret;
	};

protected:
	virtual void analyze(Env& env);
};

// ������
struct ParameterListAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_char, SymbolType::key_int
		};
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

// ����ͷ��
struct FunctionHeaderAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_char, SymbolType::key_int
		};
	};
	shared_ptr<const IdentifierToken> get_token()
	{
		return token;
	}
	// ֻ���� type_int / type_char
	BaseType get_return_type()
	{
		return return_type;
	}
protected:
	virtual void analyze(Env& env);
private:
	shared_ptr<const IdentifierToken> token = nullptr;
	BaseType return_type;
};

// ������
struct MainFunctionAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_void
		};
	};
protected:
	virtual void analyze(Env& env);
};

// �з���ֵ��������
struct ReturnFunctionDefinationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_int, SymbolType::key_char
		};
	};

protected:
	virtual void analyze(Env& env);
};

// �޷���ֵ��������
struct VoidFunctionDefinationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_void
		};
	};

protected:
	virtual void analyze(Env& env);
};

// �޷�������
struct UnsignedIntegerAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::number
		};
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

// ����
struct IntegerAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		unordered_set<SymbolType> ret=
		{
			SymbolType::plus, SymbolType::minus, SymbolType::number
		};
		for (auto t : UnsignedIntegerAnalyze::first_set())
		{
			ret.insert(t);
		}
		return ret;
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

// ����
struct ConstantAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::character, SymbolType::plus, SymbolType::minus, SymbolType::number
		};
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

// ���������޳�ʼ��
struct VariableDefinationNoInitializationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_int, SymbolType::key_char
		};
	};

protected:
	virtual void analyze(Env& env);
};

// �������弰��ʼ��
struct VariableDefinationWithInitializationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_int, SymbolType::key_char
		};
	};

protected:
	shared_ptr<InitializeListElement> analyze_initialize_list(Env& env);
	virtual void analyze(Env& env);
};

// ��������
struct VariableDefinationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_int, SymbolType::key_char
		};
	};

protected:
	virtual void analyze(Env& env);
};

// ����˵��
struct VariableDeclarationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_int, SymbolType::key_char
		};
	};
protected:
	virtual void analyze(Env& env);
};

// ��������
struct ConstantDefinationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_int, SymbolType::key_char
		};
	};
protected:
	virtual void analyze(Env& env);
};

// ����˵��
struct ConstantDeclarationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_const
		};
	};
protected:
	virtual void analyze(Env& env);
};

// ����
struct ProgramAnalyze : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			SymbolType::key_const, SymbolType::key_int, SymbolType::key_char, SymbolType::key_void
		};
	};
protected:
	void analyze(Env& env);
};

/*

//
struct : AbstractSyntacticAnalyzeTactics
{
	static unordered_set<SymbolType> first_set()
	{
		return
		{
			// TODO
		};
	};

protected:
	virtual void analyze(Env& env);
};


*/

#endif // !__ANSTRACT_SYNTACTIC_ANALYZER_TACTICS_H__

