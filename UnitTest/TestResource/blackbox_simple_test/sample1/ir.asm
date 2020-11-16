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
	main_tmp_0	=	1	*	1
	main_tmp_1	=	$0	+	main_tmp_0
	main_tmp_2	=	1	*	1
	main_tmp_3	=	$0	+	main_tmp_2
	main_tmp_4	=	main_tmp_1	*	2
	main_tmp_5	=	main_tmp_4	+	main_tmp_3
	main_tmp_6	=	$sp	+	main_tmp_5
	main_tmp_7	|=	main_tmp_6[main__arr__larr1]
	main_tmp_8	=	main_tmp_7	*	1
	main_tmp_9	=	$0	+	main_tmp_8
	printf	NIL,	main_tmp_9,	char
	scanf	main_var_buffer,	int
	main_tmp_15	=	main_var_buffer	*	1
	main_tmp_16	=	$0	+	main_tmp_15
	main_tmp_10	=	main_tmp_16	+	$0
	main_tmp_11	=	2	*	1
	main_tmp_12	=	$0	+	main_tmp_11
	main_tmp_14	=	main_tmp_12	<<	2
	main_tmp_13	=	$sp	+	main_tmp_14
	main_tmp_13[main__arr__larr3]	=	main_tmp_10
	main_tmp_18	=	2	*	1
	main_tmp_19	=	$0	+	main_tmp_18
	main_tmp_21	=	main_tmp_19	<<	2
	main_tmp_20	=	$sp	+	main_tmp_21
	main_tmp_22	=	main_tmp_20[main__arr__larr3]
	main_tmp_23	=	main_tmp_22	*	1
	main_tmp_24	=	$0	+	main_tmp_23
	main_tmp_17	=	main_tmp_24	+	$0
	main_var_tmp	=	main_tmp_17	+	$0
	main_tmp_25	=	main_var_tmp	*	1
	main_tmp_26	=	$0	+	main_tmp_25
	printf	"your input is\t",	main_tmp_26,	int
func_end_main:

