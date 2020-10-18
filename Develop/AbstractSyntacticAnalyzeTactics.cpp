#include "AbstractSyntacticAnalyzeTactics.h"



void AbstractSyntacticAnalyzeTactics::operator()(Env& env)
{
	state_t current_state = env.state();
	try
	{
		this->analyze(env);
	}
	catch (const syntax_exception&)
	{
		env.state(current_state);
		throw;
	}
}

// 程序
void ProgramAnalyze::analyze(Env& env)
{
	if (in_branch_of<ConstantDeclarationAnalyze>(env))
	{
		ConstantDeclarationAnalyze()(env);							// 常量说明
	}
	if (in_branch_of<VariableDeclarationAnalyze>(env))
	{
		VariableDeclarationAnalyze()(env);							// 变量说明
	}
	while (true)
	{
		SymbolType type = env.peek();
		if (in_branch_of<ReturnFunctionDefinationAnalyze>(env))
		{
			ReturnFunctionDefinationAnalyze()(env);					// 有返回值函数定义
		}
		else if (in_branch_of<MainFunctionAnalyze>(env))
		{
			MainFunctionAnalyze()(env);								// 主函数
			break;
		}
		else if (in_branch_of<VoidFunctionDefinationAnalyze>(env))
		{
			
			VoidFunctionDefinationAnalyze()(env);					// 无返回值函数定义
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
		env.dequeue_and_push_message();							// key_const
		if (!in_branch_of<ConstantDefinationAnalyze>(env))
		{
			// TODO error
		}
		ConstantDefinationAnalyze()(env);						// 常量定义
		if (env.peek() != SymbolType::semicolon)
		{
			// TODO error
		}
		env.dequeue_and_push_message();							// semicolon
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
		env.dequeue_and_push_message();							// key_int / key_char
		while (true)
		{
			if (!new_ids.empty())
			{
				if (env.peek() != SymbolType::comma)
				{
					break;
				}
				env.dequeue_and_push_message();					// comma
			}
			if (env.peek() != SymbolType::identifier)
			{
				// TODO error
			}
			auto token = env.dequeue_and_push_message();		// identifier
			shared_ptr<const IdentifierToken> id = dynamic_pointer_cast<const IdentifierToken>(token);
			if (env.get_identifier_info(id->id_name_content, false) != nullptr)
			{
				// TODO error
			}
			if (env.peek() != SymbolType::assign)
			{
				//TODO error
			}
			env.dequeue_and_push_message();						// assign
			shared_ptr<IdentifierInfo> id_info;
			shared_ptr<IdentifierType> id_type = make_shared<IdentifierType>();
			id_type->extern_type = ExternType::constant;
			if (type == SymbolType::key_char)
			{
				id_type->base_type = BaseType::type_char;
				if (env.peek() != SymbolType::character)
				{
					// TODO error
				}
				auto tk = env.dequeue_and_push_message();		// charactor
				shared_ptr<CharactorIdentifierInfo> info = make_shared<CharactorIdentifierInfo>();
				info->value = dynamic_pointer_cast<const CharToken>(tk)->char_content;
				id_info = info;
			}
			else
			{
				id_type->base_type = BaseType::type_int;
				if (!in_branch_of<IntegerAnalyze>(env))
				{
					//TODO error
				}
				IntegerAnalyze integer_analyze;					// 整数
				integer_analyze(env);
				shared_ptr<IntegerIdentifierInfo> info = make_shared<IntegerIdentifierInfo>();
				info->value = integer_analyze.get_value();
				id_info = info;
			}
			id_info->id = id->id_name_content;
			id_info->return_type = id_type;
			new_ids.push_back(id_info);
			env.insert_identifier(id_info);
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
		VariableDefinationAnalyze()(env);			// 变量定义
		if (env.peek() != SymbolType::semicolon)
		{
			// TODO error
		}
		env.dequeue_and_push_message();				// semicolon
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
		VariableDefinationWithInitializationAnalyze()(env);		// 变量定义及初始化
	}
	else if (in_branch_of<VariableDefinationNoInitializationAnalyze>(env))
	{
		VariableDefinationNoInitializationAnalyze()(env);		// 变量定义无初始化
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
		SymbolType type = env.peek();
		BaseType base_type = type == SymbolType::key_int ? BaseType::type_int : BaseType::type_char;
		env.dequeue_and_push_message();									// key_int / key_char
		while (true)
		{
			if (!new_ids.empty())
			{
				if (env.peek() != SymbolType::comma)
				{
					break;
				}
				env.dequeue_and_push_message();							// comma
			}
			if (env.peek() != SymbolType::identifier)
			{
				// TODO error
			}
			auto token = env.dequeue_and_push_message();				// identifier
			shared_ptr<const IdentifierToken> id = dynamic_pointer_cast<const IdentifierToken>(token);
			if (env.get_identifier_info(id->id_name_content, false) != nullptr)
			{
				// TODO error
			}
			shared_ptr<IdentifierType> return_type;
			ConstantAnalyze constant_analyze;
			if (env.peek() == SymbolType::left_square)
			{
				// 数组
				if (env.peek(3) == SymbolType::left_square)
				{
					// 二维数组
					env.dequeue_and_push_message();						// left_square
					UnsignedIntegerAnalyze size_1_unsigned_analyze;			// unsigned_integer
					size_1_unsigned_analyze(env);
					unsigned size_1 = size_1_unsigned_analyze.get_value();
					if (env.peek() != SymbolType::right_square)
					{
						//TODO error
					}
					env.dequeue_and_push_message();						// right_square
					env.dequeue_and_push_message();						// left_square
					UnsignedIntegerAnalyze unsigned_analyze;			// unsigned_integer
					unsigned_analyze(env);
					unsigned size_2 = unsigned_analyze.get_value();
					if (env.peek() != SymbolType::right_square)
					{
						//TODO error
					}
					env.dequeue_and_push_message();						// right_square
					env.dequeue_and_push_message();						// assigin
					if (env.peek() != SymbolType::left_brance)
					{
						// TODO error
					}
					env.dequeue_and_push_message();						// left_brance
					for (int i = 0; i != size_1; ++i)
					{
						if (i != 0 && env.peek() != SymbolType::comma)
						{
							// TODO error
						}
						env.dequeue_and_push_message();					// comma
						if (env.peek() != SymbolType::left_brance)
						{
							// TODO error
						}
						env.dequeue_and_push_message();						// left_brance

						for (int j = 0; j != size_2; ++j)
						{
							if (j != 0 && env.peek() != SymbolType::comma)
							{
								// TODO error
							}
							env.dequeue_and_push_message();					// comma;
							if (!in_branch_of<ConstantAnalyze>(env))
							{
								// TODO error
							}
							constant_analyze(env);							// 常量
							if (!constant_analyze.is_type_of(base_type))
							{
								// TODO error
							}
							// TODO 赋值
						}

						if (env.peek() != SymbolType::right_brance)
						{
							// TODO error
						}
						env.dequeue_and_push_message();						// right_brance
					}
					if (env.peek() != SymbolType::right_brance)
					{
						// TODO error
					}
					env.dequeue_and_push_message();						// right_brance

					// 插入符号表
					shared_ptr<DoubleDimensionalArrayIdentifierType> id_type = make_shared<DoubleDimensionalArrayIdentifierType>();
					id_type->extern_type = ExternType::d_array;
					id_type->size_1 = size_1;
					id_type->size_2 = size_2;
					return_type = id_type;
				}
				else
				{
					// 一维数组
					env.dequeue_and_push_message();						// left_square
					UnsignedIntegerAnalyze size_2_unsigned_analyze;			// unsigned_integer
					size_2_unsigned_analyze(env);
					unsigned size = size_2_unsigned_analyze.get_value();
					if (env.peek() != SymbolType::right_square)
					{
						//TODO error
					}
					env.dequeue_and_push_message();						// right_square
					env.dequeue_and_push_message();						// assigin
					if (env.peek() != SymbolType::left_brance)
					{
						// TODO error
					}
					env.dequeue_and_push_message();						// left_brance
					for (int i = 0; i != size; ++i)
					{
						if (i != 0 && env.peek() != SymbolType::comma)
						{
							// TODO error
						}
						env.dequeue_and_push_message();					// comma
						if (!in_branch_of<ConstantAnalyze>(env))
						{
							// TODO error
						}
						constant_analyze(env);							// 常量
						if (!constant_analyze.is_type_of(base_type))
						{
							// TODO error
						}
						// TODO 赋值
					}
					if (env.peek() != SymbolType::right_brance)
					{
						// TODO error
					}
					env.dequeue_and_push_message();						// right_brance

					// 插入符号表
					shared_ptr<LinearArrayIdentifierType> id_type = make_shared<LinearArrayIdentifierType>();
					id_type->extern_type = ExternType::l_array;
					id_type->size = size;
					return_type = id_type;
				}
			}
			else
			{
				// 普通变量
				env.dequeue_and_push_message();								// assign
				if (!in_branch_of<ConstantAnalyze>(env))
				{
					// TODO error
				}
				constant_analyze(env);										// 常量
				if (!constant_analyze.is_type_of(base_type))
				{
					// TODO error
				}
				// TODO 赋值

				// 插入符号表
				shared_ptr<IdentifierType> id_type = make_shared<IdentifierType>();
				id_type->extern_type = ExternType::variable;
				return_type = id_type;
			}
			shared_ptr<IdentifierInfo> info = make_shared<IdentifierInfo>();
			return_type->base_type = base_type;
			info->id = id->id_name_content;
			info->return_type = return_type;
			new_ids.push_back(info);
			env.insert_identifier(info);
		}
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

// 变量定义无初始化
void VariableDefinationNoInitializationAnalyze::analyze(Env& env)
{
	vector<shared_ptr<const IdentifierInfo>> new_ids;
	try
	{
		SymbolType type = env.peek();
		BaseType base_type = type == SymbolType::key_int ? BaseType::type_int : BaseType::type_char;
		env.dequeue_and_push_message();									// key_int / key_char
		while (true)
		{
			if (!new_ids.empty())
			{
				if (env.peek() != SymbolType::comma)
				{
					break;
				}
				env.dequeue_and_push_message();							// comma
			}
			if (env.peek() != SymbolType::identifier)
			{
				// TODO error
			}
			auto token = env.dequeue_and_push_message();				// identifier
			shared_ptr<const IdentifierToken> id = dynamic_pointer_cast<const IdentifierToken>(token);
			if (env.get_identifier_info(id->id_name_content, false) != nullptr)
			{
				// TODO error
			}
			shared_ptr<IdentifierType> return_type;
			if (env.peek() == SymbolType::left_square)
			{
				// 数组
				if (env.peek(3) == SymbolType::left_square)
				{
					// 二维数组
					env.dequeue_and_push_message();						// left_square
					UnsignedIntegerAnalyze size_1_unsigned_analyze;			// unsigned_integer
					size_1_unsigned_analyze(env);
					unsigned size_1 = size_1_unsigned_analyze.get_value();
					if (env.peek() != SymbolType::right_square)
					{
						//TODO error
					}
					env.dequeue_and_push_message();						// right_square
					env.dequeue_and_push_message();						// left_square
					UnsignedIntegerAnalyze size_2_unsigned_analyze;			// unsigned_integer
					size_2_unsigned_analyze(env);
					unsigned size_2 = size_2_unsigned_analyze.get_value();
					if (env.peek() != SymbolType::right_square)
					{
						//TODO error
					}
					env.dequeue_and_push_message();						// right_square

					// 插入符号表
					shared_ptr<DoubleDimensionalArrayIdentifierType> id_type = make_shared<DoubleDimensionalArrayIdentifierType>();
					id_type->extern_type = ExternType::d_array;
					id_type->size_1 = size_1;
					id_type->size_2 = size_2;
					return_type = id_type;
				}
				else
				{
					// 一维数组
					env.dequeue_and_push_message();						// left_square
					UnsignedIntegerAnalyze unsigned_analyze;			// unsigned_integer
					unsigned_analyze(env);
					unsigned size = unsigned_analyze.get_value();
					if (env.peek() != SymbolType::right_square)
					{
						//TODO error
					}
					env.dequeue_and_push_message();						// right_square

					// 插入符号表
					shared_ptr<LinearArrayIdentifierType> id_type = make_shared<LinearArrayIdentifierType>();
					id_type->extern_type = ExternType::l_array;
					id_type->size = size;
					return_type = id_type;
				}
			}
			else
			{
				// 普通变量

				// 插入符号表
				shared_ptr<IdentifierType> id_type = make_shared<IdentifierType>();
				id_type->base_type = type == SymbolType::key_int ? BaseType::type_int : BaseType::type_char;
				id_type->extern_type = ExternType::variable;
				return_type = id_type;
			}
			shared_ptr<IdentifierInfo> info = make_shared<IdentifierInfo>();
			return_type->base_type = base_type;
			info->id = id->id_name_content;
			info->return_type = return_type;
			new_ids.push_back(info);
			env.insert_identifier(info);
		}
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

bool ConstantAnalyze::is_type_of(BaseType type)
{
	if (is_ch)
	{
		return type == BaseType::type_char;
	}
	else
	{
		return type == BaseType::type_int;
	}
}

// 常量
void ConstantAnalyze::analyze(Env& env)
{
	if (env.peek() == SymbolType::character)
	{
		is_ch = true;
		auto token = env.dequeue_and_push_message();	// charactor
		ch = dynamic_pointer_cast<const CharToken>(token)->char_content;
	}
	else
	{
		is_ch = false;
		IntegerAnalyze integer_analyze;
		integer_analyze(env);							// 整数
		integer = integer_analyze.get_value();			
	}
	env.push_message("<常量>");
}

// 整数
void IntegerAnalyze::analyze(Env& env)
{
	bool need_negative = false;
	switch (env.peek())
	{
	case SymbolType::minus:
		need_negative = true;
	case SymbolType::plus:
		env.dequeue_and_push_message();							// +/-
	default:					
		if (!in_branch_of<UnsignedIntegerAnalyze>(env))
		{
			// TODO error
		}
		UnsignedIntegerAnalyze unsigned_integer_analyzer;		// 无符号整数
		value = unsigned_integer_analyzer.get_value();
		if (need_negative)
		{
			value = -value;
		}
	}
	env.push_message("<整数>");
}

// 无符号整数
void UnsignedIntegerAnalyze::analyze(Env& env)
{
	auto token = env.dequeue_and_push_message();		// number
	value = dynamic_pointer_cast<const UnsignedToken>(token)->unsigned_content;
	env.push_message("<无符号整数>");
}

void analyze_inner_block(
	AbstractSyntacticAnalyzeTactics::Env& env, 
	shared_ptr<const vector<shared_ptr<IdentifierInfo>>> param_list)
{
	env.change_to_local_table();

	for (auto param : *param_list)
	{
		env.insert_identifier(param);
	}

	if (env.peek() != SymbolType::left_brance)
	{
		// TODO error
	}
	env.dequeue_and_push_message();						// left_brance

	if (env.peek() != SymbolType::left_brance
		&& !AbstractSyntacticAnalyzeTactics::in_branch_of<CompoundStatementsAnalyze>(env))
	{
		// TODO error
	}
	CompoundStatementsAnalyze()(env);							// 复合语句

	env.change_to_global_table();

	if (env.peek() != SymbolType::right_brance)
	{
		// TODO error
	}
	env.dequeue_and_push_message();						// right_brance
}

void analyze_function(
	AbstractSyntacticAnalyzeTactics::Env& env,
	shared_ptr<const string> function_id, 
	BaseType return_type,
	const char* information)
{
	if (env.get_identifier_info(function_id, false) != nullptr)
	{
		// TODO error
	}
	if (env.peek() != SymbolType::left_paren)
	{
		// TODO error
	}
	env.dequeue_and_push_message();						// left_paren

	if (!AbstractSyntacticAnalyzeTactics::in_branch_of<ParameterListAnalyze>(env) 
		&& env.peek() != SymbolType::right_paren)
	{
		// TODO error
	}
	ParameterListAnalyze parameter_list_analyze;
	parameter_list_analyze(env);						// 参数表
	shared_ptr<const vector<BaseType>> param_type_list = parameter_list_analyze.get_param_type_list();
	auto param_list = parameter_list_analyze.get_param_list();

	if (env.peek() != SymbolType::right_paren)
	{
		// TODO error
	}
	env.dequeue_and_push_message();						// right_paren

	// 插入符号表
	shared_ptr<FuctionIdentifierType> id_type = make_shared<FuctionIdentifierType>();
	id_type->base_type = return_type;
	id_type->extern_type = ExternType::function;
	id_type->param_type_list = param_type_list;
	shared_ptr<IdentifierInfo> function_info = make_shared<IdentifierInfo>();
	function_info->return_type = id_type;
	function_info->id = function_id;

	env.insert_identifier(function_info);
	try
	{
		analyze_inner_block(env, param_list);
		env.push_message(information);
	}
	catch (const syntax_exception&)
	{
		env.remove_identifier(function_info);
		throw;
	}
}

// 有返回值函数定义
void ReturnFunctionDefinationAnalyze::analyze(Env& env)
{
	FunctionHeaderAnalyze function_header_analyze;
	function_header_analyze(env);						// 声明头部
	shared_ptr<const string> function_id = function_header_analyze.get_id();
	BaseType return_type = function_header_analyze.get_return_type();

	analyze_function(env, function_id, return_type, "<有返回值函数定义>");
}

// 无返回值函数定义
void VoidFunctionDefinationAnalyze::analyze(Env& env)
{
	env.dequeue_and_push_message();						// key_void

	if (env.peek() != SymbolType::identifier)
	{
		// TODO error
	}
	auto token = env.dequeue_and_push_message();		// identifier
	shared_ptr<const string> function_id = dynamic_pointer_cast<const IdentifierToken>(token)->id_name_content;

	analyze_function(env, function_id, BaseType::type_void, "<无返回值函数定义>");
}

// 主函数
void MainFunctionAnalyze::analyze(Env& env)
{
	env.dequeue_and_push_message();					// key_void;
	env.dequeue_and_push_message();					// key_main;
	if (env.peek() != SymbolType::left_paren)
	{
		// TODO error
	}
	env.dequeue_and_push_message();					// left_paren
	if (env.peek() != SymbolType::right_paren)
	{
		// TODO error
	}
	env.dequeue_and_push_message();					// right_paren
	analyze_inner_block(env, make_shared<const vector<shared_ptr<IdentifierInfo>>>());
	env.push_message("<主函数>");
}

// 声明头部
void FunctionHeaderAnalyze::analyze(Env& env)
{
	env.dequeue_and_push_message();						// key_char / key_int
	return_type = env.peek() == SymbolType::key_char ? BaseType::type_char : BaseType::type_int;
	if (env.peek() != SymbolType::identifier)
	{
		// TODO error
	}
	auto token = env.dequeue_and_push_message();		// identifier
	id = dynamic_pointer_cast<const IdentifierToken>(token)->id_name_content;
	env.push_message("<声明头部>");
}

shared_ptr<const vector<BaseType>> ParameterListAnalyze::get_param_type_list()
{
	shared_ptr<vector<BaseType>> ret = make_shared<vector<BaseType>>();
	for (auto param : *param_list)
	{
		ret->push_back(param->return_type->base_type);
	}
	return ret;
}

// 参数表
void ParameterListAnalyze::analyze(Env& env)
{
	bool flag = false;
	param_list = make_shared<vector<shared_ptr<IdentifierInfo>>>();
	while (true)
	{
		if (flag)
		{
			if (env.peek() != SymbolType::comma)
			{
				break;
			}
			env.dequeue_and_push_message();							// comma
		}
		if (env.peek() != SymbolType::key_int && env.peek() != SymbolType::key_char)
		{
			if (!flag)
			{
				break;
			}
			// TODO error
		}
		auto type_token = env.dequeue_and_push_message();			// key_char / key_int
		if (env.peek() != SymbolType::identifier)
		{
			// TODO error
		}
		auto id_token = env.dequeue_and_push_message();				// identifier

		shared_ptr<IdentifierType> id_type = make_shared<IdentifierType>();
		id_type->base_type = type_token->type == SymbolType::key_char ? BaseType::type_char : BaseType::type_int;
		id_type->extern_type = ExternType::variable;
		shared_ptr<IdentifierInfo> id_info = make_shared<IdentifierInfo>();
		id_info->id = dynamic_pointer_cast<const IdentifierToken>(id_token)->id_name_content;
		id_info->return_type = id_type;

		param_list->push_back(id_info);
		flag = true;
	}
	env.push_message("<参数表>");
}

// 复合语句
void CompoundStatementsAnalyze::analyze(Env& env)
{
	if (in_branch_of<ConstantDeclarationAnalyze>(env))
	{
		ConstantDeclarationAnalyze()(env);							// 常量说明
	}
	if (in_branch_of<VariableDeclarationAnalyze>(env))
	{
		VariableDeclarationAnalyze()(env);							// 变量说明
	}
	StatementsListAnalyze()(env);											// 语句列
	env.push_message("<复合语句>");
}

// 语句列
void StatementsListAnalyze::analyze(Env& env)
{
	while (in_branch_of<StatementAnalyze>(env))
	{
		StatementAnalyze()(env);									// 语句
	}
	env.push_message("<语句列>");
}

// 语句
void StatementAnalyze::analyze(Env& env)
{
	switch (env.peek())
	{
	case SymbolType::key_while:
	case SymbolType::key_for:
		LoopStatementAnalyze()(env);									// 循环语句
		break;
	case SymbolType::key_if:
		ConditionStatementAnalyze()(env);								// 条件语句
		break;
	case SymbolType::identifier:
		if (in_branch_of<AssignmentStatementAnalyze>(env))
		{
			AssignmentStatementAnalyze()(env);							// 赋值语句
		}
		else if (in_branch_of<CallReturnFunctionStatementAnalyze>(env))
		{
			CallReturnFunctionStatementAnalyze()(env);					// 有返回值函数调用语句
		}
		else // if (in_branch_of<CallVoidFunctionStatementAnalyze>(env))
		{
			CallVoidFunctionStatementAnalyze()(env);					// 无返回值调用语句
		}
		if (env.peek() != SymbolType::semicolon)
		{
			// TODO error
		}
		env.dequeue_and_push_message();									// semicolon
		break;
	case SymbolType::key_scanf:
		ReadStatementAnalyze()(env);									// 读语句
		if (env.peek() != SymbolType::semicolon)
		{
			// TODO error
		}
		env.dequeue_and_push_message();									// semicolon
		break;
	case SymbolType::key_printf:
		WriteStatementAnalyze()(env);									// 写语句
		if (env.peek() != SymbolType::semicolon)
		{
			// TODO error
		}
		env.dequeue_and_push_message();									// semicolon
		break;
	case SymbolType::key_switch:
		SwitchStatementAnalyze()(env);									// 情况语句
		break;
	case SymbolType::key_return:
		ReturnStatementAnalyze()(env);									// 返回语句
		if (env.peek() != SymbolType::semicolon)
		{
			// TODO error
		}
		env.dequeue_and_push_message();									// semicolon
		break;
	case SymbolType::left_brance:
		env.dequeue_and_push_message();									// left_brance
		StatementsListAnalyze()(env);										// 语句列
		if (env.peek() != SymbolType::right_brance)
		{
			// TODO error
		}
		env.dequeue_and_push_message();									// right_brance
		break;
	case SymbolType::semicolon:
		env.dequeue_and_push_message();									// semicolon
		break;
	default:
		// TODO error
		break;
	}

	env.push_message("<语句>");

	//if (in_branch_of<LoopStatementAnalyze>(env))
	//{
	//	LoopStatementAnalyze()(env);								// 循环语句
	//}
	//else if (in_branch_of<ConditionStatementAnalyze>(env))
	//{
	//	ConditionStatementAnalyze()(env);							// 条件语句
	//}
	//else if (in_branch_of<ReadStatementAnalyze>(env))
	//{
	//	ReadStatementAnalyze()(env);								// 读语句
	//	if (env.peek() != SymbolType::semicolon)
	//	{
	//		// TODO error
	//	}
	//	env.dequeue_and_push_message();								// semicolon
	//}
	//else if (in_branch_of<WriteStatementAnalyze>(env))
	//{
	//	WriteStatementAnalyze()(env);								// 写语句
	//	if (env.peek() != SymbolType::semicolon)
	//	{
	//		// TODO error
	//	}
	//	env.dequeue_and_push_message();								// semicolon
	//}
	//else if (in_branch_of<SwitchStatementAnalyze>(env))
	//{
	//	SwitchStatementAnalyze()(env);								// 情况语句
	//}
	//else if (in_branch_of<ReturnStatementAnalyze>(env))
	//{
	//	ReturnStatementAnalyze()(env);								// 返回语句
	//	if (env.peek() != SymbolType::semicolon)
	//	{
	//		// TODO error
	//	}
	//	env.dequeue_and_push_message();								// semicolon
	//}
	//else if (in_branch_of<AssignmentStatementAnalyze>(env))
	//{
	//	AssignmentStatementAnalyze()(env);							// 赋值语句
	//	if (env.peek() != SymbolType::semicolon)
	//	{
	//		// TODO error
	//	}
	//	env.dequeue_and_push_message();								// semicolon
	//}
	//else if (in_branch_of<CallReturnFunctionStatementAnalyze>(env))
	//{
	//	CallReturnFunctionStatementAnalyze()(env);					// 有返回值函数调用语句
	//	if (env.peek() != SymbolType::semicolon)
	//	{
	//		// TODO error
	//	}
	//	env.dequeue_and_push_message();								// semicolon
	//}
	//else if (in_branch_of<CallVoidFunctionStatementAnalyze>(env))
	//{
	//	CallVoidFunctionStatementAnalyze()(env);					// 无返回值调用语句
	//	if (env.peek() != SymbolType::semicolon)
	//	{
	//		// TODO error
	//	}
	//	env.dequeue_and_push_message();								// semicolon
	//}
	//else if (env.peek() == SymbolType::left_brance)
	//{
	//	env.dequeue_and_push_message();								// left_brance
	//	StatementAnalyze()(env);									// 语句
	//	if (env.peek() != SymbolType::right_brance)
	//	{
	//		// TODO error
	//	}
	//	env.dequeue_and_push_message();
	//}
	//else
	//{
	//	if (env.peek() != SymbolType::semicolon)
	//	{
	//		// TODO error
	//	}
	//	env.dequeue_and_push_message();								// semicolon
	//}
	//env.push_message("<语句>");
}

// 循环语句
void LoopStatementAnalyze::analyze(Env& env)
{
	if (env.peek() == SymbolType::key_while)
	{
		env.dequeue_and_push_message();							// key_while
		if (env.peek() != SymbolType::left_paren)
		{
			// TODO error
		}
		env.dequeue_and_push_message();							// left_paren

		if (!in_branch_of<ConditionAnalyze>(env))
		{
			// TODO error
		}
		ConditionAnalyze()(env);								// 条件

		if (env.peek() != SymbolType::right_paren)
		{
			// TODO error
		}
		env.dequeue_and_push_message();							// right_paren
		if (!in_branch_of<StatementAnalyze>(env))
		{
			// TODO error
		}
		StatementAnalyze()(env);									// 语句
	}
	else
	{
		env.dequeue_and_push_message();							// key_for
		if (env.peek() != SymbolType::left_paren)
		{
			// TODO error
		}
		env.dequeue_and_push_message();							// left_paren

		if (env.peek() != SymbolType::identifier)
		{
			// TODO error
		}
		auto init_token = env.dequeue_and_push_message();			// identifier
		auto init_id_token = dynamic_pointer_cast<const IdentifierToken>(init_token);
		auto init_id_info = env.get_identifier_info(init_id_token->id_name_content);
		if (init_id_info == nullptr)
		{
			// TODO error
		}
		if (env.peek() != SymbolType::assign)
		{
			// TODO error
		}
		env.dequeue_and_push_message();							// assign
		if (!in_branch_of<ExpressionAnalyze>(env))
		{
			// TODO error
		}
		ExpressionAnalyze()(env);								// 表达式
		if (env.peek() != SymbolType::semicolon)
		{
			// TODO error
		}
		env.dequeue_and_push_message();							// semicolon

		if (!in_branch_of<ConditionAnalyze>(env))
		{
			// TODO error
		}
		ConditionAnalyze()(env);								// 条件
		if (env.peek() != SymbolType::semicolon)
		{
			// TODO error
		}
		env.dequeue_and_push_message();							// semicolon

		if (env.peek() != SymbolType::identifier)
		{
			// TODO error
		}
		auto delta_left_token = env.dequeue_and_push_message();		// identifier
		auto delta_left_id_token = dynamic_pointer_cast<const IdentifierToken>(delta_left_token);
		auto delta_left_id_info = env.get_identifier_info(delta_left_id_token->id_name_content);
		if (delta_left_id_info == nullptr)
		{
			// TOTO error
		}
		if (env.peek() != SymbolType::assign)
		{
			// TODO error
		}
		env.dequeue_and_push_message();							// assign
		if (env.peek() != SymbolType::identifier)
		{
			// TODO error
		}
		auto delta_right_token = env.dequeue_and_push_message();	// identifier
		auto delta_right_id_token = dynamic_pointer_cast<const IdentifierToken>(delta_right_token);
		auto delta_right_id_info = env.get_identifier_info(delta_right_id_token->id_name_content);
		if (delta_right_id_info == nullptr)
		{
			// TOTO error
		}
		SymbolType delta_type = env.peek();
		if (delta_type != SymbolType::plus && delta_type != SymbolType::minus)
		{
			// TODO error
		}
		env.dequeue_and_push_message();							// +/-
		if (!in_branch_of<StepLengthAnalyze>(env))
		{
			// TODO error
		}
		StepLengthAnalyze step_length_analyze;
		step_length_analyze(env);								// 步长
		unsigned step_length = step_length_analyze.get_value();

		if (env.peek() != SymbolType::right_paren)
		{
			// TODO error
		}
		env.dequeue_and_push_message();							// right_paren
		if (!in_branch_of<StatementAnalyze>(env))
		{
			// TODO error
		}
		StatementAnalyze()(env);									// 语句

	}
	env.push_message("<循环语句>");
}

// 步长
void StepLengthAnalyze::analyze(Env& env)
{
	UnsignedIntegerAnalyze unsigned_integer_analyze;
	unsigned_integer_analyze(env);
	value = unsigned_integer_analyze.get_value();
	env.push_message("<步长>");
}

// 条件语句
void ConditionStatementAnalyze::analyze(Env& env)
{
	env.dequeue_and_push_message();								// key_if
	if (env.peek() != SymbolType::left_paren)
	{
		// TODO error
	}
	env.dequeue_and_push_message();								// left_paren
	if (!in_branch_of<ConditionAnalyze>(env))
	{
		// TODO error
	}
	ConditionAnalyze()(env);									// 条件
	if (env.peek() != SymbolType::right_paren)
	{
		// TODO error
	}
	env.dequeue_and_push_message();								// right_paren

	if (!in_branch_of<StatementAnalyze>(env))
	{
		// TODO error
	}
	StatementAnalyze()(env);									// 语句

	if (env.peek() == SymbolType::key_else)
	{
		env.dequeue_and_push_message();							// else
		if (!in_branch_of<StatementAnalyze>(env))
		{
			// TODO error
		}
		StatementAnalyze()(env);									// 语句
	}

	env.push_message("<条件语句>");
}

// 条件
void ConditionAnalyze::analyze(Env& env)
{
	ExpressionAnalyze left_expression_analyze;
	left_expression_analyze(env);								// 表达式

	switch (env.peek())
	{
	default:
		// TODO error
		break;
	case SymbolType::equal:
	case SymbolType::not_equal:
	case SymbolType::less:
	case SymbolType::less_equal:
	case SymbolType::greater:
	case SymbolType::greater_equal:
		break;
	}
	env.dequeue_and_push_message();								// 关系运算符

	if (!in_branch_of<ExpressionAnalyze>(env))
	{
		// TODO error
	}
	ExpressionAnalyze right_expression_analyze;
	right_expression_analyze(env);								// 表达式
}

// 有返回值函数调用语句
void CallReturnFunctionStatementAnalyze::analyze(Env& env)
{
	auto token = env.dequeue_and_push_message();				// identifier
	if (env.peek() != SymbolType::left_paren)
	{
		// TODO error
	}
	env.dequeue_and_push_message();								// left_paren

	auto id_token = dynamic_pointer_cast<const IdentifierToken>(token);
	auto id_info = env.get_identifier_info(id_token->id_name_content);
	auto function_type = dynamic_pointer_cast<const FuctionIdentifierType>(id_info->return_type);
	if (!in_branch_of<ParameterValueListAnalyze>(env) && env.peek() != SymbolType::right_paren)
	{
		// TODO error
	}
	ParameterValueListAnalyze parameter_value_list_analyze(function_type->param_type_list);
	parameter_value_list_analyze(env);							// 值参数表

	if (env.peek() != SymbolType::right_paren)
	{
		// TODO error
	}
	env.dequeue_and_push_message();								// right_paren

	env.push_message("<有返回值函数调用语句>");
}

// 无返回值函数调用语句
void CallVoidFunctionStatementAnalyze::analyze(Env& env)
{
	auto token = env.dequeue_and_push_message();				// identifier
	if (env.peek() != SymbolType::left_paren)
	{
		// TODO error
	}
	env.dequeue_and_push_message();								// left_paren

	auto id_token = dynamic_pointer_cast<const IdentifierToken>(token);
	auto id_info = env.get_identifier_info(id_token->id_name_content);
	auto function_type = dynamic_pointer_cast<const FuctionIdentifierType>(id_info->return_type);
	if (!in_branch_of<ParameterValueListAnalyze>(env) && env.peek() != SymbolType::right_paren)
	{
		// TODO error
	}
	ParameterValueListAnalyze parameter_value_list_analyze(function_type->param_type_list);
	parameter_value_list_analyze(env);							// 值参数表

	if (env.peek() != SymbolType::right_paren)
	{
		// TODO error
	}
	env.dequeue_and_push_message();								// right_paren

	env.push_message("<无返回值函数调用语句>");
}

// 值参数表
void ParameterValueListAnalyze::analyze(Env& env)
{
	bool flag = false;
	ExpressionAnalyze expression_analyze;
	while (true)
	{
		if (flag)
		{
			if (env.peek() != SymbolType::comma)
			{
				break;
			}
			env.dequeue_and_push_message();							// comma
		}
		if (!in_branch_of<ExpressionAnalyze>(env))
		{
			if (!flag)
			{
				break;
			}
			// TODO error
		}
		expression_analyze(env);							// 表达式

		// TODO 判断表达式结果类型, 判断是否和形参匹配
		flag = true;
	}
	env.push_message("<值参数表>");
}

// 赋值语句
void AssignmentStatementAnalyze::analyze(Env& env)
{
	auto token = env.dequeue_and_push_message();						// identifier
	auto id_token = dynamic_pointer_cast<const IdentifierToken>(token);
	auto id_type = env.get_identifier_info(id_token->id_name_content)->return_type;


	if (env.peek() == SymbolType::left_square)
	{
		// 数组
		env.dequeue_and_push_message();									// left_square
		if (!in_branch_of<ExpressionAnalyze>(env))
		{
			// TODO error
		}
		ExpressionAnalyze expression_analyze_1;
		expression_analyze_1(env);										// 表达式
		if (env.peek() != SymbolType::right_square)
		{
			// TODO error
		}
		env.dequeue_and_push_message();									// right_square
		if (env.peek() == SymbolType::left_square)
		{
			// 二维数组
			env.dequeue_and_push_message();									// left_square
			if (!in_branch_of<ExpressionAnalyze>(env))
			{
				// TODO error
			}
			ExpressionAnalyze expression_analyze_2;
			expression_analyze_2(env);										// 表达式
			if (env.peek() != SymbolType::right_square)
			{
				// TODO error
			}
			env.dequeue_and_push_message();									// right_square

			if (id_type->extern_type != ExternType::d_array)
			{
				// TODO error
			}
		}
		else
		{
			// 一维数组
			if (id_type->extern_type != ExternType::l_array)
			{
				// TODO error
			}
		}
	}
	else
	{
		// 普通变量
		if (id_type->extern_type != ExternType::variable)
		{
			// TODO error
		}
	}
	
	if (env.peek() != SymbolType::assign)
	{
		// TODO error
	}
	env.dequeue_and_push_message();									// assign

	if (!in_branch_of<ExpressionAnalyze>(env))
	{
		// TODO error
	}
	ExpressionAnalyze left_expression_analyze;
	left_expression_analyze(env);									// 表达式
	
	env.push_message("<赋值语句>");
}

// 读语句
void ReadStatementAnalyze::analyze(Env& env)
{
	env.dequeue_and_push_message();								// key_scanf
	if (env.peek() != SymbolType::left_paren)
	{
		// TODO error
	}
	env.dequeue_and_push_message();								// left_paren

	if (env.peek() != SymbolType::identifier)
	{
		// TODO error
	}
	auto token = env.dequeue_and_push_message();				// identifier
	auto id_token = dynamic_pointer_cast<const IdentifierToken>(token);
	auto id_type = env.get_identifier_info(id_token->id_name_content)->return_type;
	if (id_type->extern_type != ExternType::variable)
	{
		// TODO error
	}
	// 读取

	if (env.peek() != SymbolType::right_paren)
	{
		// TODO error
	}
	env.dequeue_and_push_message();								// right_paren
	env.push_message("<读语句>");
}

// 写语句
void WriteStatementAnalyze::analyze(Env& env)
{
	env.dequeue_and_push_message();								// key_printf
	if (env.peek() != SymbolType::left_paren)
	{
		// TODO error
	}
	env.dequeue_and_push_message();								// left_paren

	if (env.peek() == SymbolType::string)
	{
		auto tk1 = env.dequeue_and_push_message();				// 字符串
		auto str_token = dynamic_pointer_cast<const StringToken>(tk1);
		env.push_message("<字符串>");
		if (env.peek() == SymbolType::comma)
		{
			env.dequeue_and_push_message();						// comma
			if (!in_branch_of<ExpressionAnalyze>(env))
			{
				// TODO error
			}
			ExpressionAnalyze expression_analyze;
			expression_analyze(env);
		}
		else
		{
			// TODO 仅字符串
		}
	}
	else
	{
		// 仅表达式
		if (!in_branch_of<ExpressionAnalyze>(env))
		{
			// TODO error
		}
		ExpressionAnalyze expression_analyze;
		expression_analyze(env);
	}

	if (env.peek() != SymbolType::right_paren)
	{
		// TODO error
	}
	env.dequeue_and_push_message();								// right_paren
	env.push_message("<写语句>");
}

// 情况语句
void SwitchStatementAnalyze::analyze(Env& env)
{
	env.dequeue_and_push_message();								// key_switch
	if (env.peek() != SymbolType::left_paren)
	{
		// TODO error
	}
	env.dequeue_and_push_message();								// left_paren

	if (!in_branch_of<ExpressionAnalyze>(env))
	{
		// TODO error
	}
	ExpressionAnalyze expression_analyze;
	expression_analyze(env);									// 表达式

	if (env.peek() != SymbolType::right_paren)
	{
		// TODO error
	}
	env.dequeue_and_push_message();								// right_paren

	if (env.peek() != SymbolType::left_brance)
	{
		// TODO error
	}
	env.dequeue_and_push_message();								// left_brance

	if (!in_branch_of<SwitchTableAnalyze>(env))
	{
		// TODO error
	}
	SwitchTableAnalyze()(env);									// 情况表
	if (!in_branch_of<DefaultCaseAnalyze>(env))
	{
		// TODO error
	}
	DefaultCaseAnalyze()(env);									// 缺省

	if (env.peek() != SymbolType::right_brance)
	{
		// TODO error
	}
	env.dequeue_and_push_message();								// right_brance
	env.push_message("<情况语句>");
}

// 情况表
void SwitchTableAnalyze::analyze(Env& env)
{
	unordered_set<int> used_case;
	while (!in_branch_of<CaseStatementAnalyze>(env))
	{
		CaseStatementAnalyze case_statement_analyze;
		case_statement_analyze(env);								// 情况子语句
		if (used_case.find(case_statement_analyze.get_case_value()) != used_case.end())
		{
			// TODO error
		}
		used_case.insert(case_statement_analyze.get_case_value());
	}
	if (used_case.empty())
	{
		// TODO error
	}
	env.push_message("<情况表>");
}

// 情况子语句
void CaseStatementAnalyze::analyze(Env& env)
{
	env.dequeue_and_push_message();						// key_case
	if (!in_branch_of<ConstantAnalyze>(env))
	{
		// TODO error
	}
	ConstantAnalyze constant_analyze;					// 常量
	constant_analyze(env);
	if (constant_analyze.is_type_of(BaseType::type_char))
	{
		case_value = constant_analyze.get_char();
	}
	else
	{
		case_value = constant_analyze.get_int();
	}

	if (env.peek() != SymbolType::colon)
	{
		// TODO error
	}
	env.dequeue_and_push_message();						// colon

	if (!in_branch_of<StatementAnalyze>(env))
	{
		// TODO error
	}
	StatementAnalyze()(env);							// 语句
	env.push_message("<情况子语句>");
}

// 缺省
void DefaultCaseAnalyze::analyze(Env& env)
{
	env.dequeue_and_push_message();				// key_default

	if (env.peek() != SymbolType::colon)
	{
		// TODO error
	}
	env.dequeue_and_push_message();				// colon

	if (!in_branch_of<StatementAnalyze>(env))
	{
		// TODO error
	}
	StatementAnalyze()(env);							// 语句
	env.push_message("<缺省>");

}

// 返回语句
void ReturnStatementAnalyze::analyze(Env& env)
{
	env.dequeue_and_push_message();					// key_return
	if (env.peek() != SymbolType::left_paren)
	{
		env.push_message("<返回语句>");
		return;
	}
	env.dequeue_and_push_message();					// left_paren

	if (!in_branch_of<ExpressionAnalyze>(env))
	{
		// TODO error
	}
	ExpressionAnalyze expression_analyze;
	expression_analyze(env);

	if (env.peek() != SymbolType::right_paren)
	{
		// TODO error
	}
	env.dequeue_and_push_message();					// right_paren

	env.push_message("<返回语句>");
}

// 表达式
void ExpressionAnalyze::analyze(Env& env)
{
	bool first_need_negative;
	if (env.peek() == SymbolType::plus || env.peek() == SymbolType::minus)
	{
		auto token = env.dequeue_and_push_message();				// + / -
		first_need_negative = token->type == SymbolType::minus;
	}

	bool flag = false;
	while (true)
	{
		bool need_negative = flag ? false : first_need_negative;
		if (flag)
		{
			if (env.peek() != SymbolType::plus && env.peek() != SymbolType::minus)
			{
				break;
			}
			need_negative = env.dequeue_and_push_message()->type == SymbolType::minus;	// + / -
		}
		if (!in_branch_of<TermAnalyze>(env))
		{
			// TODO error
		}
		TermAnalyze term_analyze;								// 项
		term_analyze(env);
		flag = true;
	}
	env.push_message("<表达式>");
}

// 项
void TermAnalyze::analyze(Env& env)
{
	bool flag = false;
	while (true)
	{
		bool is_mult = true;
		if (flag)
		{
			if (env.peek() != SymbolType::mult && env.peek() != SymbolType::div)
			{
				break;
			}
			is_mult = env.dequeue_and_push_message()->type == SymbolType::mult;			// * / /
		}
		if (!in_branch_of<FactorAnalyze>(env))
		{
			// TODO error
		}
		FactorAnalyze factor_analyze;
		factor_analyze(env);
		flag = true;
	}
	env.push_message("<项>");
}

// 因子
void FactorAnalyze::analyze(Env& env)
{
	if (env.peek() == SymbolType::character)
	{
		env.dequeue_and_push_message();							// charactor
	}
	else if (in_branch_of<IntegerAnalyze>(env))
	{
		IntegerAnalyze integer_analyze;
		integer_analyze(env);									// 整数
	}
	else if (env.peek() == SymbolType::left_paren)
	{
		env.dequeue_and_push_message();							// left_paren
		if (!in_branch_of<ExpressionAnalyze>(env))
		{
			// TODO error
		}
		ExpressionAnalyze expression_analyze;
		expression_analyze(env);								// 表达式
		if (env.peek() != SymbolType::right_paren)
		{
			// TODO error
		}
		env.dequeue_and_push_message();							// right_paren
	}
	else if (in_branch_of<CallReturnFunctionStatementAnalyze>(env))
	{
		CallReturnFunctionStatementAnalyze()(env);				// 有返回值函数调用语句
	}
	else
	{
		// assert env.peek() == SymbolType::identifier
		auto token = env.dequeue_and_push_message();			// identifier
		auto id_token = dynamic_pointer_cast<const IdentifierToken>(token);
		auto id_type = env.get_identifier_info(id_token->id_name_content)->return_type;
		if (env.peek() == SymbolType::left_square)
		{
			env.dequeue_and_push_message();						// left_square
			if (!in_branch_of<ExpressionAnalyze>(env))
			{
				// TODO error
			}
			ExpressionAnalyze size_1_expression_analyze;
			size_1_expression_analyze(env);						// 表达式
			if (env.peek() != SymbolType::right_square)
			{
				// TODO error
			}
			env.dequeue_and_push_message();						// right_square
			if (env.peek() == SymbolType::left_square)
			{
				env.dequeue_and_push_message();						// left_square
				if (!in_branch_of<ExpressionAnalyze>(env))
				{
					// TODO error
				}
				ExpressionAnalyze size_2_expression_analyze;
				size_2_expression_analyze(env);						// 表达式
				if (env.peek() != SymbolType::right_square)
				{
					// TODO error
				}
				env.dequeue_and_push_message();						// right_square

				// 二维数组
				if (id_type->extern_type != ExternType::d_array)
				{
					// TODO error
				}
			}
			else
			{
				// 一维数组
				if (id_type->extern_type != ExternType::l_array)
				{
					// TODO error
				}
			}
		}
		else
		{
			// 普通变量
			if (id_type->extern_type != ExternType::variable)
			{
				// TODO error
			}
		}
	}
	env.push_message("<因子>");
}
