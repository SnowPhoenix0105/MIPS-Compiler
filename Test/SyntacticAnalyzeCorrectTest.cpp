#include "pch.h"
#include "CppUnitTest.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include "../Develop/global_control.h"
#include "../Develop/compile_controller.h"
#include "../Develop/SyntacticAnalyzer.h"

using std::shared_ptr;
using std::unique_ptr;
using std::string;
using std::vector;
using std::istream;
using std::ostream;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SyntacticAnalyzeTest
{
	TEST_CLASS(SyntacticAnalyzeCorrectTest)
	{
	public:
		void test(const string& test_case, const vector<string>& expect_answer)
		{
			unique_ptr<istream> input_istream(new std::istringstream(test_case));
			unique_ptr<ostringstream> output_ostream(new std::ostringstream());
			output_ostream.reset(
				dynamic_cast<ostringstream*>(
					syntactic_analyze(std::move(input_istream), std::move(output_ostream)).release()
					)
			);
			string answer = output_ostream->str();
			std::istringstream is(answer);
			auto it = expect_answer.cbegin();
			while (std::getline(is, answer))
			{
				Assert::AreEqual(*it++, answer);
			}
			Assert::IsTrue(it == expect_answer.cend());
		}

		TEST_METHOD(Simple_Test_1)
		{
			string test_case =
				"void main() {"
				"printf(\"fuck u\");"
				"}";
			vector<string> expect_answer =
			{
				"VOIDTK void",
				"MAINTK main",
				"LPARENT (",
				"RPARENT )",
				"LBRACE {",
				"PRINTFTK printf",
				"LPARENT (",
				"STRCON fuck u",
				"<字符串>",
				"RPARENT )",
				"<写语句>",
				"SEMICN ;",
				"<语句>",
				"<语句列>",
				"<复合语句>",
				"RBRACE }",
				"<主函数>",
				"<程序>"
			};
			test(test_case, expect_answer);
		}

		TEST_METHOD(HW_2_Test1)
		{
			string test_case =
				"coNst int cONst1 = 001, const2 = -100;"
				"const char const3 = \'_\';"
				"int change1;"
				"char change3;"
				"int gets1(int var1, int var2) {"
				"	change1 = var1 + var2;"
				"	return (change1);"
				"}"
				"void main() {"
				"printf(\"Hello World\");"
				"printf(gets1(10, 20));"
				"}";
			vector<string> expect_answer =
			{
				"CONSTTK coNst",
				"INTTK int",
				"IDENFR cONst1",
				"ASSIGN =",
				"INTCON 001",
			"<无符号整数>",
			"<整数>",
				"COMMA ,",
				"IDENFR const2",
				"ASSIGN =",
				"MINU -",
				"INTCON 100",
			"<无符号整数>",
			"<整数>",
			"<常量定义>",
				"SEMICN ;",
				"CONSTTK const",
				"CHARTK char",
				"IDENFR const3",
				"ASSIGN =",
				"CHARCON _",
			"<常量定义>",
				"SEMICN ;",
			"<常量说明>",
				"INTTK int",
				"IDENFR change1",
			"<变量定义无初始化>",
			"<变量定义>",
				"SEMICN ;",
				"CHARTK char",
				"IDENFR change3",
			"<变量定义无初始化>",
			"<变量定义>",
				"SEMICN ;",
			"<变量说明>",
				"INTTK int",
				"IDENFR gets1",
			"<声明头部>",
				"LPARENT (",
				"INTTK int",
				"IDENFR var1",
				"COMMA ,",
				"INTTK int",
				"IDENFR var2",
			"<参数表>"
				"RPARENT )",
				"LBRACE {",
				"IDENFR change1",
				"ASSIGN =",
				"IDENFR var1",
			"<因子>",
			"<项>",
				"PLUS +",
				"IDENFR var2",
			"<因子>",
			"<项>",
			"<表达式>",
			"<赋值语句>"
				"SEMICN ;",
			"<语句>"
				"RETURNTK return",
				"LPARENT (",
				"IDENFR change1",
			"<因子>",
			"<项>",
			"<表达式>",
				"RPARENT )",
			"<返回语句>",
				"SEMICN ;",
			"<语句>",
			"<语句列>",
			"<复合语句>",
				"RBRACE }",
			"<有返回值函数定义>",
				"VOIDTK void",
				"MAINTK main",
				"LPARENT (",
				"RPARENT )",
				"LBRACE {",
				"PRINTFTK printf",
				"LPARENT (",
				"STRCON Hello World",
			"<字符串>",
				"RPARENT )",
			"<写语句>"
				"SEMICN ;",
			"<语句>"
				"PRINTFTK printf",
				"LPARENT (",
				"IDENFR gets1",
				"LPARENT (",
				"INTCON 10",
			"<无符号整数>",
			"<整数>",
			"<因子>",
			"<项>",
			"<表达式>",
				"COMMA ,",
				"INTCON 20",
			"<无符号整数>",
			"<整数>",
			"<因子>",
			"<项>",
			"<表达式>",
			"<值参数表>"
				"RPARENT )",
			"<有返回值函数调用语句>"
			"<因子>",
			"<项>",
			"<表达式>",
				"RPARENT )",
			"<写语句>",
				"SEMICN ;",
			"<语句>",
			"<语句列>",
			"<复合语句>",
				"RBRACE }",
			"<主函数>",
			"<程序>"
			};
			test(test_case, expect_answer);
		}
	};
}
