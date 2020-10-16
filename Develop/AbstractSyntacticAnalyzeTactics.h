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

	/*

	template<>
	static bool in_branch_of<>(Env& env)
	{
		return  && in_first_set_of<>(env);
	}
	
	*/
};

// ����
struct ProgramAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set = 
	{ 
		SymbolType::key_const, SymbolType::key_int, SymbolType::key_char, SymbolType::key_void
	};
protected:
	void analyze(Env& env);
};

// ����˵��
struct ConstantDeclarationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =  
	{
		SymbolType::key_const
	};
protected:
	virtual void analyze(Env& env);
};

// ��������
struct ConstantDefinationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set = 
	{ 
		SymbolType::key_int, SymbolType::key_char
	};
protected:
	virtual void analyze(Env& env);
};

// ����˵��
struct VariableDeclarationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set = 
	{ 
		SymbolType::key_int, SymbolType::key_char
	};
protected:
	virtual void analyze(Env& env);
};

// ��������
struct VariableDefinationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set = 
	{
		SymbolType::key_int, SymbolType::key_char
	};

protected:
	virtual void analyze(Env& env);
};

// �������弰��ʼ��
struct VariableDefinationWithInitializationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_int, SymbolType::key_char
	};

protected:
	virtual void analyze(Env& env);
};

// ���������޳�ʼ��
struct VariableDefinationNoInitializationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_int, SymbolType::key_char
	};

protected:
	virtual void analyze(Env& env);
};

// ����
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

// ����
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

// �޷�������
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

// �޷���ֵ��������
struct VoidFunctionDefinationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_void
	};

protected:
	virtual void analyze(Env& env);
};

// �з���ֵ��������
struct ReturnFunctionDefinationAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_int, SymbolType::key_char
	};

protected:
	virtual void analyze(Env& env);
};

// ������
struct MainFunctionAnalyze : AbstractSyntacticAnalyzeTactics
{
	static constexpr std::initializer_list<SymbolType> first_set =
	{
		SymbolType::key_void
	};

protected:
	virtual void analyze(Env& env);
};

// ����ͷ��
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
	// ֻ���� type_int / type_char
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

// ������
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

// �������
struct CompoundStatements : AbstractSyntacticAnalyzeTactics
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

