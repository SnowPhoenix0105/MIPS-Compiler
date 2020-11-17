#include "AbstractSyntacticAnalyzeTactics.h"

FirstSetJudgement AbstractSyntacticAnalyzeTactics::first_set_judgment;
ErrorJudgment AbstractSyntacticAnalyzeTactics::error_judgment;

bool AbstractSyntacticAnalyzeTactics::operator()(Env& env)
{
	DEBUG_LOG_VAL(5, "calling", typeid(*this).name());
	state_t current_state = env.state();
	try
	{
		this->analyze(env);
	}
	catch (const syntax_exception&)
	{
		env.state(current_state);
		DEBUG_LOG_VAL(5, "exception", typeid(*this).name());
		return false;
	}
	//catch (const exception& e)
	//{
	//	env.error_back(env.peek_info()->line_number, e.what());
	//}
	DEBUG_LOG_VAL(5, "finish", typeid(*this).name());
	return true;
}

void AbstractSyntacticAnalyzeTactics::operator()(Env& env, int sign)
{
	DEBUG_LOG_VAL(5, "calling", typeid(*this).name());
	// state_t current_state = env.state();
	try
	{
		this->analyze(env);
	}
	catch (const syntax_exception&)
	{
		// env.state(current_state);
		DEBUG_LOG_VAL(5, "exception", typeid(*this).name());
		throw;
	}
	DEBUG_LOG_VAL(5, "finish", typeid(*this).name());
}

// ����
void ProgramAnalyze::analyze(Env& env)
{
	if (in_branch_of<ConstantDeclarationAnalyze>(env))
	{
		ConstantDeclarationAnalyze()(env);							// ����˵��
	}
	if (in_branch_of<VariableDeclarationAnalyze>(env))
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
			DEBUG_LOG_VAL(10, "env.peek()", symboltype_output_dictionary.at(env.peek()));
			DEBUG_LOG_VAL(10, "env.peek(1)", symboltype_output_dictionary.at(env.peek(1)));
			DEBUG_LOG_VAL(10, "env.peek(2)", symboltype_output_dictionary.at(env.peek(2)));
			DEBUG_LOG_VAL(10, "env.peek(3)", symboltype_output_dictionary.at(env.peek(3)));
			DEBUG_LOG_VAL(10, "env.peek(4)", symboltype_output_dictionary.at(env.peek(4)));
			DEBUG_LOG_VAL(10, "env.peek(5)", symboltype_output_dictionary.at(env.peek(5)));
			DEBUG_LOG_VAL(10, "env.peek(6)", symboltype_output_dictionary.at(env.peek(6)));
			DEBUG_LOG_VAL(10, "env.peek(7)", symboltype_output_dictionary.at(env.peek(7)));
			// PANIC();
			env.error_back(env.peek_info()->line_number, ErrorType::unknown_error);
			break;
		}
	}
	env.message_back("<����>");
}

// ����˵��
void ConstantDeclarationAnalyze::analyze(Env& env)
{
	bool flag = true;
	while (env.peek() == SymbolType::key_const)
	{
		env.dequeue_and_message_back();							// key_const
		if (env.ensure(in_branch_of<ConstantDefinationAnalyze>, { SymbolType::semicolon }))
		{
			ConstantDefinationAnalyze()(env);						// ��������
		}
		env.dequeue_certain_and_message_back(SymbolType::semicolon);		// semicolon
		flag = false;
	}
	env.message_back("<����˵��>");
}

// ��������
void ConstantDefinationAnalyze::analyze(Env& env)
{
	vector<shared_ptr<const IdentifierInfo>> new_ids;
	try
	{
		SymbolType type = env.peek();
		env.dequeue_and_message_back();							// key_int / key_char
		int count = 0;
		while (true)
		{
			if (count != 0)
			{
				if (env.peek() != SymbolType::comma)
				{
					break;
				}
				env.dequeue_and_message_back();					// comma
			}
			shared_ptr<const IdentifierToken> id;
			if (env.ensure({ SymbolType::identifier }, { SymbolType::assign }))
			{
				auto token = env.dequeue_and_message_back();		// identifier
				id = dynamic_pointer_cast<const IdentifierToken>(token);
				if (env.get_identifier_info(id->id_name_content).second)
				{
					env.error_back(token->line_number, ErrorType::duplicated_identifier);
				}
			}
			if (env.ensure({ SymbolType::assign }, OrCondition<Env>(IsType<Env>(SymbolType::character), in_branch_of<IntegerAnalyze>)))
			{
				env.dequeue_and_message_back();						// assign
			}
			shared_ptr<IdentifierInfo> id_info;
			shared_ptr<IdentifierType> id_type = make_shared<IdentifierType>();
			id_type->extern_type = ExternType::constant;
			if (type == SymbolType::key_char)
			{
				id_type->base_type = BaseType::type_char;
				if (env.peek() != SymbolType::character)
				{
					if (in_branch_of<IntegerAnalyze>(env))
					{
						env.error_back(env.dequeue_and_message_back()->line_number, ErrorType::constant_type_mismatching);
					}
					else
					{
						env.error_back(env.peek_info()->line_number, ErrorType::unknown_error);
					}
				}
				else
				{
					auto tk = env.dequeue_and_message_back();		// charactor
					shared_ptr<CharactorIdentifierInfo> info = make_shared<CharactorIdentifierInfo>();
					info->value = dynamic_pointer_cast<const CharToken>(tk)->char_content;
					//info->ir_id = env.elem().alloc_imm(info->value);
					id_info = info;
				}
			}
			else
			{
				id_type->base_type = BaseType::type_int;
				if (!in_branch_of<IntegerAnalyze>(env))
				{
					if (env.peek() == SymbolType::character)
					{
						env.error_back(env.dequeue_and_message_back()->line_number, ErrorType::constant_type_mismatching);
					}
					else
					{
						env.error_back(env.peek_info()->line_number, ErrorType::unknown_error);
					}
				}
				else
				{
					IntegerAnalyze integer_analyze;					// ����
					integer_analyze(env);
					shared_ptr<IntegerIdentifierInfo> info = make_shared<IntegerIdentifierInfo>();
					info->value = integer_analyze.get_value();
					//info->ir_id = env.elem().alloc_imm(info->value);
					id_info = info;
				}
			}
			if (id != nullptr)
			{
				id_info->id = id->id_name_content;
				id_info->return_type = id_type;
				new_ids.push_back(id_info);
				env.insert_identifier(id_info);
			}
			++count;
		}
		env.message_back("<��������>");
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
		env.dequeue_certain_and_message_back(SymbolType::semicolon);	// semicolon
		flag = false;
	}
	env.message_back("<����˵��>");
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
	env.message_back("<��������>");
}

shared_ptr<InitializeListElement> VariableDefinationWithInitializationAnalyze::analyze_initialize_list(SyntacticAnalyzerEnvironment& env)
{
	int line_number = env.peek_info()->line_number;
	if (in_branch_of<ConstantAnalyze>(env))
	{
		ConstantAnalyze constant_analyze;
		constant_analyze(env);
		if (constant_analyze.is_type_of(BaseType::type_char))
		{
			return make_shared<InitializeListChar>(line_number, constant_analyze.get_char());
		}
		else
		{
			return make_shared<InitializeListInteger>(line_number, constant_analyze.get_int());
		}
	}
	if (env.peek() == SymbolType::left_brance)
	{
		env.dequeue_and_message_back();				// left_brance
		auto ret = make_shared<InitializeList>(line_number);
		while (env.peek() != SymbolType::right_brance)
		{
			if (ret->list.size() != 0)
			{
				env.dequeue_certain_and_message_back(SymbolType::comma);			// comma
			}
			auto rlt = analyze_initialize_list(env);
			if (rlt == nullptr)
			{
				break;
			}
			ret->list.push_back(rlt);
		}
		ret->last_line = env.dequeue_certain_and_message_back(SymbolType::right_brance)->line_number;	// right_brance
		return ret;
	}
	return nullptr;
}

// �������弰��ʼ��
void VariableDefinationWithInitializationAnalyze::analyze(Env& env)
{
	vector<shared_ptr<const IdentifierInfo>> new_ids;
	try
	{
		SymbolType type = env.peek();
		BaseType base_type = type == SymbolType::key_int ? BaseType::type_int : BaseType::type_char;
		env.dequeue_and_message_back();									// key_int / key_char

		int count = 0;
		while (true)
		{
			if (count != 0)
			{
				if (env.peek() != SymbolType::comma)
				{
					break;
				}
				env.dequeue_and_message_back();							// comma
			}

			shared_ptr<const IdentifierToken> id;
			if (env.ensure({ SymbolType::identifier }, { SymbolType::right_square, SymbolType::assign }))
			{
				auto token = env.dequeue_and_message_back();				// identifier
				id = dynamic_pointer_cast<const IdentifierToken>(token);
				if (env.get_identifier_info(id->id_name_content).second)
				{
					env.error_back(token->line_number, ErrorType::duplicated_identifier);
				}
			}

			shared_ptr<IdentifierType> return_type;
			ConstantAnalyze constant_analyze;
			vector<int> init_vec;
			if (env.peek() == SymbolType::left_square)
			{
				env.dequeue_and_message_back();						// left_square
				UnsignedIntegerAnalyze size_1_unsigned_analyze;			// unsigned_integer
				size_1_unsigned_analyze(env);
				unsigned size_1 = size_1_unsigned_analyze.get_value();
				env.dequeue_certain_and_message_back(SymbolType::right_square);		// right_square
				// ����
				if (env.peek() == SymbolType::left_square)
				{
					// ��ά����
					env.dequeue_and_message_back();						// left_square
					UnsignedIntegerAnalyze unsigned_analyze;			// unsigned_integer
					unsigned_analyze(env);
					unsigned size_2 = unsigned_analyze.get_value();
					env.dequeue_certain_and_message_back(SymbolType::right_square);		// right_square

					int line_number = env.dequeue_and_message_back()->line_number;			// assigin

					auto rlt = analyze_initialize_list(env);
					if (rlt != nullptr)
					{
						auto list_1 = dynamic_pointer_cast<InitializeList>(rlt);
						if (list_1 == nullptr)
						{
							env.error_back(rlt->line_number, ErrorType::array_initialize_mismatching);	// ά������ 0
							continue;
						}
						else
						{
							if (list_1->list.size() != size_1)
							{
								env.error_back(list_1->last_line, ErrorType::array_initialize_mismatching);				// �����ϵ�һά
							}
							for (auto line : list_1->list)
							{
								auto list_2 = dynamic_pointer_cast<InitializeList>(line);
								if (list_2 == nullptr)
								{
									env.error_back(line->line_number, ErrorType::array_initialize_mismatching);	// ά������ 1
									continue;
								}
								else
								{
									if (list_2->list.size() != size_2)
									{
										env.error_back(list_2->last_line, ErrorType::array_initialize_mismatching);			// �����ϵڶ�ά
									}
									for (auto elem : list_2->list)
									{
										if (typeid(*elem) == typeid(InitializeList))
										{
											env.error_back(elem->line_number, ErrorType::array_initialize_mismatching);	// ά������ 3
											continue;
										}
										if (type == SymbolType::key_char)
										{
											auto ch = dynamic_pointer_cast<InitializeListChar>(elem);
											if (ch == nullptr)
											{
												env.error_back(elem->line_number, ErrorType::constant_type_mismatching);	// �������Ͳ�ƥ��
											}
											else
											{
												init_vec.push_back(ch->content);
											}
										}
										else
										{
											// type == SymbolType::key_int
											auto in = dynamic_pointer_cast<InitializeListInteger>(elem);
											if (in == nullptr)
											{
												env.error_back(elem->line_number, ErrorType::constant_type_mismatching);	// �������Ͳ�ƥ��
											}
											else
											{
												init_vec.push_back(in->content);
											}
										}
									}
								}
							}
						}
					}
					else
					{
						env.error_back(line_number, ErrorType::unknown_error);
					}

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
					int line_number = env.dequeue_and_message_back()->line_number;			// assigin

					auto rlt = analyze_initialize_list(env);
					if (rlt != nullptr)
					{
						auto list = dynamic_pointer_cast<InitializeList>(rlt);
						if (list == nullptr)
						{
							env.error_back(rlt->line_number, ErrorType::array_initialize_mismatching);		// ά������ 0
						}
						else
						{
							if (list->list.size() != size_1)
							{
								env.error_back(list->end_line(), ErrorType::array_initialize_mismatching);		// ���Ȳ�ƥ��
							}
							for (auto elem : list->list)
							{

								if (typeid(*elem) == typeid(InitializeList))
								{
									env.error_back(elem->line_number, ErrorType::array_initialize_mismatching);	// ά������ 2
									continue;
								}
								if (type == SymbolType::key_char)
								{
									auto ch = dynamic_pointer_cast<InitializeListChar>(elem);
									if (ch == nullptr)
									{
										env.error_back(elem->line_number, ErrorType::constant_type_mismatching);	// �������Ͳ�ƥ��
									}
									else
									{
										init_vec.push_back(ch->content);
									}
								}
								else
								{
									// type == SymbolType::key_int
									auto in = dynamic_pointer_cast<InitializeListInteger>(elem);
									if (in == nullptr)
									{
										env.error_back(elem->line_number, ErrorType::constant_type_mismatching);	// �������Ͳ�ƥ��
									}
									else
									{
										init_vec.push_back(in->content);
									}
								}
							}
						}
					}
					else
					{
						env.error_back(line_number, ErrorType::unknown_error);
					}

					// ������ű�
					shared_ptr<LinearArrayIdentifierType> id_type = make_shared<LinearArrayIdentifierType>();
					id_type->extern_type = ExternType::l_array;
					id_type->size = size_1;
					return_type = id_type;
				}
			}
			else
			{
				// ��ͨ����
				env.dequeue_and_message_back();								// assign
				if (env.ensure(in_branch_of<ConstantAnalyze>, { SymbolType::semicolon, SymbolType::key_const, SymbolType::key_int, SymbolType::key_char }))
				{
					int line_number = env.peek_info()->line_number;
					constant_analyze(env);							// ����
					if (!constant_analyze.is_type_of(base_type))
					{
						env.error_back(line_number, ErrorType::constant_type_mismatching);
					}
					else
					{
						int value = base_type == BaseType::type_char ? constant_analyze.get_char() : constant_analyze.get_int();
						init_vec.push_back(value);
					}
				}

				// ������ű�
				shared_ptr<IdentifierType> id_type = make_shared<IdentifierType>();
				id_type->extern_type = ExternType::variable;
				return_type = id_type;
			}
			if (id != nullptr)
			{
				shared_ptr<IdentifierInfo> info = make_shared<IdentifierInfo>();
				return_type->base_type = base_type;
				info->id = id->id_name_content;
				info->return_type = return_type;
				new_ids.push_back(info);
				irelem_t elem = env.insert_identifier(info);
				if (info->return_type->is_one_from(ExternType::variable))
				{
					for (int ini : init_vec)
					{
						if (env.is_using_global_table())
						{
							env.code_builder().push_back(env.ir().init(ini));
						}
						else
						{
							env.code_builder().push_back(env.ir().add(elem, env.elem().zero(), env.elem().alloc_imm(ini)));
						}
						break;
					}
				}
				else
				{
					for (int ini : init_vec)
					{
						env.code_builder().push_back(env.ir().init(ini));
					}
				}
			}
			++count;
		}
		env.message_back("<�������弰��ʼ��>");
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
		env.dequeue_and_message_back();									// key_int / key_char
		int count = 0;
		while (true)
		{
			if (!new_ids.empty())
			{
				if (env.peek() != SymbolType::comma)
				{
					break;
				}
				env.dequeue_and_message_back();							// comma
			}
			shared_ptr<const IdentifierToken> id;
			if (env.ensure({ SymbolType::identifier }, { SymbolType::right_square, SymbolType::assign }))
			{
				auto token = env.dequeue_and_message_back();				// identifier
				id = dynamic_pointer_cast<const IdentifierToken>(token);
				if (env.get_identifier_info(id->id_name_content).second)
				{
					env.error_back(token->line_number, ErrorType::duplicated_identifier);
				}
			}
			shared_ptr<IdentifierType> return_type;
			if (env.peek() == SymbolType::left_square)
			{
				env.dequeue_and_message_back();							// left_square
				UnsignedIntegerAnalyze size_1_unsigned_analyze;			// unsigned_integer
				size_1_unsigned_analyze(env);
				unsigned size_1 = size_1_unsigned_analyze.get_value();
				env.dequeue_certain_and_message_back(SymbolType::right_square);		// right_square
				// ����
				if (env.peek() == SymbolType::left_square)
				{
					// ��ά����
					env.dequeue_and_message_back();							// left_square
					UnsignedIntegerAnalyze size_2_unsigned_analyze;			// unsigned_integer
					size_2_unsigned_analyze(env);
					unsigned size_2 = size_2_unsigned_analyze.get_value();
					env.dequeue_certain_and_message_back(SymbolType::right_square);	// right_square

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
					// ������ű�
					shared_ptr<LinearArrayIdentifierType> id_type = make_shared<LinearArrayIdentifierType>();
					id_type->extern_type = ExternType::l_array;
					id_type->size = size_1;
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
			if (id != nullptr)
			{
				shared_ptr<IdentifierInfo> info = make_shared<IdentifierInfo>();
				return_type->base_type = base_type;
				info->id = id->id_name_content;
				info->return_type = return_type;
				new_ids.push_back(info);
				env.insert_identifier(info);
			}
			++count;
		}
		env.message_back("<���������޳�ʼ��>");
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
		auto token = env.dequeue_and_message_back();	// charactor
		ch = dynamic_pointer_cast<const CharToken>(token)->char_content;
	}
	else
	{
		is_ch = false;
		IntegerAnalyze integer_analyze;
		integer_analyze(env);							// ����
		integer = integer_analyze.get_value();			
	}
	env.message_back("<����>");
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
		env.dequeue_and_message_back();							// +/-
	default:					
		env.ensure(in_branch_of<UnsignedIntegerAnalyze>, Env::always_false);
		UnsignedIntegerAnalyze unsigned_integer_analyzer;
		unsigned_integer_analyzer(env);							// �޷�������
		value = unsigned_integer_analyzer.get_value();
		if (need_negative)
		{
			value = -value;
		}
	}
	env.message_back("<����>");
}

// �޷�������
void UnsignedIntegerAnalyze::analyze(Env& env)
{
	auto token = env.dequeue_and_message_back();		// number
	value = dynamic_pointer_cast<const UnsignedToken>(token)->unsigned_content;
	env.message_back("<�޷�������>");
}

int analyze_inner_block(
	AbstractSyntacticAnalyzeTactics::Env& env, 
	shared_ptr<const vector<shared_ptr<IdentifierInfo>>> param_list, 
	shared_ptr<const string> func_name)
{
	env.change_to_local_table(func_name);

	for (auto param : *param_list)
	{
		env.insert_identifier(param);
		env.code_builder().push_back(env.ir().param(param->ir_id));
	}

	env.dequeue_certain_and_message_back(SymbolType::left_brance);						// left_brance

	env.ensure(
		[](SyntacticAnalyzerEnvironment& env)->bool
		{
			return FirstSetJudgement().in_branch_of<CompoundStatementsAnalyze>(env) || env.peek() == SymbolType::right_brance;
		},
		SyntacticAnalyzerEnvironment::always_false);
	CompoundStatementsAnalyze()(env);							// �������

	env.change_to_global_table();

	env.ensure({ SymbolType::right_brance }, SyntacticAnalyzerEnvironment::always_false);
	return env.dequeue_and_message_back()->line_number;						// right_brance 
}

int analyze_function(
	AbstractSyntacticAnalyzeTactics::Env& env,
	int line_number,
	shared_ptr<const string> function_id, 
	BaseType return_type,
	const char* information)
{
	bool need_insert = true;
	if (env.get_identifier_info(function_id).second)
	{
		need_insert = false;
		env.error_back(line_number, ErrorType::duplicated_identifier);
	}

	irelem_t beg = env.elem().alloc_func(function_id).beg();
	irelem_t mid = env.elem().mid();
	irelem_t end = env.elem().end();
	env.code_builder().push_back(env.ir().label(beg));
	irelem_t ret_type = return_type == BaseType::type_char ? IrType::_char : return_type == BaseType::type_int ? IrType::_int : IrType::_void;
	env.code_builder().push_back(env.ir().func(ret_type));

	env.dequeue_certain_and_message_back(SymbolType::left_paren);					// left_paren

	env.ensure(
		[](SyntacticAnalyzerEnvironment& env)->bool
		{
			return FirstSetJudgement().in_branch_of<ParameterListAnalyze>(env) || env.peek() == SymbolType::right_paren || env.peek() == SymbolType::left_brance;
		},
		{ SymbolType::left_brance });
	ParameterListAnalyze parameter_list_analyze;
	parameter_list_analyze(env);						// ������
	shared_ptr<const vector<BaseType>> param_type_list = parameter_list_analyze.get_param_type_list();
	auto param_list = parameter_list_analyze.get_param_list();

	env.dequeue_certain_and_message_back(SymbolType::right_paren);	// right_paren

	// ������ű�
	shared_ptr<FuctionIdentifierType> id_type = make_shared<FuctionIdentifierType>();
	id_type->base_type = return_type;
	id_type->extern_type = ExternType::function;
	id_type->param_type_list = param_type_list;
	shared_ptr<IdentifierInfo> function_info = make_shared<FunctionIdentifierInfo>();
	function_info->return_type = id_type;
	function_info->id = function_id;
	if (need_insert)
	{
		env.insert_identifier(function_info);
	}

	try
	{
		int line_number = analyze_inner_block(env, param_list, function_id);

		env.code_builder().push_back(env.ir().label(mid));
		env.code_builder().push_back(env.ir().label(end));

		env.message_back(information);
		return line_number;
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
	BaseType return_type = function_header_analyze.get_return_type();
	shared_ptr<const IdentifierToken> function_token = function_header_analyze.get_token();
	env.current_return_type = return_type;
	env.return_count = 0;
	int line_number;
	if (function_token != nullptr)
	{

		line_number = analyze_function(env, function_token->line_number, function_token->id_name_content, return_type, "<�з���ֵ��������>");
	}
	else
	{
		line_number = analyze_function(env, env.peek_info()->line_number, make_shared<string>("unknow"), BaseType::type_void, "<�޷���ֵ��������>");
	}
	if (env.return_count == 0)
	{
		env.error_back(line_number, ErrorType::wrong_return_in_return_function);
	}
}

// �޷���ֵ��������
void VoidFunctionDefinationAnalyze::analyze(Env& env)
{
	env.dequeue_and_message_back();						// key_void

	env.current_return_type = BaseType::type_void;
	env.return_count = 0;
	if (env.ensure({ SymbolType::identifier }, { SymbolType::left_paren }))
	{
		auto token = env.dequeue_and_message_back();		// identifier
		auto function_token = dynamic_pointer_cast<const IdentifierToken>(token);

		analyze_function(env, function_token->line_number, function_token->id_name_content, BaseType::type_void, "<�޷���ֵ��������>");
	}
	else
	{
		analyze_function(env, env.peek_info()->line_number, make_shared<string>("unknow"), BaseType::type_void, "<�޷���ֵ��������>");
	}
}

// ������
void MainFunctionAnalyze::analyze(Env& env)
{
	env.dequeue_and_message_back();					// key_void;
	env.dequeue_and_message_back();					// key_main;
	env.dequeue_certain_and_message_back(SymbolType::left_paren);	// left_paren
	env.dequeue_certain_and_message_back(SymbolType::right_paren);	// right_paren
	env.current_return_type = BaseType::type_void;
	env.return_count = 0;

	irelem_t beg = env.elem().alloc_func("main").beg();
	irelem_t mid = env.elem().mid();
	irelem_t end = env.elem().end();
	env.code_builder().push_back(env.ir().label(beg));
	env.code_builder().push_back(env.ir().func(IrType::_void));

	analyze_inner_block(env, make_shared<const vector<shared_ptr<IdentifierInfo>>>(), make_shared<string>("main"));

	env.code_builder().push_back(env.ir().label(mid));
	env.code_builder().push_back(env.ir().label(end));

	env.message_back("<������>");
}

// ����ͷ��
void FunctionHeaderAnalyze::analyze(Env& env)
{
	return_type = env.peek() == SymbolType::key_char ? BaseType::type_char : BaseType::type_int;
	env.dequeue_and_message_back();						// key_char / key_int
	if (env.ensure({ SymbolType::identifier }, { SymbolType::left_paren }))
	{
		auto token = env.dequeue_and_message_back();		// identifier
		this->token = dynamic_pointer_cast<const IdentifierToken>(token);
	}
	env.message_back("<����ͷ��>");
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
			env.dequeue_and_message_back();							// comma
		}
		if (env.peek() != SymbolType::key_int && env.peek() != SymbolType::key_char)
		{
			if (flag)
			{
				env.error_back(env.peek_info()->line_number, ErrorType::unknown_error);
			}
			break;
		}
		auto type_token = env.dequeue_and_message_back();			// key_char / key_int
		if (env.peek() != SymbolType::identifier)
		{
			env.error_back(env.peek_info()->line_number, ErrorType::unknown_error);
			flag = true;
			continue;
		}
		auto id_token = env.dequeue_and_message_back();				// identifier

		shared_ptr<IdentifierType> id_type = make_shared<IdentifierType>();
		id_type->base_type = type_token->type == SymbolType::key_char ? BaseType::type_char : BaseType::type_int;
		id_type->extern_type = ExternType::variable;
		shared_ptr<IdentifierInfo> id_info = make_shared<IdentifierInfo>();
		id_info->id = dynamic_pointer_cast<const IdentifierToken>(id_token)->id_name_content;
		id_info->return_type = id_type;

		param_list->push_back(id_info);
		flag = true;
	}
	env.message_back("<������>");
}

// �������
void CompoundStatementsAnalyze::analyze(Env& env)
{
	if (in_branch_of<ConstantDeclarationAnalyze>(env))
	{
		ConstantDeclarationAnalyze()(env);							// ����˵��
	}
	if (in_branch_of<VariableDeclarationAnalyze>(env))
	{
		VariableDeclarationAnalyze()(env);							// ����˵��
	}
	StatementsListAnalyze()(env);											// �����
	env.message_back("<�������>");
}

// �����
void StatementsListAnalyze::analyze(Env& env)
{
	while (in_branch_of<StatementAnalyze>(env))
	{
		StatementAnalyze()(env);									// ���
	}
	env.message_back("<�����>");
}

// ���
void StatementAnalyze::analyze(Env& env)
{
	if (in_branch_of<LoopStatementAnalyze>(env))
	{
		LoopStatementAnalyze()(env);								// ѭ�����
	}
	else if (in_branch_of<ConditionStatementAnalyze>(env))
	{
		ConditionStatementAnalyze()(env);							// �������
	}
	else if (in_branch_of<ReadStatementAnalyze>(env))
	{
		ReadStatementAnalyze()(env);								// �����
		env.dequeue_certain_and_message_back(SymbolType::semicolon);	// semicolon
	}
	else if (in_branch_of<WriteStatementAnalyze>(env))
	{
		WriteStatementAnalyze()(env);								// д���
		env.dequeue_certain_and_message_back(SymbolType::semicolon);	// semicolon
	}
	else if (in_branch_of<SwitchStatementAnalyze>(env))
	{
		SwitchStatementAnalyze()(env);								// ������
	}
	else if (in_branch_of<ReturnStatementAnalyze>(env))
	{
		ReturnStatementAnalyze()(env);								// �������
		env.dequeue_certain_and_message_back(SymbolType::semicolon);	// semicolon
	}
	else if (in_branch_of<AssignmentStatementAnalyze>(env))
	{
		AssignmentStatementAnalyze()(env);							// ��ֵ���
		env.dequeue_certain_and_message_back(SymbolType::semicolon);	// semicolon
	}
	else if (in_branch_of<CallReturnFunctionStatementAnalyze>(env))
	{
		CallReturnFunctionStatementAnalyze()(env);					// �з���ֵ�����������
		env.dequeue_certain_and_message_back(SymbolType::semicolon);	// semicolon
	}
	else if (in_branch_of<CallVoidFunctionStatementAnalyze>(env))
	{
		CallVoidFunctionStatementAnalyze()(env);					// �޷���ֵ�������
		env.dequeue_certain_and_message_back(SymbolType::semicolon);	// semicolon
	}
	else if (env.peek() == SymbolType::left_brance)
	{
		env.dequeue_and_message_back();								// left_brance
		StatementsListAnalyze()(env);									// ���
		env.dequeue_certain_and_message_back(SymbolType::right_brance);			// right_brance
	}
	else
	{
		env.dequeue_certain_and_message_back(SymbolType::semicolon);	// semicolon
	}
	env.message_back("<���>");


	//switch (env.peek())
	//{
	//case SymbolType::key_while:
	//case SymbolType::key_for:
	//	LoopStatementAnalyze()(env);									// ѭ�����
	//	break;
	//case SymbolType::key_if:
	//	ConditionStatementAnalyze()(env);								// �������
	//	break;
	//case SymbolType::identifier:
	//	if (in_branch_of<AssignmentStatementAnalyze>(env))
	//	{
	//		AssignmentStatementAnalyze()(env);							// ��ֵ���
	//	}
	//	else if (in_branch_of<CallReturnFunctionStatementAnalyze>(env))
	//	{
	//		CallReturnFunctionStatementAnalyze()(env);					// �з���ֵ�����������
	//	}
	//	else // if (in_branch_of<CallVoidFunctionStatementAnalyze>(env))
	//	{
	//		CallVoidFunctionStatementAnalyze()(env);					// �޷���ֵ�������
	//	}
	//	env.dequeue_certain_and_message_back(SymbolType::semicolon);	// semicolon
	//	break;
	//case SymbolType::key_scanf:
	//	ReadStatementAnalyze()(env);									// �����
	//	env.dequeue_certain_and_message_back(SymbolType::semicolon);	// semicolon
	//	break;
	//case SymbolType::key_printf:
	//	WriteStatementAnalyze()(env);									// д���
	//	env.dequeue_certain_and_message_back(SymbolType::semicolon);	// semicolon
	//	break;
	//case SymbolType::key_switch:
	//	SwitchStatementAnalyze()(env);									// ������
	//	break;
	//case SymbolType::key_return:
	//	ReturnStatementAnalyze()(env);									// �������
	//	env.dequeue_certain_and_message_back(SymbolType::semicolon);	// semicolon
	//	break;
	//case SymbolType::left_brance:
	//	env.dequeue_and_message_back();									// left_brance
	//	StatementsListAnalyze()(env);										// �����
	//	if (env.peek() != SymbolType::right_brance)
	//	{
	//		// error
	//	}
	//	env.dequeue_and_message_back();									// right_brance
	//	env.dequeue_certain_and_message_back(SymbolType::semicolon);	// semicolon
	//	break;
	//default:
	//	// error
	//	break;
	//}

	//env.message_back("<���>");
}

// ѭ�����
void LoopStatementAnalyze::analyze(Env& env)
{
	if (env.peek() == SymbolType::key_while)
	{
		env.dequeue_and_message_back();							// key_while
		env.dequeue_certain_and_message_back(SymbolType::left_paren);		// left_paren

		irelem_t beg = env.elem().alloc_while().beg();
		irelem_t mid = env.elem().mid();
		irelem_t end = env.elem().end();

		env.code_builder().push_back(env.ir()._goto(mid));
		env.code_builder().push_back(env.ir().label(beg));
		auto front = env.fresh_code_builder();

		env.code_builder().push_back(env.ir().label(mid));
		if (env.ensure(in_branch_of<ConditionAnalyze>, { SymbolType::right_paren }))
		{
			ConditionAnalyze(true, beg)(env);								// ����
		}

		auto condition = env.fresh_code_builder();
		env.code_builder().push_back_all(*front);

		env.dequeue_certain_and_message_back(SymbolType::right_paren);	// right_paren
		env.ensure(in_branch_of<StatementAnalyze>, Env::always_false);
		
		StatementAnalyze()(env);									// ���

		env.code_builder().push_back_all(*condition);
		env.code_builder().push_back(env.ir().label(end));
	}
	else
	{
		env.dequeue_and_message_back();							// key_for

		env.dequeue_certain_and_message_back(SymbolType::left_paren);				// left_paren


		irelem_t beg = env.elem().alloc_for().beg();
		irelem_t mid = env.elem().mid();
		irelem_t end = env.elem().end();

		if (env.ensure({ SymbolType::identifier }, { SymbolType::semicolon }))
		{
			auto init_token = env.dequeue_and_message_back();			// identifier
			auto init_id_token = dynamic_pointer_cast<const IdentifierToken>(init_token);
			auto init_id_info = env.get_identifier_info(init_id_token->id_name_content).first;
			if (init_id_info == nullptr)
			{
				env.error_back(init_token->line_number, ErrorType::undefined_identifier);
			}
			else
			{
				if (init_id_info->return_type->extern_type == ExternType::constant)
				{
					env.error_back(init_id_token->line_number, ErrorType::try_change_const_value);
				}
			}
			env.dequeue_certain_and_message_back(SymbolType::assign);		// assign

			if (env.ensure(in_branch_of<ExpressionAnalyze>, { SymbolType::semicolon }))
			{
				ExpressionAnalyze expression_analyze;
				expression_analyze(env);								// ���ʽ
				if (init_id_info != nullptr)
				{
					env.code_builder().push_back(
						env.ir().add(init_id_info->ir_id, env.elem().zero(), expression_analyze.get_res()));
				}
			}
		}
		env.dequeue_certain_and_message_back(SymbolType::semicolon);	// semicolon

		env.code_builder().push_back(env.ir()._goto(mid));
		env.code_builder().push_back(env.ir().label(beg));
		auto front_codes = env.fresh_code_builder();
		env.code_builder().push_back(env.ir().label(mid));

		if (env.ensure(in_branch_of<ConditionAnalyze>, { SymbolType::semicolon }))
		{
			ConditionAnalyze(true, beg)(env);								// ����
		}
		env.dequeue_certain_and_message_back(SymbolType::semicolon);	// semicolon

		auto condition_codes = env.fresh_code_builder();

		shared_ptr<Ir> move_on_code;

		if (env.ensure({ SymbolType::identifier }, { SymbolType::right_paren }))
		{
			auto delta_left_token = env.dequeue_and_message_back();		// identifier
			auto delta_left_id_token = dynamic_pointer_cast<const IdentifierToken>(delta_left_token);
			auto delta_left_id_info = env.get_identifier_info(delta_left_id_token->id_name_content).first;
			if (delta_left_id_info == nullptr)
			{
				env.error_back(delta_left_token->line_number, ErrorType::undefined_identifier);
			}
			else
			{
				if (delta_left_id_info->return_type->extern_type == ExternType::constant)
				{
					env.error_back(delta_left_id_token->line_number, ErrorType::try_change_const_value);
				}
			}
			env.dequeue_certain_and_message_back(SymbolType::assign);		// assign
			shared_ptr<const IdentifierInfo> delta_right_id_info;
			if (env.peek() == SymbolType::identifier)
			{
				auto delta_right_token = env.dequeue_and_message_back();	// identifier
				auto delta_right_id_token = dynamic_pointer_cast<const IdentifierToken>(delta_right_token);
				delta_right_id_info = env.get_identifier_info(delta_right_id_token->id_name_content).first;
				if (delta_right_id_info == nullptr)
				{
					env.error_back(delta_right_token->line_number, ErrorType::undefined_identifier);
				}
				else
				{
					if (delta_right_id_info->return_type->extern_type == ExternType::constant)
					{
						env.error_back(delta_right_id_token->line_number, ErrorType::try_change_const_value);
					}
				}
			}
			else 
			{
				env.error_back(env.peek_info()->line_number, ErrorType::unknown_error);
			}
			SymbolType delta_type = env.peek();
			if (delta_type != SymbolType::plus && delta_type != SymbolType::minus)
			{
				env.error_back(env.peek_info()->line_number, ErrorType::unknown_error);
			}
			env.dequeue_and_message_back();							// +/-
			if (env.ensure(in_branch_of<StepLengthAnalyze>, { SymbolType::right_paren }))
			{
				StepLengthAnalyze step_length_analyze;
				step_length_analyze(env);								// ����
				unsigned step_length = step_length_analyze.get_value();
				if (delta_left_id_info != nullptr && delta_right_id_info != nullptr)
				{
					if (delta_type == SymbolType::plus)
					{
						move_on_code = make_shared<Ir>(IrHead::add, delta_left_id_info->ir_id, delta_right_id_info->ir_id, env.elem().alloc_imm(step_length));
					}
					if (delta_type == SymbolType::minus)
					{
						move_on_code = make_shared<Ir>(IrHead::sub, delta_left_id_info->ir_id, delta_right_id_info->ir_id, env.elem().alloc_imm(step_length));
					}
				}
			}
		}

		env.code_builder().push_back_all(*front_codes);

		env.dequeue_certain_and_message_back(SymbolType::right_paren);	// right_paren
		env.ensure(in_branch_of<StatementAnalyze>, Env::always_false);
		StatementAnalyze()(env);									// ���

		if (move_on_code != nullptr)
		{
			env.code_builder().push_back(*move_on_code);
		}
		env.code_builder().push_back_all(*condition_codes);
		env.code_builder().push_back(env.ir().label(end));
	}
	env.message_back("<ѭ�����>");
}

// ����
void StepLengthAnalyze::analyze(Env& env)
{
	UnsignedIntegerAnalyze unsigned_integer_analyze;
	unsigned_integer_analyze(env);
	value = unsigned_integer_analyze.get_value();
	env.message_back("<����>");
}

// �������
void ConditionStatementAnalyze::analyze(Env& env)
{
	irelem_t beg = env.elem().alloc_if().beg();
	irelem_t mid = env.elem().mid();
	irelem_t end = env.elem().end();

	env.dequeue_and_message_back();								// key_if
	env.dequeue_certain_and_message_back(SymbolType::left_paren);		// left_paren
	if (env.ensure(in_branch_of<ConditionAnalyze>, { SymbolType::right_paren }))
	{
		ConditionAnalyze(false, mid)(env);									// ����
	}

	env.code_builder().push_back(env.ir().label(beg));

	env.dequeue_certain_and_message_back(SymbolType::right_paren);	// right_paren

	env.ensure(in_branch_of<StatementAnalyze>, Env::always_false);
	StatementAnalyze()(env);									// ���

	if (env.peek() == SymbolType::key_else)
	{
		env.code_builder().push_back(env.ir()._goto(end));
		env.code_builder().push_back(env.ir().label(mid));
		env.dequeue_and_message_back();							// else
		if (env.ensure(in_branch_of<StatementAnalyze>, { SymbolType::semicolon }))
		{
			StatementAnalyze()(env);									// ���
		}
	}
	else
	{
		env.code_builder().push_back(env.ir().label(mid));
	}
	env.code_builder().push_back(env.ir().label(end));

	env.message_back("<�������>");
}

// ����
void ConditionAnalyze::analyze(Env& env)
{
	ExpressionAnalyze left_expression_analyze;
	left_expression_analyze(env);								// ���ʽ
	BaseType e1_type = left_expression_analyze.get_type();	
	irelem_t res_l = left_expression_analyze.get_res();

	auto op = env.dequeue_and_message_back();
	int line_number = op->line_number;			// ��ϵ�����

	if (env.ensure(in_branch_of<ExpressionAnalyze>, { SymbolType::right_paren, SymbolType::semicolon }))
	{
		ExpressionAnalyze right_expression_analyze;
		right_expression_analyze(env);								// ���ʽ
		BaseType e2_type = right_expression_analyze.get_type();
		irelem_t res_r = right_expression_analyze.get_res();
		if (e1_type == BaseType::type_char || e2_type == BaseType::type_char)
		{
			env.error_back(line_number, ErrorType::illegal_type_in_condition);
		}

		switch (op->type)
		{
		default:
			env.error_back(line_number, ErrorType::unknown_error);
			break;
		case SymbolType::equal:
			if (success_switch)
			{
				env.code_builder().push_back(env.ir().beq(res_l, res_r, target));
			}
			else
			{
				env.code_builder().push_back(env.ir().bne(res_l, res_r, target));
			}
			break;
		case SymbolType::not_equal:
			if (success_switch)
			{
				env.code_builder().push_back(env.ir().bne(res_l, res_r, target));
			}
			else
			{
				env.code_builder().push_back(env.ir().beq(res_l, res_r, target));
			}
			break;
		case SymbolType::less:
		{
			irelem_t rst = env.elem().alloc_tmp();
			env.code_builder().push_back(env.ir().less(rst, res_l, res_r));
			if (success_switch)
			{
				env.code_builder().push_back(env.ir().bne(rst, env.elem().zero(), target));
			}
			else
			{
				env.code_builder().push_back(env.ir().beq(rst, env.elem().zero(), target));
			}
			break;
		}
		case SymbolType::less_equal:
		{
			irelem_t rst = env.elem().alloc_tmp();
			env.code_builder().push_back(env.ir().less(rst, res_r, res_l));
			if (success_switch)
			{
				env.code_builder().push_back(env.ir().beq(rst, env.elem().zero(), target));
			}
			else
			{
				env.code_builder().push_back(env.ir().bne(rst, env.elem().zero(), target));
			}
			break;
		}
		case SymbolType::greater:
		{
			irelem_t rst = env.elem().alloc_tmp();
			env.code_builder().push_back(env.ir().less(rst, res_r, res_l));
			if (success_switch)
			{
				env.code_builder().push_back(env.ir().bne(rst, env.elem().zero(), target));
			}
			else
			{
				env.code_builder().push_back(env.ir().beq(rst, env.elem().zero(), target));
			}
			break;
		}
		case SymbolType::greater_equal:
		{
			irelem_t rst = env.elem().alloc_tmp();
			env.code_builder().push_back(env.ir().less(rst, res_l, res_r));
			if (success_switch)
			{
				env.code_builder().push_back(env.ir().beq(rst, env.elem().zero(), target));
			}
			else
			{
				env.code_builder().push_back(env.ir().bne(rst, env.elem().zero(), target));
			}
			break;
		}
		}
	}

	env.message_back("<����>");
}

// �з���ֵ�����������
void CallReturnFunctionStatementAnalyze::analyze(Env& env)
{
	auto token = env.dequeue_and_message_back();				// identifier
	auto id_token = dynamic_pointer_cast<const IdentifierToken>(token);
	auto id_info = env.get_identifier_info(id_token->id_name_content).first;
	shared_ptr<const vector<BaseType>> parameter_table;
	if (id_info == nullptr)
	{
		env.error_back(token->line_number, ErrorType::undefined_identifier);
		type = BaseType::type_int;
	}
	else
	{
		auto function_type = dynamic_pointer_cast<const FuctionIdentifierType>(id_info->return_type);
		type = function_type->base_type;
		parameter_table = function_type->param_type_list;
	}

	env.dequeue_certain_and_message_back(SymbolType::left_paren);						// left_paren

	env.ensure(
		TypeInsideSet<Env>({ SymbolType::right_paren, SymbolType::semicolon }) || in_branch_of<ParameterValueListAnalyze>,
		Env::always_false
	);
	ParameterValueListAnalyze parameter_value_list_analyze(parameter_table);
	parameter_value_list_analyze(env);							// ֵ������

	env.dequeue_certain_and_message_back(SymbolType::right_paren);	// right_paren
	
	if (id_info != nullptr)
	{
		env.code_builder().push_back(env.ir().call(id_info->ir_id));
		res = env.elem().alloc_tmp();
		env.code_builder().push_back(env.ir().add(res, env.elem().ret(), env.elem().zero()));
	}

	env.message_back("<�з���ֵ�����������>");
}

// �޷���ֵ�����������
void CallVoidFunctionStatementAnalyze::analyze(Env& env)
{
	auto token = env.dequeue_and_message_back();				// identifier
	auto id_token = dynamic_pointer_cast<const IdentifierToken>(token);
	auto id_info = env.get_identifier_info(id_token->id_name_content).first;
	shared_ptr<const vector<BaseType>> parameter_table;
	if (id_info == nullptr)
	{
		env.error_back(token->line_number, ErrorType::undefined_identifier);
	}
	else
	{
		auto function_type = dynamic_pointer_cast<const FuctionIdentifierType>(id_info->return_type);
		parameter_table = function_type->param_type_list;
	}

	env.dequeue_certain_and_message_back(SymbolType::left_paren);								// left_paren
	
	env.ensure(
		TypeInsideSet<Env>({ SymbolType::right_paren, SymbolType::semicolon }) || in_branch_of<ParameterValueListAnalyze>,
		Env::always_false
	);
	ParameterValueListAnalyze parameter_value_list_analyze(parameter_table);
	parameter_value_list_analyze(env);							// ֵ������

	env.dequeue_certain_and_message_back(SymbolType::right_paren);	// right_paren
	
	if (id_info != nullptr)
	{
		env.code_builder().push_back(env.ir().call(id_info->ir_id));
	}

	env.message_back("<�޷���ֵ�����������>");
}

// ֵ������
void ParameterValueListAnalyze::analyze(Env& env)
{
	unsigned count = 0;
	
	vector<irelem_t> results;
	vector<shared_ptr<IrTable>> codes;
	ExpressionAnalyze expression_analyze;
	auto front_codes = env.fresh_code_builder();
	while (true)
	{
		if (count != 0)
		{
			if (env.peek() != SymbolType::comma)
			{
				break;
			}
			env.dequeue_and_message_back();							// comma
		}
		if (!in_branch_of<ExpressionAnalyze>(env))
		{
			if (count != 0)
			{
				env.error_back(env.peek_info()->line_number, ErrorType::unknown_error);
			}
			break;
		}
		int line_number = env.peek_info()->line_number;
		expression_analyze(env);							// ���ʽ
		BaseType value_type = expression_analyze.get_type();
		if (param_type_list != nullptr)
		{
			if (count >= param_type_list->size())
			{
				env.error_back(line_number, ErrorType::parameter_count_mismatching);
			}
			else
			{
				BaseType param_type = (*param_type_list)[count];
				if (value_type != param_type)
				{
					env.error_back(line_number, ErrorType::parameter_type_mismatching);
				}
			}
		}

		codes.push_back(env.fresh_code_builder());
		results.push_back(expression_analyze.get_res());

		++count;
	}
	int line_number = env.peek_info()->line_number;
	if (param_type_list != nullptr && count != param_type_list->size())
	{
		env.error_back(line_number, ErrorType::parameter_count_mismatching);
	}

	env.code_builder().push_back_all(*front_codes);
	while (codes.size() > 0)
	{
		env.code_builder().push_back_all(*codes.back());
		codes.pop_back();
	}
	while (results.size() > 0)
	{
		env.code_builder().push_back(env.ir().push(results.back()));
		results.pop_back();
	}

	env.message_back("<ֵ������>");
}

// ��ֵ���
void AssignmentStatementAnalyze::analyze(Env& env)
{
	auto token = env.dequeue_and_message_back();						// identifier
	auto id_token = dynamic_pointer_cast<const IdentifierToken>(token);
	shared_ptr<const IdentifierType> id_type;
	auto id_info = env.get_identifier_info(id_token->id_name_content);

	shared_ptr<const IrTable> front_codes = env.fresh_code_builder();

	// exprд��buffer�� assign��buffer��ȡ
	irelem_t buffer = env.elem().alloc_tmp();

	if (id_info.first == nullptr)
	{
		env.error_back(token->line_number, ErrorType::undefined_identifier);
	}
	else
	{
		id_type = id_info.first->return_type;
		if (id_type->extern_type == ExternType::constant)
		{
			env.error_back(token->line_number, ErrorType::try_change_const_value);
		}
	}

	if (env.peek() == SymbolType::left_square)
	{
		// ����
		int line_number = env.dequeue_and_message_back()->line_number;		// left_square
		// if (env.ensure(in_branch_of<ExpressionAnalyze>, {SymbolType::left_square, SymbolType::right_square, SymbolType::assign }))
		if (!in_branch_of<ExpressionAnalyze>(env))
		{
			// TODO error
		}
		ExpressionAnalyze expression_analyze_1;
		expression_analyze_1(env);										// ���ʽ
		irelem_t index_1 = expression_analyze_1.get_res();
		if (expression_analyze_1.get_type() != BaseType::type_int)
		{
			env.error_back(line_number, ErrorType::non_int_index_for_array);
		}
		env.dequeue_certain_and_message_back(SymbolType::right_square);		// right_square
		if (env.peek() == SymbolType::left_square)
		{
			// ��ά����
			int line_number = env.dequeue_and_message_back()->line_number;		// left_square
			if (!in_branch_of<ExpressionAnalyze>(env))
			{
				// TODO error
			}
			ExpressionAnalyze expression_analyze_2;
			expression_analyze_2(env);										// ���ʽ
			irelem_t index_2 = expression_analyze_2.get_res();
			if (expression_analyze_2.get_type() != BaseType::type_int)
			{
				env.error_back(line_number, ErrorType::non_int_index_for_array);
			}
			env.dequeue_certain_and_message_back(SymbolType::right_square);		// right_square

			if (id_type != nullptr && id_info.first != nullptr)
			{
				if (id_type->extern_type != ExternType::d_array)
				{
					// TODO error
				}
				else
				{
					// ��ֵ
					BaseType type = id_type->base_type;
					shared_ptr<const DoubleDimensionalArrayIdentifierType> d_array_type = dynamic_pointer_cast<const DoubleDimensionalArrayIdentifierType>(id_type);
					irelem_t length_1 = env.elem().alloc_imm(d_array_type->size_2 * (type == BaseType::type_char ? 1 : 4));
					irelem_t base_off = env.elem().alloc_tmp();
					env.code_builder().push_back(env.ir().mult(base_off, index_1, length_1));
					irelem_t offset = env.elem().alloc_tmp();
					if (type == BaseType::type_char)
					{
						env.code_builder().push_back(env.ir().add(offset, base_off, index_2));
					}
					else
					{
						irelem_t more_off = env.elem().alloc_tmp();
						env.code_builder().push_back(env.ir().sl(more_off, index_2, env.elem().alloc_imm(2)));
						env.code_builder().push_back(env.ir().add(offset, base_off, more_off));
					}
					irelem_t base = env.elem().alloc_tmp();
					irelem_t base_reg = id_info.second ? env.elem().sp() : env.elem().gp();
					env.code_builder().push_back(env.ir().add(base, base_reg, offset));
					if (type == BaseType::type_char)
					{
						env.code_builder().push_back(env.ir().sb(buffer, base, id_info.first->ir_id));
					}
					else
					{
						env.code_builder().push_back(env.ir().sw(buffer, base, id_info.first->ir_id));
					}
				}
			}
		}
		else
		{
			// һά����
			if (id_type != nullptr && id_info.first != nullptr)
			{
				if (id_type->extern_type != ExternType::l_array)
				{
					// TODO error
				}
				else
				{
					// ��ֵ
					BaseType type = id_type->base_type;
					shared_ptr<const DoubleDimensionalArrayIdentifierType> d_array_type = dynamic_pointer_cast<const DoubleDimensionalArrayIdentifierType>(id_type);
					irelem_t base_reg = id_info.second ? env.elem().sp() : env.elem().gp();
					irelem_t base = env.elem().alloc_tmp();
					if (type == BaseType::type_char)
					{
						env.code_builder().push_back(env.ir().add(base, base_reg, index_1));
					}
					else
					{
						irelem_t offset = env.elem().alloc_tmp();
						env.code_builder().push_back(env.ir().sl(offset, index_1, env.elem().alloc_imm(2)));
						env.code_builder().push_back(env.ir().add(base, base_reg, offset));
					}
					if (type == BaseType::type_char)
					{
						env.code_builder().push_back(env.ir().sb(buffer, base, id_info.first->ir_id));
					}
					else
					{
						env.code_builder().push_back(env.ir().sw(buffer, base, id_info.first->ir_id));
					}
				}
			}
		}
	}
	else
	{
		// ��ͨ����
		if (id_type != nullptr && id_info.first != nullptr)
		{
			if (id_type->extern_type != ExternType::variable)
			{
				// TODO error
			}
			else
			{
				// ��ֵ
				env.code_builder().push_back(env.ir().add(id_info.first->ir_id, buffer, env.elem().zero()));
			}
		}
	}

	shared_ptr<const IrTable> assign_codes = env.fresh_code_builder();
	env.code_builder().push_back_all(*front_codes);
	
	if (env.peek() != SymbolType::assign)
	{
		// TODO error
	}
	env.dequeue_and_message_back();									// assign

	if (!in_branch_of<ExpressionAnalyze>(env))
	{
		// TODO error
	}
	ExpressionAnalyze left_expression_analyze;
	left_expression_analyze(env);									// ���ʽ

	env.code_builder().push_back(env.ir().add(buffer, left_expression_analyze.get_res(), env.elem().zero()));

	env.code_builder().push_back_all(*assign_codes);

	
	env.message_back("<��ֵ���>");
}

// �����
void ReadStatementAnalyze::analyze(Env& env)
{
	env.dequeue_and_message_back();								// key_scanf
	if (env.peek() != SymbolType::left_paren)
	{
		// TODO error
	}
	env.dequeue_and_message_back();								// left_paren

	if (env.peek() != SymbolType::identifier)
	{
		// TODO error
	}
	auto token = env.dequeue_and_message_back();				// identifier
	auto id_token = dynamic_pointer_cast<const IdentifierToken>(token);
	auto id_info = env.get_identifier_info(id_token->id_name_content).first;
	if (id_info == nullptr)
	{
		env.error_back(token->line_number, ErrorType::undefined_identifier);
	}
	else
	{
		auto id_type = id_info->return_type;
		if (id_type->extern_type != ExternType::variable)
		{
			env.error_back(token->line_number, ErrorType::try_change_const_value);
		}
		else
		{
			// ��ȡ
			irelem_t type = id_type->base_type == BaseType::type_char ? IrType::_char : IrType::_int;
			env.code_builder().push_back(env.ir().scanf(id_info->ir_id, type));
		}
	}

	env.dequeue_certain_and_message_back(SymbolType::right_paren);	// right_paren
	env.message_back("<�����>");
}

// д���
void WriteStatementAnalyze::analyze(Env& env)
{
	env.dequeue_and_message_back();								// key_printf
	if (env.peek() != SymbolType::left_paren)
	{
		// TODO error
	}
	env.dequeue_and_message_back();								// left_paren

	if (env.peek() == SymbolType::string)
	{
		auto tk1 = env.dequeue_and_message_back();				// �ַ���
		auto str_token = dynamic_pointer_cast<const StringToken>(tk1);
		env.message_back("<�ַ���>");
		if (env.peek() == SymbolType::comma)
		{
			env.dequeue_and_message_back();						// comma
			if (!in_branch_of<ExpressionAnalyze>(env))
			{
				// TODO error
			}
			ExpressionAnalyze expression_analyze;
			expression_analyze(env);
			// �ַ��� + ���ʽ
			irelem_t type = expression_analyze.get_type() == BaseType::type_char ? IrType::_char : IrType::_int;
			env.code_builder().push_back(env.ir().printf(str_token->string_content, expression_analyze.get_res(), type));
		}
		else
		{
			// TODO ���ַ���
			env.code_builder().push_back(env.ir().printf(str_token->string_content));
		}
	}
	else
	{
		// �����ʽ
		if (!in_branch_of<ExpressionAnalyze>(env))
		{
			// TODO error
		}
		ExpressionAnalyze expression_analyze;
		expression_analyze(env);
		irelem_t type = expression_analyze.get_type() == BaseType::type_char ? IrType::_char : IrType::_int;
		env.code_builder().push_back(env.ir().printf(expression_analyze.get_res(), type));
	}

	env.dequeue_certain_and_message_back(SymbolType::right_paren);	// right_paren
	env.message_back("<д���>");
}

// ������
void SwitchStatementAnalyze::analyze(Env& env)
{
	env.dequeue_and_message_back();								// key_switch
	if (env.peek() != SymbolType::left_paren)
	{
		// TODO error
	}
	env.dequeue_and_message_back();								// left_paren

	if (!in_branch_of<ExpressionAnalyze>(env))
	{
		// TODO error
	}
	ExpressionAnalyze expression_analyze;
	expression_analyze(env);									// ���ʽ
	BaseType switch_type = expression_analyze.get_type();

	irelem_t condition_value = expression_analyze.get_res();

	env.dequeue_certain_and_message_back(SymbolType::right_paren);	// right_paren

	if (env.peek() != SymbolType::left_brance)
	{
		// TODO error
	}
	env.dequeue_and_message_back();								// left_brance

	if (!in_branch_of<SwitchTableAnalyze>(env))
	{
		// TODO error
	}
	
	irelem_t beg = env.elem().alloc_switch().beg();
	irelem_t mid = env.elem().mid();
	irelem_t end = env.elem().end();

	auto front_code = env.fresh_code_builder();

	SwitchTableAnalyze st(switch_type, end);
	st(env);												// �����

	auto switch_table = env.fresh_code_builder();
	env.code_builder().push_back_all(*front_code);
	env.code_builder().push_back(env.ir().label(beg));
	for (const auto& v_l : st.get_value_label_table())
	{
		env.code_builder().push_back(env.ir().beq(condition_value, env.elem().alloc_imm(v_l.first), v_l.second));
	}

	// if (env.ensure(in_branch_of<DefaultCaseAnalyze>, SymbolType::right_brance, ErrorType::switch_no_defaule))
	if (env.ensure(in_branch_of<DefaultCaseAnalyze>, { SymbolType::right_brance }, ErrorType::switch_no_defaule))
	{
		DefaultCaseAnalyze()(env);									// ȱʡ
	}

	if (env.peek() != SymbolType::right_brance)
	{
		// TODO error
	}

	env.code_builder().push_back(env.ir()._goto(end));
	env.code_builder().push_back(env.ir().label(mid));
	env.code_builder().push_back_all(*switch_table);
	env.code_builder().back() = env.ir().label(end);

	env.dequeue_and_message_back();								// right_brance
	env.message_back("<������>");
}

// �����
// TODO
void SwitchTableAnalyze::analyze(Env& env)
{
	while (in_branch_of<CaseStatementAnalyze>(env))
	{
		irelem_t target = env.elem().alloc_case_label();
		env.code_builder().push_back(env.ir().label(target));
		CaseStatementAnalyze case_statement_analyze(switch_type);
		case_statement_analyze(env);								// ��������
		env.code_builder().push_back(env.ir()._goto(end_label));
		if (value_label_table.find(case_statement_analyze.get_case_value()) != value_label_table.end())
		{
			// TODO error
		}
		value_label_table.insert(make_pair(case_statement_analyze.get_case_value(), target));
	}
	if (value_label_table.empty())
	{
		// TODO error
	}
	env.message_back("<�����>");
}

// ��������
// TODO
void CaseStatementAnalyze::analyze(Env& env)
{
	int line_number = env.dequeue_and_message_back()->line_number;						// key_case
	if (!in_branch_of<ConstantAnalyze>(env))
	{
		// TODO error
	}
	ConstantAnalyze constant_analyze;					// ����
	constant_analyze(env);
	if (constant_analyze.is_type_of(BaseType::type_char))
	{
		case_value = constant_analyze.get_char();
	}
	else
	{
		case_value = constant_analyze.get_int();
	}
	if (!constant_analyze.is_type_of(switch_type))
	{
		env.error_back(line_number, ErrorType::constant_type_mismatching);
	}

	if (env.peek() != SymbolType::colon)
	{
		// TODO error
	}
	env.dequeue_and_message_back();						// colon

	if (!in_branch_of<StatementAnalyze>(env))
	{
		// TODO error
	}
	StatementAnalyze()(env);							// ���
	env.message_back("<��������>");
}

// ȱʡ
void DefaultCaseAnalyze::analyze(Env& env)
{
	env.dequeue_and_message_back();				// key_default

	if (env.peek() != SymbolType::colon)
	{
		// TODO error
	}
	env.dequeue_and_message_back();				// colon

	if (!in_branch_of<StatementAnalyze>(env))
	{
		// TODO error
	}
	StatementAnalyze()(env);							// ���
	env.message_back("<ȱʡ>");

}

// �������
void ReturnStatementAnalyze::analyze(Env& env)
{
	int line_number = env.dequeue_and_message_back()->line_number;		// key_return
	++env.return_count;
	if (env.peek() != SymbolType::left_paren)
	{
		if (env.current_return_type != BaseType::type_void)
		{
			env.error_back(line_number, ErrorType::wrong_return_in_return_function);
		}
		env.code_builder().push_back(env.ir().ret());
		env.message_back("<�������>");
		return;
	}

	env.dequeue_and_message_back();					// left_paren
	if (env.current_return_type == BaseType::type_void)
	{
		env.error_back(line_number, ErrorType::return_value_in_void_function);
	}
	if (env.peek() == SymbolType::right_paren)
	{
		if (env.current_return_type != BaseType::type_void)
		{
			env.error_back(line_number, ErrorType::wrong_return_in_return_function);
		}
	}
	else if (env.ensure(in_branch_of<ExpressionAnalyze>, TypeInsideSet<Env>({ SymbolType::right_paren , SymbolType::semicolon, SymbolType::right_brance }), ErrorType::wrong_return_in_return_function))
	{
		ExpressionAnalyze expression_analyze;
		expression_analyze(env);
		if (expression_analyze.get_type() != env.current_return_type && env.current_return_type != BaseType::type_void)
		{
			env.error_back(line_number, ErrorType::wrong_return_in_return_function);
		}
		env.code_builder().push_back(env.ir().add(env.elem().ret(), expression_analyze.get_res(), env.elem().zero()));
	}

	env.dequeue_certain_and_message_back(SymbolType::right_paren);	// right_paren

	env.code_builder().push_back(env.ir().ret());
	env.message_back("<�������>");
}

// ���ʽ
void ExpressionAnalyze::analyze(Env& env)
{
	bool first_need_negative = false;
	res = env.elem().zero();
	if (env.peek() == SymbolType::plus || env.peek() == SymbolType::minus)
	{
		auto token = env.dequeue_and_message_back();				// + / -
		first_need_negative = token->type == SymbolType::minus;
	}
	type = BaseType::type_char;
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
			need_negative = env.dequeue_and_message_back()->type == SymbolType::minus;	// + / -
			type = BaseType::type_int;					// char�������ʱΪint
		}
		if (!in_branch_of<TermAnalyze>(env))
		{
			// TODO error
		}
		TermAnalyze term_analyze;								// ��
		term_analyze(env);
		if (type == BaseType::type_char && term_analyze.get_type() == BaseType::type_int)
		{
			type = BaseType::type_int;
		}
		if (flag || need_negative)
		{
			irelem_t new_res = env.elem().alloc_tmp();
			if (need_negative)
			{
				env.code_builder().push_back(env.ir().sub(new_res, res, term_analyze.get_res()));
			}
			else
			{
				env.code_builder().push_back(env.ir().add(new_res, res, term_analyze.get_res()));
			}
			res = new_res;
		}
		else
		{
			res = term_analyze.get_res();
		}
		flag = true;
	}
	env.message_back("<���ʽ>");
}

// ��
void TermAnalyze::analyze(Env& env)
{
	bool flag = false;
	type = BaseType::type_char;
	res = env.elem().alloc_imm(1);
	while (true)
	{
		bool is_mult = true;
		if (flag)
		{
			if (env.peek() != SymbolType::mult && env.peek() != SymbolType::div)
			{
				break;
			}
			is_mult = env.dequeue_and_message_back()->type == SymbolType::mult;			// * / /
			type = BaseType::type_int;					// char�������ʱΪint
		}
		if (!in_branch_of<FactorAnalyze>(env))
		{
			// TODO error
		}
		FactorAnalyze factor_analyze;
		factor_analyze(env);
		if (type == BaseType::type_char && factor_analyze.get_type() == BaseType::type_int)
		{
			type = BaseType::type_int;
		}
		if (flag)
		{
			irelem_t new_res = env.elem().alloc_tmp();
			if (is_mult)
			{
				env.code_builder().push_back(env.ir().mult(new_res, factor_analyze.get_res(), res));
			}
			else
			{
				env.code_builder().push_back(env.ir().div(new_res, res, factor_analyze.get_res()));
			}
			res = new_res;
		}
		else
		{
			res = factor_analyze.get_res();
		}
		flag = true;
	}
	env.message_back("<��>");
}

// ����
void FactorAnalyze::analyze(Env& env)
{
	if (env.peek() == SymbolType::character)
	{
		token_ptr token = env.dequeue_and_message_back();							// charactor
		type = BaseType::type_char;
		char value = dynamic_pointer_cast<const CharToken>(token)->char_content;
		res = env.elem().alloc_imm(value);
	}
	else if (in_branch_of<IntegerAnalyze>(env))
	{
		IntegerAnalyze integer_analyze;
		integer_analyze(env);									// ����
		type = BaseType::type_int;
		int value = integer_analyze.get_value();
		res = env.elem().alloc_imm(value);
	}
	else if (env.peek() == SymbolType::left_paren)
	{
		env.dequeue_and_message_back();							// left_paren
		if (!in_branch_of<ExpressionAnalyze>(env))
		{
			// TODO error
		}
		ExpressionAnalyze expression_analyze;
		expression_analyze(env);								// ���ʽ
		type = BaseType::type_int;
		env.dequeue_certain_and_message_back(SymbolType::right_paren);// right_paren
		res = expression_analyze.get_res();
	}
	else if (in_branch_of<CallReturnFunctionStatementAnalyze>(env))
	{
		CallReturnFunctionStatementAnalyze call_return_function_statement_analyze;
		call_return_function_statement_analyze(env);				// �з���ֵ�����������
		type = call_return_function_statement_analyze.get_type();
		res = call_return_function_statement_analyze.get_res();
	}
	else
	{
		// assert env.peek() == SymbolType::identifier
		auto token = env.dequeue_and_message_back();			// identifier
		auto id_token = dynamic_pointer_cast<const IdentifierToken>(token);
		shared_ptr<const IdentifierType> id_type;
		auto id_info = env.get_identifier_info(id_token->id_name_content);
		if (id_info.first == nullptr)
		{
			env.error_back(token->line_number, ErrorType::undefined_identifier);
			type = BaseType::type_char;
		}
		else
		{
			id_type = id_info.first->return_type;
			type = id_type->base_type;
		}

		if (env.peek() == SymbolType::left_square)
		{
			env.dequeue_and_message_back();						// left_square
			if (!in_branch_of<ExpressionAnalyze>(env))
			{
				// TODO error
			}
			int line_number = env.peek_info()->line_number;
			ExpressionAnalyze size_1_expression_analyze;
			size_1_expression_analyze(env);						// ���ʽ
			if (size_1_expression_analyze.get_type() != BaseType::type_int)
			{
				env.error_back(line_number, ErrorType::non_int_index_for_array);
			}
			irelem_t index_1 = size_1_expression_analyze.get_res();
			env.dequeue_certain_and_message_back(SymbolType::right_square);		// right_square
			if (env.peek() == SymbolType::left_square)
			{
				env.dequeue_and_message_back();						// left_square
				if (!in_branch_of<ExpressionAnalyze>(env))
				{
					// TODO error
				}
				int line_number = env.peek_info()->line_number;
				ExpressionAnalyze size_2_expression_analyze;
				size_2_expression_analyze(env);						// ���ʽ
				irelem_t index_2 = size_2_expression_analyze.get_res();
				if (size_2_expression_analyze.get_type() != BaseType::type_int)
				{
					env.error_back(line_number, ErrorType::non_int_index_for_array);
				}
				env.dequeue_certain_and_message_back(SymbolType::right_square);		// right_square

				// ��ά����
				if (id_type != nullptr && id_info.first != nullptr)
				{
					if (id_type->extern_type != ExternType::d_array)
					{
						// TODO error
					}
					else
					{
						// ȡֵ
						shared_ptr<const DoubleDimensionalArrayIdentifierType> d_array_type = dynamic_pointer_cast<const DoubleDimensionalArrayIdentifierType>(id_type);
						irelem_t length_1 = env.elem().alloc_imm(d_array_type->size_2 * (type == BaseType::type_char ? 1 : 4));
						irelem_t base_off = env.elem().alloc_tmp();
						env.code_builder().push_back(env.ir().mult(base_off, index_1, length_1));
						irelem_t offset = env.elem().alloc_tmp();
						if (type == BaseType::type_char)
						{
							env.code_builder().push_back(env.ir().add(offset, base_off, index_2));
						}
						else
						{
							irelem_t more_off = env.elem().alloc_tmp();
							env.code_builder().push_back(env.ir().sl(more_off, index_2, env.elem().alloc_imm(2)));
							env.code_builder().push_back(env.ir().add(offset, base_off, more_off));
						}
						irelem_t base = env.elem().alloc_tmp();
						irelem_t base_reg = id_info.second ? env.elem().sp() : env.elem().gp();
						env.code_builder().push_back(env.ir().add(base, base_reg, offset));
						res = env.elem().alloc_tmp();
						if (type == BaseType::type_char)
						{
							env.code_builder().push_back(env.ir().lb(res, base, id_info.first->ir_id));
						}
						else
						{
							env.code_builder().push_back(env.ir().lw(res, base, id_info.first->ir_id));
						}
					}
				}
			}
			else
			{
				// һά����
				if (id_type != nullptr && id_info.first != nullptr)
				{
					if (id_type->extern_type != ExternType::l_array)
					{
						// TODO error
					}
					else
					{
						// ȡֵ
						shared_ptr<const DoubleDimensionalArrayIdentifierType> d_array_type = dynamic_pointer_cast<const DoubleDimensionalArrayIdentifierType>(id_type);
						irelem_t base_reg = id_info.second ? env.elem().sp() : env.elem().gp();
						irelem_t base = env.elem().alloc_tmp();
						if (type == BaseType::type_char)
						{
							env.code_builder().push_back(env.ir().add(base, base_reg, index_1));
						}
						else
						{
							irelem_t offset = env.elem().alloc_tmp();
							env.code_builder().push_back(env.ir().sl(offset, index_1, env.elem().alloc_imm(2)));
							env.code_builder().push_back(env.ir().add(base, base_reg, offset));
						}
						res = env.elem().alloc_tmp();
						if (type == BaseType::type_char)
						{
							env.code_builder().push_back(env.ir().lb(res, base, id_info.first->ir_id));
						}
						else
						{
							env.code_builder().push_back(env.ir().lw(res, base, id_info.first->ir_id));
						}
					}
				}
			}
		}
		else
		{
			// ��ͨ���� / ����
			if (id_type != nullptr && id_info.first != nullptr)
			{
				if (id_type->is_one_from(ExternType::variable, ExternType::constant))
				{
					res = id_info.first->ir_id;
				}
				else
				{
					// TODO error
				}
			}
		}
	}
	env.message_back("<����>");
}
