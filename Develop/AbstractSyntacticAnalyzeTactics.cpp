#include "AbstractSyntacticAnalyzeTactics.h"



void AbstractSyntacticAnalyzeTactics::operator()(Env& env)
{
	state_t state = env.state();
	try
	{
		this->analyze(env);
	}
	catch (const syntax_exception&)
	{
		env.state(state);
		throw;
	}
}

bool AbstractSyntacticAnalyzeTactics::in_first_set_of(const std::type_info& type_id, Env& env, const initializer_list<SymbolType>& first_set)
{
	auto it = first_sets.find(type_id);
	if (it == first_sets.end())
	{
		first_sets[type_id] = unordered_set<SymbolType>(first_set); 
		it = first_sets.find(type_id);
	}
	const auto& set = it->second;
	return set.find(env.peek()) != set.end();
}


void ProgramAnalyze::analyze(Env& env)
{
	if (in_branch_of<ConstantDeclarationAnalyze>(env))
	{
		ConstantDeclarationAnalyze()(env);
	}
	if (env.peek() == SymbolType::key_int || env.peek() == SymbolType::key_char)
	{
		VariableDeclarationAnalyze()(env);
	}
	while (true)
	{
		SymbolType type = env.peek();
		if (type == SymbolType::key_int || type == SymbolType::key_char)
		{
			ReturnFunctionDefinationAnalyze()(env);
		}
		else if (type == SymbolType::key_void)
		{
			if (env.peek(1) == SymbolType::key_main)
			{
				MainFunctionAnalyze()(env);
				break;
			}
			VoidFunctionDefinationAnalyze()(env);
		}
	}
	env.push_message("<程序>");
}

void ConstantDeclarationAnalyze::analyze(Env& env)
{
	bool flag = true;
	while (env.peek() == SymbolType::key_const)
	{
		env.dequeue_and_push_message();
		if (!in_branch_of<ConstantDefinationAnalyze>(env))
		{
			// TODO error
		}
		ConstantDefinationAnalyze()(env);
		if (env.peek() != SymbolType::semicolon)
		{
			// TODO error
		}
		env.dequeue_and_push_message();
		flag = false;
	}
	if (flag)
	{
		// TODO error
	}
	env.push_message("<常量说明>");
}

void ConstantDefinationAnalyze::analyze(Env& env)
{
	SymbolType type = env.peek();
	env.dequeue_and_push_message();
	bool flag = true;
	while (env.peek() == SymbolType::identifier)
	{
		if (!flag)
		{
			if (env.peek() != SymbolType::comma)
			{
				// TODO error
			}
			env.dequeue_and_push_message();
		}
		token_ptr id = env.dequeue_and_push_message();
		if (env.peek() != SymbolType::assign)
		{
			//TODO error
		}
		env.dequeue_and_push_message();
		if (type == SymbolType::key_char)
		{
			if (env.peek() != SymbolType::character)
			{
				// TODO error
			}
			env.dequeue_and_push_message();
		}
		else
		{
			if (!in_branch_of<IntegerAnalyze>(env))
			{
				//TODO error
			}
			IntegerAnalyze()(env);
		}
		flag = false;
	}
	if (flag)
	{
		// TODO error
	}
	env.push_message("<变量定义>");
}

void VariableDeclarationAnalyze::analyze(Env& env)
{
	bool flag = true;
	while (in_branch_of<VariableDefinationAnalyze>(env))
	{
		VariableDefinationAnalyze()(env);
		if (env.peek() != SymbolType::semicolon)
		{
			// TODO error
		}
		env.dequeue_and_push_message();
		flag = false;
	}
	if (flag)
	{
		// TODO error
	}
	env.push_message("<常量说明>");
}
