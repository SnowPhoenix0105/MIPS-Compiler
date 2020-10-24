
#ifndef __ERROR_TYPE_H__
#define __ERROR_TYPE_H__

#include <unordered_map>
#include <string>

using std::string;
using std::unordered_map;

enum class ErrorType : size_t
{
	lexical_error,						// a	非法符号或不符合词法
	duplicated_identifier,				// b	名字重定义
	undefined_identifier,				// c	未定义名字
	parameter_count_mismatching,		// d	函数参数个数不匹配
	parameter_type_mismatching,			// e	函数类型不匹配
	illegal_type_in_condition,			// f	条件判断中出现不合法的类型
	return_value_in_void_function,		// g	无返回值函数存在不匹配的return语句
	wrong_return_in_return_function,	// h	有返回值函数缺少return或存在不匹配的return
	non_int_index_for_array,			// i	数组下标不是整型表达式
	try_change_const_value,				// j	试图改变常量的值
	need_semicolon,						// k	需要分号
	need_right_paren,					// l	需要右小括号
	need_right_square,					// m	需要右中括号
	array_initialize_mismatching,		// n	数组初始化个数不匹配
	switch_type_mismatching,			// o	switch语句中case类型不正确
	switch_no_defaule,					// p	缺少缺省语句

	unknown_error,						// z	未知错误
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

extern unordered_map<ErrorType, char> error_code_dictionary;

extern unordered_map<ErrorType, string> error_message;

#endif // !__ERROR_TYPE_H__
