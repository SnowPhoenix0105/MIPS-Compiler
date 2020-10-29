#include "ErrorType.h"

unordered_map<ErrorType, const char> error_code_dictionary =
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
	{ErrorType::constant_type_mismatching 			, 'o'},
	{ErrorType::switch_no_defaule					, 'p'},
	{ErrorType::unknown_error						, 'z'},
};


unordered_map<ErrorType, const string> error_message =
{
	{ErrorType::lexical_error						, "�Ƿ����Ż򲻷��ϴʷ�"							},
	{ErrorType::duplicated_identifier				, "�����ض���"									},
	{ErrorType::undefined_identifier				, "δ��������"									},
	{ErrorType::parameter_count_mismatching			, "��������������ƥ��"							},
	{ErrorType::parameter_type_mismatching			, "�������Ͳ�ƥ��"								},
	{ErrorType::illegal_type_in_condition			, "�����ж��г��ֲ��Ϸ�������"						},
	{ErrorType::return_value_in_void_function		, "�޷���ֵ�������ڲ�ƥ���return���"				},
	{ErrorType::wrong_return_in_return_function		, "�з���ֵ����ȱ��return����ڲ�ƥ���return"		},
	{ErrorType::non_int_index_for_array				, "�����±겻�����ͱ��ʽ"						},
	{ErrorType::try_change_const_value				, "��ͼ�ı䳣����ֵ"								},
	{ErrorType::need_semicolon						, "��Ҫ�ֺ�"										},
	{ErrorType::need_right_paren					, "��Ҫ��С����"									},
	{ErrorType::need_right_square					, "��Ҫ��������"									},
	{ErrorType::array_initialize_mismatching		, "�����ʼ��������ƥ��"							},
	{ErrorType::constant_type_mismatching 			, "switch�����case���Ͳ���ȷ"					},
	{ErrorType::switch_no_defaule					, "ȱ��ȱʡ���"									},
	{ErrorType::unknown_error						, "δ֪����"										}
};