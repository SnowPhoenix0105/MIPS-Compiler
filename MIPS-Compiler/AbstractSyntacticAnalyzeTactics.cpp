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

// 程序
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
		if (in_branch_of<ReturnFunctionDefinationAnalyze>(env))
		{
			ReturnFunctionDefinationAnalyze()(env);
		}
		else if (in_branch_of<MainFunctionAnalyze>(env))
		{
			MainFunctionAnalyze()(env);
			break;
		}
		else if (in_branch_of<VoidFunctionDefinationAnalyze>(env))
		{
			
			VoidFunctionDefinationAnalyze()(env);
		}
		else
		{
			//TODO error
		}
	}
	env.push_message("<程序>");
}

// 常量说明
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

// 常量定义
void ConstantDefinationAnalyze::analyze(Env& env)
{
	vector<shared_ptr<const IdentifierInfo>> new_ids;
	try
	{
		SymbolType type = env.peek();
		env.dequeue_and_push_message();
		while (env.peek() == SymbolType::identifier)
		{
			if (!new_ids.empty())
			{
				if (env.peek() != SymbolType::comma)
				{
					// TODO error
				}
				env.dequeue_and_push_message();
			}
			shared_ptr<const IdentifierToken> id = dynamic_pointer_cast<IdentifierToken>(env.dequeue_and_push_message());
			if (env.get_identifier_info(id->id_name_content) != nullptr)
			{
				// TODO error
			}
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
				shared_ptr<const CharToken> ch = dynamic_pointer_cast<CharToken>(env.dequeue_and_push_message());
				shared_ptr<IdentifierType> type;
				type->base_type = BaseType::type_char;
				type->extern_type = ExternType::constant;
				shared_ptr<CharactorIdentifierInfo> info = make_shared<CharactorIdentifierInfo>();
				info->id = id->id_name_content;
				info->return_type = type;
				info->value = ch->char_content;
				env.insert_identifier(info);
				new_ids.push_back(info);
			}
			else
			{
				if (!in_branch_of<IntegerAnalyze>(env))
				{
					//TODO error
				}
				IntegerAnalyze integer_analyze;
				integer_analyze(env);
				shared_ptr<IdentifierType> type;
				type->base_type = BaseType::type_int;
				type->extern_type = ExternType::constant;
				shared_ptr<IntegerIdentifierInfo> info = make_shared<IntegerIdentifierInfo>();
				info->id = id->id_name_content;
				info->return_type = type;
				info->value = integer_analyze.get_value();
				env.insert_identifier(info);
				new_ids.push_back(info);
			}
		}
		if (new_ids.empty())
		{
			// TODO error
		}
		env.push_message("<常量定义>");
	}
	catch (const syntax_exception&)
	{
		for (const auto& id : new_ids)
		{
			env.remove_identifier(id);
		}
		throw;
	}
}

// 变量说明
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
	env.push_message("<变量说明>");
}

// 变量定义
void VariableDefinationAnalyze::analyze(Env& env)
{
	if (in_branch_of<VariableDefinationWithInitializationAnalyze>(env))
	{
		VariableDefinationWithInitializationAnalyze()(env);
	}
	else if (in_branch_of<VariableDefinationNoInitializationAnalyze>(env))
	{
		VariableDefinationNoInitializationAnalyze()(env);
	}
	else
	{
		// TODO error
	}
	env.push_message("<变量定义>");
}

// 变量定义及初始化
void VariableDefinationWithInitializationAnalyze::analyze(Env& env)
{
	vector<shared_ptr<const IdentifierInfo>> new_ids;
	try
	{
		// TODO
		env.push_message("<变量定义及初始化>");
	}
	catch (const syntax_exception&)
	{
		for (const auto& id : new_ids)
		{
			env.remove_identifier(id);
		}
		throw;
	}
}

// 变量定义未初始化
void VariableDefinationNoInitializationAnalyze::analyze(Env& env)
{
	vector<shared_ptr<const IdentifierInfo>> new_ids;
	try
	{
		// TODO
		env.push_message("<变量定义无初始化>");
	}
	catch (const syntax_exception&)
	{
		for (const auto& id : new_ids)
		{
			env.remove_identifier(id);
		}
		throw;
	}
}
