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
	SymbolType::key_case,		// ¹Ø¼ü×Ö case
	SymbolType::key_char,		// ¹Ø¼ü×Ö char
	SymbolType::key_const,		// ¹Ø¼ü×Ö const
	SymbolType::key_default,	// ¹Ø¼ü×Ö default
	SymbolType::key_else,		// ¹Ø¼ü×Ö else
	SymbolType::key_for,		// ¹Ø¼ü×Ö for
	SymbolType::key_if,			// ¹Ø¼ü×Ö if
	SymbolType::key_int,		// ¹Ø¼ü×Ö int
	SymbolType::key_main,		// ¹Ø¼ü×Ö main
	SymbolType::key_printf,		// ¹Ø¼ü×Ö printf
	SymbolType::key_return,		// ¹Ø¼ü×Ö return
	SymbolType::key_scanf,		// ¹Ø¼ü×Ö scanf
	SymbolType::key_switch,		// ¹Ø¼ü×Ö switch
	SymbolType::key_void,		// ¹Ø¼ü×Ö void
	SymbolType::key_while		// ¹Ø¼ü×Ö while
};


#ifdef DEBUG_LEVEL
const std::unordered_map<SymbolType, wstring> symboltype_translate_dictionary =
{
	{	SymbolType::key_case		,		L"¹Ø¼ü×Ö case"		},
	{	SymbolType::key_char		,		L"¹Ø¼ü×Ö char"		},
	{	SymbolType::key_const		,		L"¹Ø¼ü×Ö const"		},
	{	SymbolType::key_default		,		L"¹Ø¼ü×Ö default"	},
	{	SymbolType::key_else		,		L"¹Ø¼ü×Ö else"		},
	{	SymbolType::key_for			,		L"¹Ø¼ü×Ö for"		},
	{	SymbolType::key_if			,		L"¹Ø¼ü×Ö if"			},
	{	SymbolType::key_int			,		L"¹Ø¼ü×Ö int"		},
	{	SymbolType::key_main		,		L"¹Ø¼ü×Ö main"		},
	{	SymbolType::key_printf		,		L"¹Ø¼ü×Ö printf"		},
	{	SymbolType::key_return		,		L"¹Ø¼ü×Ö return"		},
	{	SymbolType::key_scanf		,		L"¹Ø¼ü×Ö scanf"		},
	{	SymbolType::key_switch		,		L"¹Ø¼ü×Ö switch"		},
	{	SymbolType::key_void		,		L"¹Ø¼ü×Ö void"		},
	{	SymbolType::key_while		,		L"¹Ø¼ü×Ö while"		},

	{	SymbolType::colon			,		L"Ã°ºÅ :"			},
	{	SymbolType::comma			,		L"¶ººÅ ,"			},
	{	SymbolType::semicolon		,		L"·ÖºÅ ;"			},
	{	SymbolType::left_paren		,		L"×óÔ²À¨ºÅ ("		},
	{	SymbolType::right_paren		,		L"ÓÒÔ²À¨ºÅ )"		},
	{	SymbolType::left_brance		,		L"×ó»¨À¨ºÅ {"		},
	{	SymbolType::right_brance	,		L"ÓÒ»¨À¨ºÅ },"		},
	{	SymbolType::left_square		,		L"×ó·½À¨ºÅ ["		},
	{	SymbolType::right_square	,		L"ÓÒ·½À¨ºÅ ]"		},
	{	SymbolType::plus			,		L"¼ÓºÅ +"			},
	{	SymbolType::minus			,		L"¼õºÅ -"			},
	{	SymbolType::mult			,		L"³ËºÅ *"			},
	{	SymbolType::div				,		L"³ýºÅ /"			},

	{	SymbolType::assign			,		L"¸³ÖµºÅ ="			},
	{	SymbolType::less			,		L"Ð¡ÓÚ <"			},
	{	SymbolType::greater			,		L"´óÓÚ >"			},
	{	SymbolType::less_equal		,		L"Ð¡ÓÚµÈÓÚ <="		},
	{	SymbolType::greater_equal	,		L"´óÓÚµÈÓÚ >="		},
	{	SymbolType::equal			,		L"µÈÓÚºÅ =="			},
	{	SymbolType::not_equal		,		L"²»µÈºÅ !="			},

	{	SymbolType::character		,		L"×Ö·û"				},
	{	SymbolType::string			,		L"×Ö·û´®"			},
	{	SymbolType::number			,		L"Êý×Ö"				},
	{	SymbolType::identifier		,		L"ÆäËüºÏ·¨±êÊ¶·û"		},

	{	SymbolType::wrong			,		L"´íÎó"				}
};
#endif // DEBUG_LEVEL


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
























