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
		ConstantDeclarationAnalyze()(env);							// 常量说明
	}
	if (env.peek() == SymbolType::key_int || env.peek() == SymbolType::key_char)
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
				info->value = dynamic_pointer_cast<CharToken>(tk)->char_content;
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
			env.insert_identifier(id_info);
			new_ids.push_back(id_info);
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
					UnsignedIntegerAnalyze unsigned_analyze;			// unsigned_integer
					unsigned_analyze(env);
					unsigned size_1 = unsigned_analyze.get_value();
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
					UnsignedIntegerAnalyze unsigned_analyze;			// unsigned_integer
					unsigned_analyze(env);
					unsigned size = unsigned_analyze.get_value();
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
			env.insert_identifier(info);
			new_ids.push_back(info);
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
					UnsignedIntegerAnalyze unsigned_analyze;			// unsigned_integer
					unsigned_analyze(env);
					unsigned size_1 = unsigned_analyze.get_value();
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
			env.insert_identifier(info);
			new_ids.push_back(info);
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
