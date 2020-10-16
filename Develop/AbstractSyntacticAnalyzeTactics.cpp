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

// ����
void ProgramAnalyze::analyze(Env& env)
{
	if (in_branch_of<ConstantDeclarationAnalyze>(env))
	{
		ConstantDeclarationAnalyze()(env);							// ����˵��
	}
	if (env.peek() == SymbolType::key_int || env.peek() == SymbolType::key_char)
	{
		VariableDeclarationAnalyze()(env);							// ����˵��
	}
	while (true)
	{
		SymbolType type = env.peek();
		if (in_branch_of<ReturnFunctionDefinationAnalyze>(env))
		{
			ReturnFunctionDefinationAnalyze()(env);					// �з���ֵ��������
		}
		else if (in_branch_of<MainFunctionAnalyze>(env))
		{
			MainFunctionAnalyze()(env);								// ������
			break;
		}
		else if (in_branch_of<VoidFunctionDefinationAnalyze>(env))
		{
			
			VoidFunctionDefinationAnalyze()(env);					// �޷���ֵ��������
		}
		else
		{
			//TODO error
		}
	}
	env.push_message("<����>");
}

// ����˵��
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
		ConstantDefinationAnalyze()(env);						// ��������
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
	env.push_message("<����˵��>");
}

// ��������
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
				IntegerAnalyze integer_analyze;					// ����
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
		env.push_message("<��������>");
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

// ����˵��
void VariableDeclarationAnalyze::analyze(Env& env)
{
	bool flag = true;
	while (in_branch_of<VariableDefinationAnalyze>(env))
	{
		VariableDefinationAnalyze()(env);			// ��������
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
	env.push_message("<����˵��>");
}

// ��������
void VariableDefinationAnalyze::analyze(Env& env)
{
	if (in_branch_of<VariableDefinationWithInitializationAnalyze>(env))
	{
		VariableDefinationWithInitializationAnalyze()(env);		// �������弰��ʼ��
	}
	else if (in_branch_of<VariableDefinationNoInitializationAnalyze>(env))
	{
		VariableDefinationNoInitializationAnalyze()(env);		// ���������޳�ʼ��
	}
	else
	{
		// TODO error
	}
	env.push_message("<��������>");
}

// �������弰��ʼ��
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
				// ����
				if (env.peek(3) == SymbolType::left_square)
				{
					// ��ά����
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
							constant_analyze(env);							// ����
							if (!constant_analyze.is_type_of(base_type))
							{
								// TODO error
							}
							// TODO ��ֵ
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

					// ������ű�
					shared_ptr<DoubleDimensionalArrayIdentifierType> id_type = make_shared<DoubleDimensionalArrayIdentifierType>();
					id_type->extern_type = ExternType::d_array;
					id_type->size_1 = size_1;
					id_type->size_2 = size_2;
					return_type = id_type;
				}
				else
				{
					// һά����
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
						constant_analyze(env);							// ����
						if (!constant_analyze.is_type_of(base_type))
						{
							// TODO error
						}
						// TODO ��ֵ
					}
					if (env.peek() != SymbolType::right_brance)
					{
						// TODO error
					}
					env.dequeue_and_push_message();						// right_brance

					// ������ű�
					shared_ptr<LinearArrayIdentifierType> id_type = make_shared<LinearArrayIdentifierType>();
					id_type->extern_type = ExternType::l_array;
					id_type->size = size;
					return_type = id_type;
				}
			}
			else
			{
				// ��ͨ����
				env.dequeue_and_push_message();								// assign
				if (!in_branch_of<ConstantAnalyze>(env))
				{
					// TODO error
				}
				constant_analyze(env);										// ����
				if (!constant_analyze.is_type_of(base_type))
				{
					// TODO error
				}
				// TODO ��ֵ

				// ������ű�
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
		env.push_message("<�������弰��ʼ��>");
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

// ���������޳�ʼ��
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
				// ����
				if (env.peek(3) == SymbolType::left_square)
				{
					// ��ά����
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

					// ������ű�
					shared_ptr<DoubleDimensionalArrayIdentifierType> id_type = make_shared<DoubleDimensionalArrayIdentifierType>();
					id_type->extern_type = ExternType::d_array;
					id_type->size_1 = size_1;
					id_type->size_2 = size_2;
					return_type = id_type;
				}
				else
				{
					// һά����
					env.dequeue_and_push_message();						// left_square
					UnsignedIntegerAnalyze unsigned_analyze;			// unsigned_integer
					unsigned_analyze(env);
					unsigned size = unsigned_analyze.get_value();
					if (env.peek() != SymbolType::right_square)
					{
						//TODO error
					}
					env.dequeue_and_push_message();						// right_square

					// ������ű�
					shared_ptr<LinearArrayIdentifierType> id_type = make_shared<LinearArrayIdentifierType>();
					id_type->extern_type = ExternType::l_array;
					id_type->size = size;
					return_type = id_type;
				}
			}
			else
			{
				// ��ͨ����

				// ������ű�
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
		env.push_message("<���������޳�ʼ��>");
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

// ����
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
		integer_analyze(env);							// ����
		integer = integer_analyze.get_value();			
	}
	env.push_message("<����>");
}

// ����
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
		UnsignedIntegerAnalyze unsigned_integer_analyzer;		// �޷�������
		value = unsigned_integer_analyzer.get_value();
		if (need_negative)
		{
			value = -value;
		}
	}
	env.push_message("<����>");
}

// �޷�������
void UnsignedIntegerAnalyze::analyze(Env& env)
{
	auto token = env.dequeue_and_push_message();		// number
	value = dynamic_pointer_cast<const UnsignedToken>(token)->unsigned_content;
	env.push_message("<�޷�������>");
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
		&& !AbstractSyntacticAnalyzeTactics::in_branch_of<CompoundStatements>(env))
	{
		// TODO error
	}
	CompoundStatements()(env);							// �������

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
	parameter_list_analyze(env);						// ������
	shared_ptr<const vector<BaseType>> param_type_list = parameter_list_analyze.get_param_type_list();
	auto param_list = parameter_list_analyze.get_param_list();

	if (env.peek() != SymbolType::right_paren)
	{
		// TODO error
	}
	env.dequeue_and_push_message();						// right_paren

	// ������ű�
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

// �з���ֵ��������
void ReturnFunctionDefinationAnalyze::analyze(Env& env)
{
	FunctionHeaderAnalyze function_header_analyze;
	function_header_analyze(env);						// ����ͷ��
	shared_ptr<const string> function_id = function_header_analyze.get_id();
	BaseType return_type = function_header_analyze.get_return_type();

	analyze_function(env, function_id, return_type, "<�з���ֵ��������>");
}

// �޷���ֵ��������
void VoidFunctionDefinationAnalyze::analyze(Env& env)
{
	env.dequeue_and_push_message();						// key_void

	if (env.peek() != SymbolType::identifier)
	{
		// TODO error
	}
	auto token = env.dequeue_and_push_message();		// identifier
	shared_ptr<const string> function_id = dynamic_pointer_cast<IdentifierToken>(token)->id_name_content;

	analyze_function(env, function_id, BaseType::type_void, "<�޷���ֵ��������>");
}

// ������
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
	env.push_message("<������>");
}

// ����ͷ��
void FunctionHeaderAnalyze::analyze(Env& env)
{
	env.dequeue_and_push_message();						// key_char / key_int
	return_type = env.peek() == SymbolType::key_char ? BaseType::type_char : BaseType::type_int;
	if (env.peek() != SymbolType::identifier)
	{
		// TODO error
	}
	auto token = env.dequeue_and_push_message();		// identifier
	id = dynamic_pointer_cast<IdentifierToken>(token)->id_name_content;
	env.push_message("<����ͷ��>");
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

// ������
void ParameterListAnalyze::analyze(Env& env)
{
	param_list = make_shared<vector<shared_ptr<IdentifierInfo>>>();
	while (in_first_set_of<ParameterListAnalyze>(env))
	{
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
		id_info->id = dynamic_pointer_cast<IdentifierToken>(id_token)->id_name_content;
		id_info->return_type = id_type;

		param_list->push_back(id_info);
	}
	env.push_message("<������>");
}
