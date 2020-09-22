#include "SymbolType.h"

const std::unordered_map<string, SymbolType> saved_key_words_dictionary =
{
	{	"case"		,	SymbolType::key_case		},
	{	"char"		,	SymbolType::key_char		},
	{	"const"		,	SymbolType::key_const		},
	{	"default"	,	SymbolType::key_default		},
	{	"else"		,	SymbolType::key_else		},
	{	"for"		,	SymbolType::key_for			},
	{	"if"		,	SymbolType::key_if			},
	{	"int"		,	SymbolType::key_int			},
	{	"main"		,	SymbolType::key_main		},
	{	"printf"	,	SymbolType::key_printf		},
	{	"return"	,	SymbolType::key_return		},
	{	"scanf"		,	SymbolType::key_scanf		},
	{	"switch"	,	SymbolType::key_switch		},
	{	"void"		,	SymbolType::key_void		},
	{	"while"		,	SymbolType::key_while		}
};


const std::unordered_map<char, SymbolType> simple_symbol_dictionary = 
{
	{	':'		,	SymbolType::colon			},
	{	','		,	SymbolType::comma			},
	{	';'		,	SymbolType::semicolon		},
	{	'('		,	SymbolType::left_paren		},
	{	')'		,	SymbolType::right_paren		},
	{	'{'		,	SymbolType::left_brance		},
	{	'}'		,	SymbolType::right_brance	},
	{	'['		,	SymbolType::left_square		},
	{	']'		,	SymbolType::right_square	},
	{	'+'		,	SymbolType::plus			},
	{	'-'		,	SymbolType::minus			},
	{	'*'		,	SymbolType::mult			},
	{	'/'		,	SymbolType::div				}
};

const std::unordered_set<SymbolType> keyword_symbol_set =
{
	SymbolType::key_case,		// 关键字 case
	SymbolType::key_char,		// 关键字 char
	SymbolType::key_const,		// 关键字 const
	SymbolType::key_default,	// 关键字 default
	SymbolType::key_else,		// 关键字 else
	SymbolType::key_for,		// 关键字 for
	SymbolType::key_if,			// 关键字 if
	SymbolType::key_int,		// 关键字 int
	SymbolType::key_main,		// 关键字 main
	SymbolType::key_printf,		// 关键字 printf
	SymbolType::key_return,		// 关键字 return
	SymbolType::key_scanf,		// 关键字 scanf
	SymbolType::key_switch,		// 关键字 switch
	SymbolType::key_void,		// 关键字 void
	SymbolType::key_while		// 关键字 while
};


#ifdef DEBUG_LEVEL
const std::unordered_map<SymbolType, wstring> symboltype_translate_dictionary =
{
	{	SymbolType::key_case		,		L"关键字 case"		},
	{	SymbolType::key_char		,		L"关键字 char"		},
	{	SymbolType::key_const		,		L"关键字 const"		},
	{	SymbolType::key_default		,		L"关键字 default"	},
	{	SymbolType::key_else		,		L"关键字 else"		},
	{	SymbolType::key_for			,		L"关键字 for"		},
	{	SymbolType::key_if			,		L"关键字 if"			},
	{	SymbolType::key_int			,		L"关键字 int"		},
	{	SymbolType::key_main		,		L"关键字 main"		},
	{	SymbolType::key_printf		,		L"关键字 printf"		},
	{	SymbolType::key_return		,		L"关键字 return"		},
	{	SymbolType::key_scanf		,		L"关键字 scanf"		},
	{	SymbolType::key_switch		,		L"关键字 switch"		},
	{	SymbolType::key_void		,		L"关键字 void"		},
	{	SymbolType::key_while		,		L"关键字 while"		},
	{	SymbolType::colon			,		L"冒号 :"			},
	{	SymbolType::comma			,		L"逗号 ,"			},
	{	SymbolType::semicolon		,		L"分号 ;"			},
	{	SymbolType::left_paren		,		L"左圆括号 ("			},
	{	SymbolType::right_paren		,		L"右圆括号 )"			},
	{	SymbolType::left_brance		,		L"左花括号 {"			},
	{	SymbolType::right_brance	,		L"右花括号 },"		},
	{	SymbolType::left_square		,		L"左方括号 ["			},
	{	SymbolType::right_square	,		L"右方括号 ]"			},
	{	SymbolType::plus			,		L"加号 +"			},
	{	SymbolType::minus			,		L"减号 -"			},
	{	SymbolType::mult			,		L"乘号 *"			},
	{	SymbolType::div				,		L"除号 /"			},
	{	SymbolType::assign			,		L"赋值号 ="			},
	{	SymbolType::less			,		L"小于 <"			},
	{	SymbolType::greater			,		L"大于 >"			},
	{	SymbolType::less_equal		,		L"小于等于	<="		},
	{	SymbolType::greater_equal	,		L"大于等于	>="		},
	{	SymbolType::equal			,		L"等于号 =="			},
	{	SymbolType::not_equal		,		L"不等号 !="			},
	{	SymbolType::character		,		L"字符"				},
	{	SymbolType::string			,		L"字符串"			},
	{	SymbolType::number			,		L"数字"				},
	{	SymbolType::identifier		,		L"其它合法标识符"		},
	{	SymbolType::wrong			,		L"错误"				}
};
#endif // DEBUG_LEVEL


#ifdef HW_1
const std::unordered_map<SymbolType, string> symboltype_output_dictionary =
{
	{	SymbolType::key_case		,		"CASETK"		},
	{	SymbolType::key_char		,		"CHARTK"		},
	{	SymbolType::key_const		,		"CONSTTK"		},
	{	SymbolType::key_default		,		"DEFAULTTK"		},
	{	SymbolType::key_else		,		"ELSETK"		},
	{	SymbolType::key_for			,		"FORTK"			},
	{	SymbolType::key_if			,		"IFTK"			},
	{	SymbolType::key_int			,		"INTTK"			},
	{	SymbolType::key_main		,		"MAINTK"		},
	{	SymbolType::key_printf		,		"PRINTFTK"		},
	{	SymbolType::key_return		,		"RETURNTK"		},
	{	SymbolType::key_scanf		,		"SCANFTK"		},
	{	SymbolType::key_switch		,		"SWITCHTK"		},
	{	SymbolType::key_void		,		"VOIDTK"		},
	{	SymbolType::key_while		,		"WHILETK"		},

	{	SymbolType::colon			,		"COLON"			},
	{	SymbolType::comma			,		"COMMA"			},
	{	SymbolType::semicolon		,		"SEMICN"		},
	{	SymbolType::left_paren		,		"LPARENT"		},
	{	SymbolType::right_paren		,		"RPARENT"		},
	{	SymbolType::left_brance		,		"LBRACE"		},
	{	SymbolType::right_brance	,		"RBRACE"		},
	{	SymbolType::left_square		,		"LBRACK"		},
	{	SymbolType::right_square	,		"RBRACK"		},
	{	SymbolType::plus			,		"PLUS"			},
	{	SymbolType::minus			,		"MINU"			},
	{	SymbolType::mult			,		"MULT"			},
	{	SymbolType::div				,		"DIV"			},

	{	SymbolType::assign			,		"ASSIGN"		},
	{	SymbolType::less			,		"LSS"			},
	{	SymbolType::greater			,		"GRE"			},
	{	SymbolType::less_equal		,		"LEQ"			},
	{	SymbolType::greater_equal	,		"GEQ"			},
	{	SymbolType::equal			,		"EQL"			},
	{	SymbolType::not_equal		,		"NEQ"			},

	{	SymbolType::character		,		"CHARCON"		},
	{	SymbolType::string			,		"STRCON"		},
	{	SymbolType::number			,		"INTCON"		},
	{	SymbolType::identifier		,		"IDENFR"		},

	{	SymbolType::wrong			,		"WRONG"			}
};

#endif // HW_1























