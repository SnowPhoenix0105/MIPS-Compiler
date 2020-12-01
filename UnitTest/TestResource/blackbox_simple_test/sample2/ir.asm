	gvar	G_a1
	init	1
	gvar	G_a2
	init	2
	gvar	G_a3
	init	3
	gvar	G_a4
	init	4
	gvar	G_a5
	init	5
	gvar	G_a12
	gvar	G_a13
	gvar	G_a14
	gvar	G_c1
	init	97
	gvar	G_c2
	init	97
	gvar	G_c3
	init	97
	gvar	G_c4
	init	97
	gvar	G_c5
	init	88
	gvar	G_c6
	init	52
func_beg_main:
	func	void
func_mid_main:
	$t0	=	$0	+	101
	$t1	=	$0	+	49
	$t2	=	$0	+	6
	$t3	=	$0	+	7
	$t4	=	$0	+	8
	$t5	=	$0	+	9
	$t6	=	$0	+	10
	$t7	=	$0	+	11
	scanf	$t8,	int
	printf	NIL,	$t8,	int
	$t9	=	$0	-	51
	protect	$t0,	main_c9
	$t0	=	$t9	*	3
	protect	$t0,	t2
	protect	$t1,	main_c10
	reload	$t1,	t2
	$t0	=	$t1	*	$t8
	$t1	=	$t0	+	$0
	$t8	=	$t1	+	$0
	printf	NIL,	$t8,	int
	scanf	$t2,	int
	protect	$t0,	t3
	$t0	=	$0	+	18
	$t3	=	$t0	+	$0
	scanf	$t3,	int
	protect	$t0,	t4
	scanf	$t0,	char
	scanf	$t0,	char
	protect	$t0,	main_c7
	protect	$t1,	t0
	reload	$t1,	G_a2
	$t0	=	$0	-	$t1
	$t1	=	$t0	+	$0
	$t8	=	$t1	+	$0
	printf	NIL,	$t8,	int
	protect	$t0,	t6
	$t0	=	$t8	*	$t8
	protect	$t0,	t8
	protect	$t1,	t5
	reload	$t1,	t8
	$t0	=	$t1	+	$0
	$t8	=	$t0	+	$0
	printf	NIL,	$t8,	int
	$t1	=	$t8	+	$0
	$t8	=	$t1	+	$0
	printf	NIL,	$t8,	int
	protect	$t0,	t7
	$t0	=	$0	-	97
	protect	$t0,	t11
	$t0	=	$0	-	97
	protect	$t0,	t12
	$t0	=	$0	-	97
	protect	$t0,	t13
	protect	$t1,	t9
	reload	$t1,	t12
	protect	$t2,	main_a6
	reload	$t2,	t13
	$t0	=	$t1	-	$t2
	reload	$t2,	t11
	$t1	=	$t2	-	$t0
	$t2	=	$0	+	6
	protect	$t0,	t14
	$t0	=	$t2	*	43
	protect	$t0,	t17
	protect	$t2,	t16
	reload	$t2,	t17
	$t0	=	$t1	-	$t2
	protect	$t0,	t18
	reload	$t0,	G_a1
	$t2	=	$t0	+	14
	$t0	=	$t2	*	13
	protect	$t0,	t20
	protect	$t1,	t15
	reload	$t1,	t18
	protect	$t2,	t19
	reload	$t2,	t20
	$t0	=	$t1	+	$t2
	$t1	=	$t0	+	$0
	$t8	=	$t1	+	$0
	printf	NIL,	$t8,	int
	$t2	=	$0	+	7
	protect	$t0,	t21
	$t0	=	$t2	+	5
	protect	$t0,	t23
	protect	$t1,	t10
	reload	$t1,	t23
	$t0	=	$t1	+	6
	$t1	=	$t0	-	7
	protect	$t0,	t24
	$t0	=	$t1	-	8
	protect	$t0,	t26
	$t0	=	$0	+	36
	protect	$t0,	t27
	protect	$t1,	t25
	reload	$t1,	t27
	$t0	=	$t1	*	4
	$t1	=	$t0	/	3
	protect	$t0,	t28
	$t0	=	$t1	/	2
	protect	$t0,	t30
	$t0	=	$0	+	5
	protect	$t0,	t31
	protect	$t1,	t29
	reload	$t1,	t31
	protect	$t2,	t22
	reload	$t2,	t30
	$t0	=	$t1	*	$t2
	reload	$t2,	t26
	$t1	=	$t2	-	$t0
	printf	NIL,	$t1,	int
	protect	$t0,	t32
	reload	$t0,	main_c10
	$t2	=	$t0	+	$0
	$t0	=	$t2	+	$0
	protect	$t0,	main_c8
	$t0	=	$t0	+	$0
	protect	$t0,	t35
	protect	$t1,	t33
	reload	$t1,	t35
	$t0	=	$t1	+	$0
	$t1	=	$0	+	12
	$t0	=	$t1	+	$0
	protect	$t0,	main_c4
	$t0	=	$0	+	13
	$t1	=	$t0	+	$0
	protect	$t0,	t37
	$t0	=	$0	+	14
	$t8	=	$t0	+	$0
	protect	$t0,	t38
	$t0	=	$0	+	15
	protect	$t0,	t39
	protect	$t1,	t36
	reload	$t1,	t39
	$t0	=	$t1	+	$0
	$t1	=	$0	+	16
	protect	$t0,	main_a4
	$t0	=	$t1	+	$0
	$t0	=	$0	+	17
	protect	$t0,	t41
	protect	$t1,	t40
	reload	$t1,	t41
	$t0	=	$t1	+	$0
	$t1	=	$0	+	19
	$t4	=	$t1	+	$0
	protect	$t0,	main_a6
	$t0	=	$t5	+	20
	protect	$t0,	t44
	protect	$t1,	t42
	reload	$t1,	t44
	$t0	=	$t1	+	$0
	$t5	=	$t0	+	$0
	$t1	=	$t0	+	21
	protect	$t0,	t43
	$t0	=	$t1	+	$0
	$t6	=	$t0	+	$0
	protect	$t0,	t45
	$t0	=	$t0	+	$t6
	protect	$t0,	t48
	protect	$t1,	t46
	reload	$t1,	t48
	$t0	=	$t1	+	$0
	$t7	=	$t0	+	$0
	printf	NIL,	97,	char
	printf	NIL,	$t0,	int
	$t1	=	$t0	+	97
	printf	NIL,	$t1,	int
	protect	$t0,	t47
	$t0	=	$0	-	5
	protect	$t0,	t50
	protect	$t1,	t49
	reload	$t1,	t50
	$t0	=	$t1	*	5
	$t1	=	$0	-	$t0
	printf	NIL,	$t1,	int
	protect	$t0,	t51
	reload	$t0,	main_a4
	printf	"a4",	$t0,	int
	printf	NIL,	3,	int
	printf	"18231217",	18231217,	int
	printf	NIL,	49,	char
	printf	NIL,	57,	char
	$t0	=	$0	-	49
	protect	$t0,	t53
	$t0	=	$t8	*	57
	protect	$t0,	t54
	protect	$t1,	t52
	reload	$t1,	t53
	protect	$t2,	t34
	reload	$t2,	t54
	$t0	=	$t1	-	$t2
	$t1	=	$0	-	3
	$t2	=	$t1	*	14
	protect	$t0,	t55
	protect	$t1,	t56
	reload	$t1,	t55
	$t0	=	$t1	+	$t2
	$t1	=	$0	+	-1
	protect	$t0,	t58
	$t0	=	$t1	+	2
	protect	$t0,	t60
	protect	$t1,	t59
	reload	$t1,	t60
	$t0	=	$t1	-	52
	$t1	=	$t0	+	60
	protect	$t0,	t61
	$t0	=	$t1	*	4
	protect	$t0,	t63
	protect	$t1,	t62
	reload	$t1,	t63
	$t0	=	$t1	/	1
	$t1	=	$t0	/	1
	protect	$t0,	t64
	$t0	=	$t1	/	1
	protect	$t0,	t66
	protect	$t1,	t65
	reload	$t1,	t58
	protect	$t2,	t57
	reload	$t2,	t66
	$t0	=	$t1	+	$t2
	printf	NIL,	$t0,	int
	printf	NIL,	$t0,	int
	printf	NIL,	$t1,	int
	printf	NIL,	$t8,	int
	printf	NIL,	$t4,	int
	printf	NIL,	$t5,	int
	printf	NIL,	$t6,	int
	printf	NIL,	$t7,	int
	printf	"134214321432143214321",	NIL,	char
	$t1	=	$0	-	$t1
	$t2	=	$t1	+	$t0
	protect	$t0,	t67
	$t0	=	$t2	*	$t0
	protect	$t0,	t70
	$t0	=	$t1	+	$t0
	protect	$t0,	t71
	protect	$t1,	t68
	reload	$t1,	t71
	$t0	=	$t1	+	2
	protect	$t0,	t72
	reload	$t0,	t70
	protect	$t2,	t69
	reload	$t2,	t72
	$t1	=	$t0	/	$t2
	$t0	=	$t8	-	$t1
	$t2	=	$0	-	$t0
	protect	$t0,	t74
	$t0	=	$t2	*	$t0
	protect	$t0,	t76
	protect	$t1,	t73
	reload	$t1,	t74
	protect	$t2,	t75
	reload	$t2,	t76
	$t0	=	$t1	-	$t2
	$t1	=	$0	+	15
	$t2	=	$t0	+	$t1
	protect	$t0,	t77
	protect	$t1,	t78
	reload	$t1,	main_a6
	$t0	=	$t2	+	$t1
	$t1	=	$t0	*	$t7
	protect	$t0,	t80
	$t0	=	$0	-	$t1
	protect	$t0,	t82
	protect	$t1,	t81
	reload	$t1,	main_a4
	$t0	=	$0	-	$t1
	$t1	=	$t0	+	$t1
	protect	$t0,	t83
	$t0	=	$t1	*	$t8
	protect	$t0,	t85
	protect	$t1,	t84
	reload	$t1,	t82
	protect	$t2,	t79
	reload	$t2,	t85
	$t0	=	$t1	+	$t2
	$t1	=	$t0	+	$t3
	printf	NIL,	$t1,	int
	protect	$t0,	t86
	reload	$t0,	main_a6
	protect	$t1,	t87
	reload	$t1,	main_c9
	$t2	=	$t0	*	$t1
	$t0	=	$t2	+	47
	protect	$t0,	t90
	reload	$t0,	main_a4
	protect	$t2,	t89
	reload	$t2,	t90
	$t1	=	$t0	+	$t2
	$t0	=	$t5	*	50
	$t2	=	$0	+	588
	protect	$t0,	t92
	$t0	=	$t2	*	10
	protect	$t0,	t94
	protect	$t1,	t91
	reload	$t1,	t94
	$t0	=	$t1	/	4
	protect	$t0,	t95
	reload	$t0,	t92
	protect	$t2,	t93
	reload	$t2,	t95
	$t1	=	$t0	-	$t2
	$t0	=	$t1	*	3
	$t2	=	$0	+	45
	protect	$t0,	t97
	$t0	=	$0	-	$t2
	protect	$t0,	t99
	$t0	=	$t0	+	$t1
	protect	$t0,	t100
	protect	$t1,	t96
	reload	$t1,	t99
	protect	$t2,	t98
	reload	$t2,	t100
	$t0	=	$t1	-	$t2
	$t1	=	$0	+	69
	protect	$t1,	t102
	reload	$t1,	main_a4
	$t2	=	$t0	+	$t1
	protect	$t0,	t101
	reload	$t0,	G_c3
	$t1	=	$t0	+	$t5
	$t0	=	$t1	*	$t2
	protect	$t0,	t105
	protect	$t1,	t104
	reload	$t1,	t105
	$t0	=	$t1	/	100
	protect	$t0,	t106
	reload	$t0,	t102
	protect	$t2,	t103
	reload	$t2,	t106
	$t1	=	$t0	-	$t2
	$t0	=	$t5	-	14
	protect	$t1,	t107
	reload	$t1,	main_a6
	$t2	=	$t0	*	$t1
	$t1	=	$0	+	342
	protect	$t0,	t108
	$t0	=	$t1	-	$t2
	protect	$t0,	t110
	protect	$t1,	t167
	reload	$t1,	t110
	protect	$t2,	t109
	reload	$t2,	t107
	$t0	=	$t1	*	$t2
	$t1	=	$t0	/	4
	protect	$t0,	t111
	reload	$t0,	t101
	$t2	=	$t0	-	$t1
	protect	$t1,	t112
	reload	$t1,	t97
	$t0	=	$t2	*	$t1
	protect	$t0,	t114
	reload	$t0,	t91
	protect	$t2,	t113
	reload	$t2,	t114
	$t1	=	$t0	-	$t2
	$t0	=	$t1	*	97
	$t2	=	$t0	+	42
	protect	$t0,	t116
	$t0	=	$0	-	$t2
	protect	$t0,	t118
	protect	$t1,	t115
	reload	$t1,	t118
	$t0	=	$t1	-	0
	$t1	=	$t0	*	4
	protect	$t0,	t119
	protect	$t1,	t120
	reload	$t1,	t119
	protect	$t2,	t117
	reload	$t2,	t120
	$t0	=	$t1	+	$t2
	$t1	=	$t0	+	$0
	$t0	=	$t1	+	$0
	printf	NIL,	$t0,	int
	reload	$t2,	main_a4
	printf	NIL,	$t2,	int
	printf	NIL,	$t4,	int
	printf	NIL,	$t0,	char
	$t2	=	$t0	+	$t0
	protect	$t0,	t121
	$t0	=	$t2	+	$0
	$t1	=	$t0	+	$0
	protect	$t0,	t122
	$t0	=	$t6	+	$t0
	protect	$t0,	t125
	$t0	=	$0	+	-36000
	protect	$t0,	t168
	protect	$t1,	t88
	reload	$t1,	t168
	protect	$t2,	t123
	reload	$t2,	t125
	$t0	=	$t1	/	$t2
	$t1	=	$t1	-	4
	$t2	=	$t1	*	12
	protect	$t0,	t126
	$t0	=	$t2	*	13
	protect	$t0,	t129
	protect	$t1,	t127
	reload	$t1,	main_a6
	$t0	=	$t1	+	24
	protect	$t2,	t128
	reload	$t2,	main_a4
	$t1	=	$t0	*	$t2
	$t2	=	$t1	/	49
	protect	$t0,	t130
	$t0	=	$t2	*	10
	protect	$t0,	t133
	protect	$t1,	t131
	reload	$t1,	t129
	protect	$t2,	t132
	reload	$t2,	t133
	$t0	=	$t1	+	$t2
	$t1	=	$t0	-	14
	$t2	=	$t1	*	42
	protect	$t0,	t134
	$t0	=	$t1	+	$t2
	protect	$t0,	t137
	$t0	=	$t5	*	14
	protect	$t0,	t138
	protect	$t1,	t135
	reload	$t1,	t137
	protect	$t2,	t136
	reload	$t2,	t138
	$t0	=	$t1	-	$t2
	$t1	=	$t0	+	$t0
	protect	$t0,	t139
	reload	$t0,	G_c2
	$t2	=	$t1	+	$t0
	protect	$t1,	t140
	reload	$t1,	main_a6
	$t0	=	$t1	+	$t2
	$t1	=	$t0	-	$t8
	protect	$t0,	t142
	$t0	=	$0	+	61
	protect	$t0,	t144
	protect	$t1,	t143
	reload	$t1,	t144
	$t0	=	$t1	*	$t1
	$t1	=	$t0	/	12
	protect	$t0,	t145
	$t0	=	$t0	-	$t1
	protect	$t0,	t147
	protect	$t1,	t146
	reload	$t1,	t147
	$t0	=	$t8	+	$t1
	$t1	=	$t0	+	42
	protect	$t0,	t148
	protect	$t2,	t141
	reload	$t2,	t143
	$t0	=	$t1	*	$t2
	protect	$t0,	t150
	reload	$t0,	t142
	protect	$t1,	t149
	reload	$t1,	t150
	$t2	=	$t0	+	$t1
	$t0	=	$t2	+	0
	$t1	=	$t0	*	-4
	protect	$t0,	t152
	$t0	=	$0	+	288
	protect	$t0,	t154
	protect	$t2,	t151
	reload	$t2,	t154
	$t0	=	$t1	+	$t2
	$t2	=	$t0	*	$t8
	protect	$t0,	t155
	$t0	=	$t2	+	$t0
	protect	$t0,	t157
	protect	$t1,	t153
	reload	$t1,	t157
	$t0	=	$t1	*	2
	protect	$t0,	t158
	reload	$t0,	t155
	protect	$t2,	t156
	reload	$t2,	t158
	$t1	=	$t0	+	$t2
	reload	$t2,	t126
	$t0	=	$t2	/	$t1
	protect	$t0,	t160
	reload	$t0,	main_a4
	$t2	=	$t0	-	12
	$t0	=	$t2	+	$t0
	protect	$t0,	t162
	protect	$t1,	t159
	reload	$t1,	t162
	$t0	=	$t1	*	$t8
	$t1	=	$t1	+	2
	protect	$t0,	t163
	protect	$t1,	t164
	reload	$t1,	t163
	protect	$t2,	t161
	reload	$t2,	t164
	$t0	=	$t1	/	$t2
	reload	$t2,	t160
	$t1	=	$t2	+	$t0
	$t2	=	$t1	+	$0
	$t8	=	$t2	+	$0
	printf	NIL,	$t8,	int
	printf	"yes",	$t1,	int
	protect	$t0,	t165
	reload	$t0,	main_a4
	printf	NIL,	$t0,	int
	printf	NIL,	-1,	int
func_end_main:





