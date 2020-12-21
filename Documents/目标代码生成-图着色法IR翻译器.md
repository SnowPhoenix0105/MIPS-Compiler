# 图着色法IR翻译器

## 寄存器使用


| 编号  | 别名  | 用途        |
| :---- | :---- | :---------- |
| 0     | zero  | 常量0       |
| 1     | at    | 汇编器保留  |
| 2     | v0    | syscall保留 |
| 3     | v1    | 函数返回值  |
| 4-7   | a0-a3 | 函数参数0-3 |
| 8-15  | t0-t7 | 临时0-7     |
| 16-23 | s0-s7 | 全局0-7     |
| 24-25 | t8-t9 | 临时8-9     |
| 26-27 | k0-k2 | 内核保留0-1 |
| 28    | gp    | 全局指针    |
| 29    | sp    | 栈顶指针    |
| 30    | fp    | 栈帧指针    |
| 31    | ra    | 返回地址    |

## 新一级中间代码

这一级中间代码是体系结构相关的, 总体和前一级中间代码相同, 但是各个指令的操作数只能是寄存器名或常量, 增加了体系结构相关的一些指令.

protect和reload指令用于寄存器和栈的交互, 其中变量名表示一个栈上的空间.

``` c++
enum class IrType
{
	label,		// 	<label>

	gvar,		//  <var> [<imm>]
	arr,		// 	<arr> <type> <imm>
	init,		//  <imm>

	func,		  //  <type>
	param,		//  <var>


	add,		// 	<reg> <reg> <reg>/<cst>
	sub,		// 	<reg> <reg> <reg>/<cst>
	mult,		// 	<reg> <reg> <reg>/<cst>
	div,		// 	<reg> <reg> <reg>/<cst>
	_and,		//  <reg> <reg> <reg>/<cst>
	_or,		//  <reg> <reg> <reg>/<cst>
	_nor,		//  <reg> <reg> <reg>/<cst>
	_xor,		//  <reg> <reg> <reg>/<cst>
	sl,			//  <reg> <reg> <reg>/<cst>
	sr,			//  <reg> <reg> <reg>/<cst>
	less,		// 	<reg> <reg> <reg>/<cst>

	lw,			  // 	<reg> <reg> <arr>
	lb,			  // 	<reg> <reg> <arr>
	sw,			  // 	<reg> <reg> <arr>
	sb,			  //  <reg> <reg> <arr>

	beq,		  //  <reg> <reg> <label>
	bne,		  //  <reg> <reg> <label>
	_goto,		//  <label>

	push,		  //  <reg>
	call,		  //  <label>
	ret,		  //  null

	scanf,		//  <reg> <type>
	printf,		//  [<string>] [<reg> <type>]	

  movz,     //  <reg> <reg> <reg>
  movn,     // <reg> <reg> <reg>
	protect,	// <reg> <var>
	reload,		// <reg> <var>
}	
```


## 运行栈结构

```
    + - - - - - - +
    |   ret addr  |
    + - - - - - - +
    |   param k   |
    |         0   |
    + - - - - - - +
    |  alloc $sx  |
    + - - - - - - +
    |   protect   |
    + - - - - - - +
    |   arrays    |
    + - - - - - - +  <- $sp
```


上一级中简代码通过寄存器分配器转换为新一级中间代码, 进行体系结构相关优化后, 转换为mips代码, 运行栈中的protect段, 是为了中间代码中protect和reload指令所使用的第二个操作数`<var>`保留的.


函数传参, 使用`$a0`-`$a3`, 函数返回值仅使用`$v1`. 而`$v0`作为syscall和sw的保留寄存器.

发生函数压参时, 先统计到`call`之前, 有多少`push`指令, 然后将所有活跃的`$ax`寄存器压入本函数块的栈中, 然后将所有对本函数的参数的引用改为同全局变量相同的方式, 即先加载到临时寄存器后使用. 若`push`多于4个, 则将多余的参数压栈至下一函数块.


发生函数压参时, 先统计到`call`之前, 有多少`push`指令, 并记录`call`指令下标.
倒数4个`push`指令之前只需做简单的`var2reg`.
倒数的4个`push`指令, 先考察当先指令结束后`$ax`是否仍旧活跃, 
* 若不活跃, 则无需保存, 但注意如果在此之后, `call`之前如果`$ax`恢复活跃, 则需要分配至`$tx`.
* 若此`push`后仍旧活跃, 再考察在该`push`至`call`之间考查`$ax`中保存的变量是否使用到, 
  * 若使用到, 则将其移动至`$tx`中(并且直到`call`之前, `$tx`不能被置换到栈中), 发生`call`时直接由`$tx`保存至栈中指定位置;
  * 若未使用到, 且在`call`之后, 仍旧活跃, 则将其保护至栈中. 
`call`结束后, 若`$ax`仍旧活跃, 则从栈中重新加载.
