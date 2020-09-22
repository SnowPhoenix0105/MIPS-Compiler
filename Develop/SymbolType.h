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
	//////////// �ؼ��� //////////////

	key_case,		// �ؼ��� case
	key_char,		// �ؼ��� char
	key_const,		// �ؼ��� const
	key_default,	// �ؼ��� default
	key_else,		// �ؼ��� else
	key_for,		// �ؼ��� for
	key_if,			// �ؼ��� if
	key_int,		// �ؼ��� int
	key_main,		// �ؼ��� main
	key_printf,		// �ؼ��� printf
	key_return,		// �ؼ��� return
	key_scanf,		// �ؼ��� scanf
	key_switch,		// �ؼ��� switch
	key_void,		// �ؼ��� void
	key_while,		// �ؼ��� while




	///////////// ���� //////////////

	// �򵥷���, ��ÿ�����ַ�Ϊ�����ķ���, ���Ҳ����κ������ַ���ǰ׺

	colon,			// ð�� :
	comma,			// ���� ,
	semicolon,		// �ֺ� ;
	left_paren,		// ��Բ���� (
	right_paren,	// ��Բ���� )
	left_brance,	// ������ {
	right_brance,	// �һ����� }
	left_square,	// ������ [
	right_square,	// �ҷ����� ]
	plus,			// �Ӻ�		+
	minus,			// ����		-
	mult,			// �˺�		*
	div,			// ����		/

	// ���ӷ���, ����Ҫ���и����߼��ж�
	assign,			// ��ֵ��	=
	less,			// С��		<
	greater,		// ����		>
	less_equal,		// С�ڵ���	<=
	greater_equal,	// ���ڵ���	>=
	equal,			// ���ں�	==
	not_equal,		// ���Ⱥ�	!=

	character,		// �ַ�
	string,			// �ַ���
	number,			// ����
	names,			// �����Ϸ���ʶ��



	wrong			// ����
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
/// �ж��Ƿ��ǺϷ��� <��ĸ>;
/// ������Сд��ĸ���»��� '_'.
/// </summary>
/// <param name="c"></param>
/// <returns></returns>
inline bool is_legal_alpha(char c)
{
	return  c == '_' || std::isalpha(c);
}


/// <summary>
/// �ж��Ƿ��ǺϷ��� <�ַ���> �е��ַ���
/// </summary>
/// <param name="c"></param>
/// <returns></returns>
inline bool is_legal_string_char(char c)
{
	return c >= ' ' && c <= '~' && c != '\"';
}




#endif // !__SYMBOL_TYPE_H__

