#include "SyntacticAnalyzeUtil.h"

void ErrorEnvironment::print_all_error(ostream& os)
{
	for (const auto& p : errors)
	{
		os << p.first.first << ' ' << p.first.second << std::endl;
	}
}


void MessageEnvironment::print_all_message(ostream& os)
{
	for (size_t i = 0; i != msg_index; ++i)
	{
		os << messages[i] << std::endl;
	}
}


const TokenEnvironment::token_ptr TokenEnvironment::NIL = make_shared<Token>();

void TokenEnvironment::ensure_capacity(size_t size)
{
	while (size >= symbols.size()) {
		if (!lexical_analyzer->has_next())
		{
			symbols.push_back(NIL);
			continue;
		}
		SymbolType next_type = lexical_analyzer->next();
		if (lexical_analyzer->is_wrong())
		{
			error_back(lexical_analyzer->get_line_number(), ErrorType::lexical_error);
		}
		shared_ptr<const Token> info = lexical_analyzer->get_token();
		symbols.push_back(info);
	}
}

TokenEnvironment::token_ptr TokenEnvironment::dequeue_certain(SymbolType type)
{
	ensure_capacity(sym_index);
	if (symbols[sym_index]->type == type)
	{
		return dequeue();
	}
	if (type == SymbolType::semicolon || type == SymbolType::right_paren || type == SymbolType::right_square)
	{
		error_require(symbols[sym_index - 1]->line_number, type);
	}
	else
	{
		error_require(symbols[sym_index]->line_number, type);
	}
	return nullptr;
}

TokenEnvironment::token_ptr TokenEnvironment::dequeue_certain_and_message_back(SymbolType type)
{
	ensure_capacity(sym_index);
	if (symbols[sym_index]->type == type)
	{
		return dequeue_and_message_back();
	}
	if (type == SymbolType::semicolon || type == SymbolType::right_paren || type == SymbolType::right_square)
	{
		error_require(symbols[sym_index - 1]->line_number, type);
	}
	else
	{
		error_require(symbols[sym_index]->line_number, type);
	}
	return nullptr;
}

irelem_t SymbolTableEnvironment::insert_identifier(shared_ptr<IdentifierInfo> id)
{
	if (id->return_type->is_one_from(ExternType::l_array, ExternType::d_array))
	{
		shared_ptr<const ArrayIdentifierType> arr_type = dynamic_pointer_cast<const ArrayIdentifierType>(id->return_type);
		int size = arr_type->total_size();
		irelem_t arr = elem().alloc_arr(id->id);
		id->ir_id = arr;
		int space;
		irelem_t type;
		if (arr_type->base_type == BaseType::type_char)
		{
			type = IrType::_char;
			space = size;
		}
		else
		{
			type = IrType::_int;
			space = size * 4;
		}
		code_builder().push_back(ir().arr(arr, type, size));
	}
	else if (id->return_type->is_one_from(ExternType::constant))
	{
		shared_ptr<ConstantIdentifierInfo> const_id = dynamic_pointer_cast<ConstantIdentifierInfo>(id);
		irelem_t imm = elem().alloc_imm(const_id->get_value());
		id->ir_id = imm;
	}
	else if (id->return_type->is_one_from(ExternType::variable))
	{
		irelem_t var = elem().alloc_named(id->id);
		id->ir_id = var;
		if (using_global_table)
		{
			code_builder().push_back(ir().gvar(var));
		}
	}
	else if (id->return_type->is_one_from(ExternType::function))
	{
		shared_ptr<FunctionIdentifierInfo> func_type = dynamic_pointer_cast<FunctionIdentifierInfo>(id);
		// irelem_t ret_type = func_type->return_type->base_type == BaseType::type_char ? IrType::_char : IrType::_int;
		//id->ir_id = elem().alloc_func(id->id).beg();
		//func_type->mid_label = elem().mid();
		//func_type->end_label = elem().end();
		// code_builder().push_back(ir().func(ret_type));
		// TODO param ÉùÃ÷
	}

	if (using_global_table)
	{
		global_table.insert_identifier(id);
	}
	else
	{
		local_table.insert_identifier(id);
	}
	return id->ir_id;
}

bool SyntacticAnalyzerEnvironment::ensure_func(
	function<bool(SyntacticAnalyzerEnvironment&)> success_condition,
	function<bool(SyntacticAnalyzerEnvironment&)> next_condition,
	ErrorType error_type, 
	unsigned max_turn
) 
{
	if (success_condition(*this))
	{
		return true;
	}
	error_back(symbols[sym_index]->line_number, error_type);
	size_t protec_index = sym_index;
	for (unsigned i = 0; i < max_turn; ++i)
	{
		if (success_condition(*this))
		{
			return true;
		}
		if (next_condition(*this))
		{
			return false;
		}
		if (peek() == SymbolType::end)
		{
			break;
		}
		dequeue_and_message_back();
	}
	sym_index = protec_index;
	throw reach_end_exception();
}
