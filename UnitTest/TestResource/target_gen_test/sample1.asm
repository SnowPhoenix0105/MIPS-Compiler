.data
__GP__:
.word
.byte 97
.byte 98
.byte 99
.byte 100
.byte 101
.byte 0
.byte 0
.byte 0
.space 16
.word	18231045
string_0:
.asciiz "your input is	"





.text
	j	func_beg_main

func_beg_main:
	la	$gp,__GP__
	addiu	$sp,$sp,-56
	li	$t0,102
	sb 	$t0,0($sp)
	li	$t0,103
	sb 	$t0,1($sp)
	li	$t0,104
	sb 	$t0,2($sp)
	li	$t0,105
	sb 	$t0,3($sp)
	li	$t0,106
	sb 	$t0,4($sp)
	li	$t0,18231045
	sw 	$t0,16($sp)
	li	$t0,10451823
	sw 	$t0,20($sp)
	li	$t0,188111987
	sw 	$t0,24($sp)
	li	$t0,-1
	sw 	$t0,28($sp)
func_mid_main:
	addu	$t1,$0,$sp
	li	$t2,1
	addu	$t0,$t1,$t2
	sw	$t0,32($sp)
	lw	$t1,32($sp)
	lb	$t0,1($t1)
	sw	$t0,40($sp)
	sw	$v0,48($sp)
	lw	$a0,40($sp)
	li	$v0,11
	syscall
	li	$a0,10
	li	$v0,11
	syscall
	lw	$v0,48($sp)
	sw	$v0,48($sp)
	li	$v0,5
	syscall
	sw	$v0,44($sp)
	lw	$v0,48($sp)
	lw	$t0,44($sp)
	addu	$t1,$0,$sp
	sw	$t0,8($t1)
	addu	$t1,$0,$sp
	li	$t2,8
	addu	$t0,$t1,$t2
	sw	$t0,36($sp)
	lw	$t1,36($sp)
	lw	$t0,0($t1)
	sw	$t0,36($sp)
	sw	$v0,48($sp)
	la	$a0,string_0
	li	$v0,4
	syscall
	lw	$a0,36($sp)
	li	$v0,1
	syscall
	li	$a0,10
	li	$v0,11
	syscall
	lw	$v0,48($sp)
func_end_main:
	li	$v0,10
	syscall

