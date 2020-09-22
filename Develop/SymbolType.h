#pragma once


#ifndef __SYMBOL_TYPE_H__
#define __SYMBOL_TYPE_H__


#include <unordered_set>
#include <unordered_map>
#include <string>
#include <cctype>
#include "global_control.h"

using std::string;
using std::wstring;

enum class SymbolType
{
	//////////// 关键字 //////////////

	key_case,		// 关键字 case
	key_char,		// 关键字 char
	key_const,		// 关键字 const
	key_default,	// 关键字 default
	key_else,		// 关键字 else
	key_for,		// 关键字 for
	key_if,			// 关键字 if
	key_int,		// 关键字 int
	key_main,		// 关键字 main
	key_printf,		// 关键字 printf
	key_return,		// 关键字 return
	key_scanf,		// 关键字 scanf
	key_switch,		// 关键字 switch
	key_void,		// 关键字 void
	key_while,		// 关键字 while




	///////////// 符号 //////////////

	// 简单符号, 即每个单字符为单独的符号, 并且不是任何其它字符的前缀

	colon,			// 冒号 :
	comma,			// 逗号 ,
	semicolon,		// 分号 ;
	left_paren,		// 左圆括号 (
	right_paren,	// 右圆括号 )
	left_brance,	// 左花括号 {
	right_brance,	// 右花括号 }
	left_square,	// 左方括号 [
	right_square,	// 右方括号 ]
	plus,			// 加号		+
	minus,			// 减号		-
	mult,			// 乘号		*
	div,			// 除号		/

	// 复杂符号, 即需要进行复杂逻辑判断
	assign,			// 赋值号	=
	less,			// 小于		<
	greater,		// 大于		>
	less_equal,		// 小于等于	<=
	greater_equal,	// 大于等于	>=
	equal,			// 等于号	==
	not_equal,		// 不等号	!=

	character,		// 字符
	string,			// 字符串
	number,			// 数字
	names,			// 其它合法标识符



	wrong			// 错误
};

namespace std
{
	template<>
	struct hash<SymbolType>
	{
		using result_type = size_t;
		using argument_type = SymbolType;
		size_t operator() (SymbolType sym) const
		{
			return static_cast<size_t>(sym);
		}
	};
}


#ifdef DEBUG_LEVEL
extern const std::unordered_map<SymbolType, wstring> symboltype_translate_dictionary;
#endif //DEBUG_LEVEL

#ifdef HW_1
extern const std::unordered_map<SymbolType, string> symboltype_output_dictionary;
#endif //HW_1


extern const std::unordered_map<string, SymbolType> saved_key_words_dictionary;

extern const std::unordered_map<char, SymbolType> simple_symbol_dictionary;


/// <summary>
/// 判断是否是合法的 <字母>;
/// 包括大小写字母和下划线 '_'.
/// </summary>
/// <param name="c"></param>
/// <returns></returns>
inline bool is_legal_alpha(char c)
{
	return  c == '_' || std::isalpha(c);
}


/// <summary>
/// 判断是否是合法的 <字符串> 中的字符。
/// </summary>
/// <param name="c"></param>
/// <returns></returns>
inline bool is_legal_string_char(char c)
{
	return c >= ' ' && c <= '~' && c != '\"';
}




#endif // !__SYMBOL_TYPE_H__

