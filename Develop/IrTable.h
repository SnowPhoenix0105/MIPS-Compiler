#pragma once

#ifndef __IR_TABLE_H__
#define __IR_TABLE_H__


enum class IrRegType
{
	var,	// ����
	arr,	// ����
	tmp,	// ��ʱ����
	cst,	// ����
};

enum class IrLabelType
{
	_func, _for, _while, _else
};

enum class IrType
{
	_add,		// �ӷ�
	_sub,		// ����
	_mult,		// �˷�
	_div,		// ����
	_less,		// С��
	_leq,		// С�ڵ���
	_great,		// ����
	_geq,		// ���ڵ���
	_equal,		// ����
	_neq,		// ������
	_beq,		// �����ת
	_bne,		// ������ת
	_goto,		// ��������ת
	_param,		// ѹ��
	_call,		// ��������
	_ret		// ����
};

struct IrLabel
{
	virtual ~IrLabel() = 0;
};

struct IrVar
{

};



class IrTable
{
};



#endif // !__IR_TABLE_H__

