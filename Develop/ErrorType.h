#pragma once

#ifndef __ERROR_TYPE_H__
#define __ERROR_TYPE_H__

#include <unordered_map>
#include <string>

using std::string;
using std::unordered_map;

enum class ErrorType : size_t
{
	lexical_error,						// a	�Ƿ����Ż򲻷��ϴʷ�
	duplicated_identifier,				// b	�����ض���
	undefined_identifier,				// c	δ��������
	parameter_count_mismatching,		// d	��������������ƥ��
	parameter_type_mismatching,			// e	�������Ͳ�ƥ��
	illegal_type_in_condition,			// f	�����ж��г��ֲ��Ϸ�������
	return_value_in_void_function,		// g	�޷���ֵ�������ڲ�ƥ���return���
	wrong_return_in_return_function,	// h	�з���ֵ����ȱ��return����ڲ�ƥ���return
	non_int_index_for_array,			// i	�����±겻�����ͱ��ʽ
	try_change_const_value,				// j	��ͼ�ı䳣����ֵ
	need_semicolon,						// k	��Ҫ�ֺ�
	need_right_paren,					// l	��Ҫ��С����
	need_right_square,					// m	��Ҫ��������
	array_initialize_mismatching,		// n	�����ʼ��������ƥ��
	constant_type_mismatching,			// o	�������Ͳ�ƥ��
	switch_no_defaule,					// p	ȱ��ȱʡ���

	unknown_error,						// z	δ֪����
};

namespace std
{
	template<>
	struct hash<ErrorType>
	{
		using result_type = size_t;
		using argument_type = ErrorType;
		size_t operator() (ErrorType sym) const
		{
			return static_cast<size_t>(sym);
		}
	};
}

extern unordered_map<ErrorType, const char> error_code_dictionary;

extern unordered_map<ErrorType, const string> error_message;

#endif // !__ERROR_TYPE_H__
