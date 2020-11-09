#pragma once

#ifndef __IR_TABLE_H__
#define __IR_TABLE_H__

#include "global_control.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

enum class IrType
{
	label,		// ���ñ�ǩ	<label>
	func,		// ���庯������ֵ <type>
	param,		// ���庯�����β� <var>
	arr,		// ��������	<arr> <type> <imm>
	add,		// �ӷ�	<var> <val> <val>
	sub,		// ����	<var> <val> <val>
	mult,		// �˷�	<var> <val> <val>
	div,		// ����	<var> <val> <val>
	sl,			// ��λ�� <var> <val> <val>
	sr,			// ��λ�� <var> <val> <val>
	less,		// С��	<var> <val> <val>
	save,		// ��������Ԫ��	<var> <var> <arr>
	load,		// ��������Ԫ��	<var> <var> <arr>
	beq,		// ���ת��
	bne,		// ����ת��
	_goto,		// ������ת��
	push,		// ����ѹ��
	call,		// ��������
	scanf,		// ������
	printf,		// д����
};



#endif // !__IR_TABLE_H__

