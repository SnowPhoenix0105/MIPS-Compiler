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

// 程序
struct ProgrameNode : IAbstractSyntaxTreeNode
{
	shared_ptr<const ConstantDeclarationNode> constant_declaration;	//常量说明 may null
	shared_ptr<const VariableDeclarationNode> variable_declaration;	//变量说明 may null
	vector<shared_ptr<const IFunctionDefinationNode>> function_definations;	//函数定义 may empty
	shared_ptr<const MainFunctionNode> main_function;		//主函数 not null

	ProgrameNode() = default;
	virtual ~ProgrameNode() = default;
	virtual string print() const;
};

//常量说明
struct ConstantDeclarationNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	vector<shared_ptr<const ConstantDefinationNode>> constant_definations;	//常量声明 size >= 1

	virtual ~ConstantDeclarationNode() = default;
	virtual string print() const;
};

//常量定义
struct ConstantDefinationNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~ConstantDefinationNode() = default;
	virtual string print() const;
};

// 整数
struct IntegerNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~IntegerNode() = default;
	virtual string print() const;
};

// 声明头部
struct DeclarationHeadNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~DeclarationHeadNode() = default;
	virtual string print() const;
};

// 常量
struct ConstantNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~ConstantNode() = default;
	virtual string print() const;
};

// 变量说明
struct VariableDeclarationNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~VariableDeclarationNode() = default;
	virtual string print() const;
};

// 变量定义
struct VariableDefinationNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~VariableDefinationNode() = default;
	virtual string print() const;
};

// 变量定义无初始化
struct VariableDefinationWithoutInitializationNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~VariableDefinationWithoutInitializationNode() = default;
	virtual string print() const;
};

// 变量定义及初始化
struct VariableDefinationWithInitializationNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~VariableDefinationWithInitializationNode() = default;
	virtual string print() const;
};

// 函数定义
struct IFunctionDefinationNode : IAbstractSyntaxTreeNode
{
public:
	virtual ~IFunctionDefinationNode() = 0;
	virtual string print() const = 0;
};

// 有返回值函数定义
struct FunctionDefinationWithReturnNode : IFunctionDefinationNode
{
private:
	//TODO
public:
	virtual ~FunctionDefinationWithReturnNode() = default;
	virtual string print() const;
};

// 无返回值的函数定义
struct FunctionDefinationWithoutReturnNode : IFunctionDefinationNode
{
private:
	//TODO
public:
	virtual ~FunctionDefinationWithoutReturnNode() = default;
	virtual string print() const;
};

// 复合语句
struct ComplexStatementNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~ComplexStatementNode() = default;
	virtual string print() const;
};

// 参数表
struct ParameterListNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~ParameterListNode() = default;
	virtual string print() const;
};

// 主函数
struct MainFunctionNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~MainFunctionNode() = default;
	virtual string print() const;
};

// 表达式
struct ExpressionNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~ExpressionNode() = default;
	virtual string print() const;
};

// 项
struct TermNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~TermNode() = default;
	virtual string print() const;
};

// 因子
struct FactorNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~FactorNode() = default;
	virtual string print() const;
};

// 语句
struct StatementNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~StatementNode() = default;
	virtual string print() const;
};

// 赋值语句
struct AssignmentStatementNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~AssignmentStatementNode() = default;
	virtual string print() const;
};

// 条件语句
struct ConditionStatementNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~ConditionStatementNode() = default;
	virtual string print() const;
};

// 条件
struct ConditionNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~ConditionNode() = default;
	virtual string print() const;
};

// 循环语句
struct LoopStatementNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~LoopStatementNode() = default;
	virtual string print() const;
};

// 步长
struct StepLengthNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	shared_ptr<SymbolUnsignedInfo> unsigned_integer;
	virtual ~StepLengthNode() = default;
	virtual string print() const;
};

// 情况语句
struct SwitchStatementNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~SwitchStatementNode() = default;
	virtual string print() const;
};

// 情况表
struct SwitchTableNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~SwitchTableNode() = default;
	virtual string print() const;
};

// 情况子语句
struct CaseStatementNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~CaseStatementNode() = default;
	virtual string print() const;
};

// 缺省
struct DefaultNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~DefaultNode() = default;
	virtual string print() const;
};

// 有返回值函数调用
struct CallingFunctionWithReturnNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~CallingFunctionWithReturnNode() = default;
	virtual string print() const;
};

// 无返回值函数调用
struct CallingFunctionWithoutReturnNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~CallingFunctionWithoutReturnNode() = default;
	virtual string print() const;
};

// 值参数表
struct ParameterValueNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~ParameterValueNode() = default;
	virtual string print() const;
};

// 语句列
struct StatementListNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~StatementListNode() = default;
	virtual string print() const;
};

// 读语句
struct ReadStatementNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~ReadStatementNode() = default;
	virtual string print() const;
};

// 写语句
struct WriteStatementNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~WriteStatementNode() = default;
	virtual string print() const;
};

// 返回语句
struct ReturnNode : IAbstractSyntaxTreeNode
{
private:
	//TODO
public:
	virtual ~ReturnNode() = default;
	virtual string print() const;
};

#endif // !__ABSTRACT_SYNTAX_TREE_H__
