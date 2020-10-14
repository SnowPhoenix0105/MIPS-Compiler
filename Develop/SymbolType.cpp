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
	SymbolType::key_case,		// �ؼ��� case
	SymbolType::key_char,		// �ؼ��� char
	SymbolType::key_const,		// �ؼ��� const
	SymbolType::key_default,	// �ؼ��� default
	SymbolType::key_else,		// �ؼ��� else
	SymbolType::key_for,		// �ؼ��� for
	SymbolType::key_if,			// �ؼ��� if
	SymbolType::key_int,		// �ؼ��� int
	SymbolType::key_main,		// �ؼ��� main
	SymbolType::key_printf,		// �ؼ��� printf
	SymbolType::key_return,		// �ؼ��� return
	SymbolType::key_scanf,		// �ؼ��� scanf
	SymbolType::key_switch,		// �ؼ��� switch
	SymbolType::key_void,		// �ؼ��� void
	SymbolType::key_while		// �ؼ��� while
};


#ifdef DEBUG_LEVEL
const std::unordered_map<SymbolType, wstring> symboltype_translate_dictionary =
{
	{	SymbolType::key_case		,		L"�ؼ��� case"		},
	{	SymbolType::key_char		,		L"�ؼ��� char"		},
	{	SymbolType::key_const		,		L"�ؼ��� const"		},
	{	SymbolType::key_default		,		L"�ؼ��� default"	},
	{	SymbolType::key_else		,		L"�ؼ��� else"		},
	{	SymbolType::key_for			,		L"�ؼ��� for"		},
	{	SymbolType::key_if			,		L"�ؼ��� if"			},
	{	SymbolType::key_int			,		L"�ؼ��� int"		},
	{	SymbolType::key_main		,		L"�ؼ��� main"		},
	{	SymbolType::key_printf		,		L"�ؼ��� printf"		},
	{	SymbolType::key_return		,		L"�ؼ��� return"		},
	{	SymbolType::key_scanf		,		L"�ؼ��� scanf"		},
	{	SymbolType::key_switch		,		L"�ؼ��� switch"		},
	{	SymbolType::key_void		,		L"�ؼ��� void"		},
	{	SymbolType::key_while		,		L"�ؼ��� while"		},

	{	SymbolType::colon			,		L"ð�� :"			},
	{	SymbolType::comma			,		L"���� ,"			},
	{	SymbolType::semicolon		,		L"�ֺ� ;"			},
	{	SymbolType::left_paren		,		L"��Բ���� ("		},
	{	SymbolType::right_paren		,		L"��Բ���� )"		},
	{	SymbolType::left_brance		,		L"������ {"		},
	{	SymbolType::right_brance	,		L"�һ����� },"		},
	{	SymbolType::left_square		,		L"������ ["		},
	{	SymbolType::right_square	,		L"�ҷ����� ]"		},
	{	SymbolType::plus			,		L"�Ӻ� +"			},
	{	SymbolType::minus			,		L"���� -"			},
	{	SymbolType::mult			,		L"�˺� *"			},
	{	SymbolType::div				,		L"���� /"			},

	{	SymbolType::assign			,		L"��ֵ�� ="			},
	{	SymbolType::less			,		L"С�� <"			},
	{	SymbolType::greater			,		L"���� >"			},
	{	SymbolType::less_equal		,		L"С�ڵ��� <="		},
	{	SymbolType::greater_equal	,		L"���ڵ��� >="		},
	{	SymbolType::equal			,		L"���ں� =="			},
	{	SymbolType::not_equal		,		L"���Ⱥ� !="			},

	{	SymbolType::character		,		L"�ַ�"				},
	{	SymbolType::string			,		L"�ַ���"			},
	{	SymbolType::number			,		L"����"				},
	{	SymbolType::identifier		,		L"�����Ϸ���ʶ��"		},

	{	SymbolType::wrong			,		L"����"				}
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
























