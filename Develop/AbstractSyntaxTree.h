#pragma once


#ifndef __ABSTRACT_SYNTAX_TREE_H__
#define __ABSTRACT_SYNTAX_TREE_H__


#include <string>
#include <vector>
#include "global_control.h"
#include "SymbolInfo.h"

using std::string;
using std::vector;
using std::shared_ptr;

struct IAbstractSyntaxTreeNode
{
public:
	virtual ~IAbstractSyntaxTreeNode() = 0;
	virtual string print() const = 0;
};

// ����
struct ProgrameNode : IAbstractSyntaxTreeNode
{
	shared_ptr<const ConstantDeclarationNode> constant_declaration;	//����˵�� may null
	shared_ptr<const VariableDeclarationNode> variable_declaration;	//����˵�� may null
	vector<shared_ptr<const IFunctionDefinationNode>> function_definations;	//�������� may empty
	shared_ptr<const MainFunctionNode> main_function;		//������ not null

	ProgrameNode() = default;
	virtual ~ProgrameNode() = default;
	virtual string print() const;
};

//����˵��
struct ConstantDeclarationNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	vector<shared_ptr<const ConstantDefinationNode>> constant_definations;	//�������� size >= 1

	virtual ~ConstantDeclarationNode() = default;
	virtual string print() const;
};

//��������
struct ConstantDefinationNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~ConstantDefinationNode() = default;
	virtual string print() const;
};

// ����
struct IntegerNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~IntegerNode() = default;
	virtual string print() const;
};

// ����ͷ��
struct DeclarationHeadNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~DeclarationHeadNode() = default;
	virtual string print() const;
};

// ����
struct ConstantNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~ConstantNode() = default;
	virtual string print() const;
};

// ����˵��
struct VariableDeclarationNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~VariableDeclarationNode() = default;
	virtual string print() const;
};

// ��������
struct VariableDefinationNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~VariableDefinationNode() = default;
	virtual string print() const;
};

// ���������޳�ʼ��
struct VariableDefinationWithoutInitializationNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~VariableDefinationWithoutInitializationNode() = default;
	virtual string print() const;
};

// �������弰��ʼ��
struct VariableDefinationWithInitializationNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~VariableDefinationWithInitializationNode() = default;
	virtual string print() const;
};

// ��������
struct IFunctionDefinationNode : IAbstractSyntaxTreeNode
{
public:
	virtual ~IFunctionDefinationNode() = 0;
	virtual string print() const = 0;
};

// �з���ֵ��������
struct FunctionDefinationWithReturnNode : IFunctionDefinationNode
{
private:
	//TODO
public:
	virtual ~FunctionDefinationWithReturnNode() = default;
	virtual string print() const;
};

// �޷���ֵ�ĺ�������
struct FunctionDefinationWithoutReturnNode : IFunctionDefinationNode
{
private:
	//TODO
public:
	virtual ~FunctionDefinationWithoutReturnNode() = default;
	virtual string print() const;
};

// �������
struct ComplexStatementNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~ComplexStatementNode() = default;
	virtual string print() const;
};

// ������
struct ParameterListNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~ParameterListNode() = default;
	virtual string print() const;
};

// ������
struct MainFunctionNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~MainFunctionNode() = default;
	virtual string print() const;
};

// ���ʽ
struct ExpressionNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~ExpressionNode() = default;
	virtual string print() const;
};

// ��
struct TermNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~TermNode() = default;
	virtual string print() const;
};

// ����
struct FactorNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~FactorNode() = default;
	virtual string print() const;
};

// ���
struct StatementNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~StatementNode() = default;
	virtual string print() const;
};

// ��ֵ���
struct AssignmentStatementNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~AssignmentStatementNode() = default;
	virtual string print() const;
};

// �������
struct ConditionStatementNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~ConditionStatementNode() = default;
	virtual string print() const;
};

// ����
struct ConditionNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~ConditionNode() = default;
	virtual string print() const;
};

// ѭ�����
struct LoopStatementNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~LoopStatementNode() = default;
	virtual string print() const;
};

// ����
struct StepLengthNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	shared_ptr<SymbolUnsignedInfo> unsigned_integer;
	virtual ~StepLengthNode() = default;
	virtual string print() const;
};

// ������
struct SwitchStatementNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~SwitchStatementNode() = default;
	virtual string print() const;
};

// �����
struct SwitchTableNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~SwitchTableNode() = default;
	virtual string print() const;
};

// ��������
struct CaseStatementNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~CaseStatementNode() = default;
	virtual string print() const;
};

// ȱʡ
struct DefaultNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~DefaultNode() = default;
	virtual string print() const;
};

// �з���ֵ��������
struct CallingFunctionWithReturnNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~CallingFunctionWithReturnNode() = default;
	virtual string print() const;
};

// �޷���ֵ��������
struct CallingFunctionWithoutReturnNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~CallingFunctionWithoutReturnNode() = default;
	virtual string print() const;
};

// ֵ������
struct ParameterValueNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~ParameterValueNode() = default;
	virtual string print() const;
};

// �����
struct StatementListNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~StatementListNode() = default;
	virtual string print() const;
};

// �����
struct ReadStatementNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~ReadStatementNode() = default;
	virtual string print() const;
};

// д���
struct WriteStatementNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~WriteStatementNode() = default;
	virtual string print() const;
};

// �������
struct ReturnNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~ReturnNode() = default;
	virtual string print() const;
};

#endif // !__ABSTRACT_SYNTAX_TREE_H__
