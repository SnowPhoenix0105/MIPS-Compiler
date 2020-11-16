	gvar	__global_var_gvar1
	aar	__global__arr__garr,	char,	5
	init	97
	init	98
	init	99
	init	100
	init	101
	gvar	__global_var_gvar2
	init	18231045
func_beg_main:
	func	void
	aar	main__arr__larr1,	char,	6
	init	102
	init	103
	init	104
	init	105
	init	106
	init	107
	aar	main__arr__larr2,	char,	2
	aar	main__arr__larr3,	char,	4
	init	18231045
	init	10451823
	init	1881110987
	init	-1
func_mid_main:
	main_tmp_0	=	1	*	2
	main_tmp_1	=	main_tmp_0	+	1
	main_tmp_2	=	$sp	+	main_tmp_1
	main_tmp_3	|=	main_tmp_2[main__arr__larr1]
	printf	NIL,	main_tmp_3,	char
	scanf	main_var_buffer,	int
	main_tmp_4	=	main_var_buffer	+	$0
	main_tmp_6	=	2	<<	2
	main_tmp_5	=	$sp	+	main_tmp_6
	main_tmp_5[main__arr__larr3]	=	main_tmp_4
	main_tmp_9	=	2	<<	2
	main_tmp_8	=	$sp	+	main_tmp_9
	main_tmp_10	=	main_tmp_8[main__arr__larr3]
	main_tmp_7	=	main_tmp_10	+	$0
	main_var_tmp	=	main_tmp_7	+	$0
	printf	"your input is\t",	main_var_tmp,	int
func_end_main:

