func_beg_main:
	func	void
func_mid_main:
	$t0	=	$0	+	4
	$t1	=	$t0	*	97
	$t2	=	$t1	+	$0
	$t0	=	$t2	+	$0
	printf	NIL,	$t0,	int
	$t3	=	$0	+	4
	$t0	=	$t3	+	$0
	$t4	=	$t0	*	97
	$t5	=	$0	-	$t4
	$t6	=	$t5	+	$0
	$t0	=	$t6	+	$0
	printf	NIL,	$t0,	int
	$t7	=	$0	+	4
	$t0	=	$t7	+	$0
	$t8	=	$0	+	-97
	$t9	=	$t0	*	$t8
	protect	$t0,	main_a1
	$t0	=	$t9	+	$0
	protect	$t0,	t7
	protect	$t1,	t1
	reload	$t1,	t7
	$t0	=	$t1	+	$0
	printf	NIL,	$t0,	int
func_end_main:





