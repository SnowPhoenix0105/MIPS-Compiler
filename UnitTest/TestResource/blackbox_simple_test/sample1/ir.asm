	gvar	G_gvar1
	aar	G_a_garr,	char,	5
	init	97
	init	98
	init	99
	init	100
	init	101
	gvar	G_gvar2
	init	18231045
func_beg_main:
	func	void
	aar	main_a_larr1,	char,	6
	init	102
	init	103
	init	104
	init	105
	init	106
	init	107
	aar	main_a_larr2,	int,	2
	aar	main_a_larr3,	int,	4
	init	18231045
	init	10451823
	init	1881110987
	init	-1
func_mid_main:
	$t0	=	$0	+	2
	$t1	=	$t0	+	1
	$t2	=	$sp	+	$t1
	$t3	|=	$t2[main_a_larr1]
	printf	NIL,	$t3,	char
	scanf	$t4,	int
	$t5	=	$t4	+	$0
	$t6	=	$0	+	8
	$t7	=	$sp	+	$t6
	$t7[main_a_larr3]	=	$t5
	$t8	=	$0	+	8
	$t9	=	$sp	+	$t8
	protect	$t0,	t0
	$t0	=	$t9[main_a_larr3]
	protect	$t0,	t10
	protect	$t1,	t1
	reload	$t1,	t10
	$t0	=	$t1	+	$0
	$t1	=	$t0	+	$0
	printf	"your input is ",	$t1,	int
func_end_main:





