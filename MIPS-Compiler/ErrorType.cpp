#include "ErrorType.h"

unordered_map<ErrorType, char> error_code_dictionary =
{
	{ErrorType::lexical_error						, 'a'},
	{ErrorType::duplicated_identifier				, 'b'},
	{ErrorType::undefined_identifier				, 'c'},
	{ErrorType::parameter_count_mismatching			, 'd'},
	{ErrorType::parameter_type_mismatching			, 'e'},
	{ErrorType::illegal_type_in_condition			, 'f'},
	{ErrorType::return_value_in_void_function		, 'g'},
	{ErrorType::wrong_return_in_return_function		, 'h'},
	{ErrorType::non_int_index_for_array				, 'i'},
	{ErrorType::try_change_const_value				, 'j'},
	{ErrorType::need_semicolon						, 'k'},
	{ErrorType::need_right_paren					, 'l'},
	{ErrorType::need_right_square					, 'm'},
	{ErrorType::array_initialize_mismatching		, 'n'},
	{ErrorType::switch_type_mismatching 			, 'o'},
	{ErrorType::switch_no_defaule					, 'p'},
	{ErrorType::unknown_error						, 'z'},
};


unordered_map<ErrorType, string> error_message =
{
	{ErrorType::lexical_error						, "非法符号或不符合词法"							},
	{ErrorType::duplicated_identifier				, "名字重定义"									},
	{ErrorType::undefined_identifier				, "未定义名字"									},
	{ErrorType::parameter_count_mismatching			, "函数参数个数不匹配"							},
	{ErrorType::parameter_type_mismatching			, "函数类型不匹配"								},
	{ErrorType::illegal_type_in_condition			, "条件判断中出现不合法的类型"						},
	{ErrorType::return_value_in_void_function		, "无返回值函数存在不匹配的return语句"				},
	{ErrorType::wrong_return_in_return_function		, "有返回值函数缺少return或存在不匹配的return"		},
	{ErrorType::non_int_index_for_array				, "数组下标不是整型表达式"						},
	{ErrorType::try_change_const_value				, "试图改变常量的值"								},
	{ErrorType::need_semicolon						, "需要分号"										},
	{ErrorType::need_right_paren					, "需要右小括号"									},
	{ErrorType::need_right_square					, "需要右中括号"									},
	{ErrorType::array_initialize_mismatching		, "数组初始化个数不匹配"							},
	{ErrorType::switch_type_mismatching 			, "switch语句中case类型不正确"					},
	{ErrorType::switch_no_defaule					, "缺少缺省语句"									},
	{ErrorType::unknown_error						, "未知错误"										}
};