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
	t0	=	1	*	2
	t1	=	t0	+	1
	t2	=	#sp	+	t1
	t3	|=	t2[main_a_larr1]
	printf	NIL,	t3,	char
	scanf	main_buffer,	int
	t4	=	main_buffer	+	#zero
	t6	=	2	<<	2
	t5	=	#sp	+	t6
	t5[main_a_larr3]	=	t4
	t9	=	2	<<	2
	t8	=	#sp	+	t9
	t10	=	t8[main_a_larr3]
	t7	=	t10	+	#zero
	main_tmp	=	t7	+	#zero
	printf	"your input is ",	main_tmp,	int
func_end_main:





