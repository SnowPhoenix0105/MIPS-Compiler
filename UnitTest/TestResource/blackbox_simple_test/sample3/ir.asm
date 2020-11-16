func_beg_main:
	func	void
func_mid_main:
	main_var_a1	=	$0	+	4
	main_tmp_1	=	main_var_a1	*	97
	main_tmp_0	=	main_tmp_1	+	$0
	main_var_a1	=	main_tmp_0	+	$0
	printf	NIL,	main_var_a1,	int
	main_tmp_2	=	4	+	$0
	main_var_a1	=	main_tmp_2	+	$0
	main_tmp_4	=	main_var_a1	*	97
	main_tmp_5	=	$0	-	main_tmp_4
	main_tmp_3	=	main_tmp_5	+	$0
	main_var_a1	=	main_tmp_3	+	$0
	printf	NIL,	main_var_a1,	int
	main_tmp_6	=	4	+	$0
	main_var_a1	=	main_tmp_6	+	$0
	main_tmp_8	=	97	*	-1
	main_tmp_9	=	main_var_a1	*	main_tmp_8
	main_tmp_7	=	main_tmp_9	+	$0
	main_var_a1	=	main_tmp_7	+	$0
	printf	NIL,	main_var_a1,	int
func_end_main:

