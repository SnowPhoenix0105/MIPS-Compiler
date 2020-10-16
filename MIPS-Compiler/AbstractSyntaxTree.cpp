#include "AbstractSyntaxTree.h"

string ProgrameNode::print() const
{
	string ret;
	if (constant_declaration != nullptr)
	{
		ret += constant_declaration->print();
		ret.push_back('\n');
	}
	if (variable_declaration != nullptr)
	{
		ret += variable_declaration->print();
		ret.push_back('\n');
	}
	for (const auto& function_defination : function_definations)
	{
		ret += function_defination->print();
		ret.push_back('\n');
	}
	ret += main_function->print();
	ret.push_back('\n');
	ret += "<程序>\n";
	return ret;
}

string ConstantDeclarationNode::print() const
{
	string ret;
	for (const auto& constant_defination : constant_definations)
	{
		ret += constant_defination->print();
		ret.push_back('\n');
	}
	ret += "<常量说明>\n";
	return ret;
}
