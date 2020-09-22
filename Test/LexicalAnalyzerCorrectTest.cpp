#include "pch.h"
#include "CppUnitTest.h"
#include "../Develop/LexicalAnalyzer.h"
#include "../Develop/compile_controller.h"
#include <string>
#include <sstream>
#include <vector>
#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using std::string;
using std::vector;
using std::wstring;
using std::ostream;


namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework {
			template<> inline std::wstring ToString<SymbolType>(const SymbolType& t)
			{
				return symboltype_translate_dictionary.at(t);
			}
		}
	}
}

namespace LexicalAnalyzerTest
{
#include "TestResource/sample1/lexical_expect.h"

	TEST_CLASS(CorrectInputTest)
	{
	private:
		static const string test_resource_path;

	public:
		friend void file_test(const string& source_file_name, const vector<SymbolType>& expect_answer);
		friend void test(
			LexicalAnalyzer& analyzer,
			const vector<SymbolType>& expect_answer,
			std::function<void(LexicalAnalyzer&, SymbolType)> check = [](LexicalAnalyzer& a, SymbolType s) {});

		TEST_METHOD(SimpleStringTest1)
		{
			string test_case = "const int while1=5; void main(){if(while1==5)printf(\"success!\");else printf(\"fail!\");}\n\n";
			vector<SymbolType> expect_answer =
			{
				SymbolType::key_const,
				SymbolType::key_int,
				SymbolType::identifier,
				SymbolType::assign,
				SymbolType::number,
				SymbolType::semicolon,
				SymbolType::key_void,
				SymbolType::key_main,
				SymbolType::left_paren,
				SymbolType::right_paren,
				SymbolType::left_brance,
				SymbolType::key_if,
				SymbolType::left_paren,
				SymbolType::identifier,
				SymbolType::equal,
				SymbolType::number,
				SymbolType::right_paren,
				SymbolType::key_printf,
				SymbolType::left_paren,
				SymbolType::string,
				SymbolType::right_paren,
				SymbolType::semicolon,
				SymbolType::key_else,
				SymbolType::key_printf,
				SymbolType::left_paren,
				SymbolType::string,
				SymbolType::right_paren,
				SymbolType::semicolon,
				SymbolType::right_brance
			};
			unique_ptr<istream> input_istream(new std::istringstream(test_case));
			LexicalAnalyzer analyzer(std::move(input_istream));
			test(analyzer, expect_answer, [](LexicalAnalyzer& a, SymbolType s)
				{
					if (s == SymbolType::identifier) Assert::AreEqual(string("while1"), a.get_content());
					if (s == SymbolType::number) Assert::AreEqual(5, std::atoi(a.get_content().c_str()));
				});
		}

		TEST_METHOD(SimpleStringTest2)
		{
			string test_case = "const char while1='-'; void main(){if(while1=='*')printf(\"success!\");else printf(\"fail!\");}\n\n";
			vector<SymbolType> expect_answer =
			{
				SymbolType::key_const,
				SymbolType::key_char,
				SymbolType::identifier,
				SymbolType::assign,
				SymbolType::character,
				SymbolType::semicolon,
				SymbolType::key_void,
				SymbolType::key_main,
				SymbolType::left_paren,
				SymbolType::right_paren,
				SymbolType::left_brance,
				SymbolType::key_if,
				SymbolType::left_paren,
				SymbolType::identifier,
				SymbolType::equal,
				SymbolType::character,
				SymbolType::right_paren,
				SymbolType::key_printf,
				SymbolType::left_paren,
				SymbolType::string,
				SymbolType::right_paren,
				SymbolType::semicolon,
				SymbolType::key_else,
				SymbolType::key_printf,
				SymbolType::left_paren,
				SymbolType::string,
				SymbolType::right_paren,
				SymbolType::semicolon,
				SymbolType::right_brance
			};
			unique_ptr<istream> input_istream(new std::istringstream(test_case));
			LexicalAnalyzer analyzer(std::move(input_istream));
			test(analyzer, expect_answer, [](LexicalAnalyzer& a, SymbolType s)
				{
					if (s == SymbolType::identifier) Assert::AreEqual(string("while1"), a.get_content());
				});
		}

		TEST_METHOD(HW_1_Test1)
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
				"COMMA ,",
				"IDENFR const2",
				"ASSIGN =",
				"MINU -",
				"INTCON 100",
				"SEMICN ;",
				"CONSTTK const",
				"CHARTK char",
				"IDENFR const3",
				"ASSIGN =",
				"CHARCON _",
				"SEMICN ;",
				"INTTK int",
				"IDENFR change1",
				"SEMICN ;",
				"CHARTK char",
				"IDENFR change3",
				"SEMICN ;",
				"INTTK int",
				"IDENFR gets1",
				"LPARENT (",
				"INTTK int",
				"IDENFR var1",
				"COMMA ,",
				"INTTK int",
				"IDENFR var2",
				"RPARENT )",
				"LBRACE {",
				"IDENFR change1",
				"ASSIGN =",
				"IDENFR var1",
				"PLUS +",
				"IDENFR var2",
				"SEMICN ;",
				"RETURNTK return",
				"LPARENT (",
				"IDENFR change1",
				"RPARENT )",
				"SEMICN ;",
				"RBRACE }",
				"VOIDTK void",
				"MAINTK main",
				"LPARENT (",
				"RPARENT )",
				"LBRACE {",
				"PRINTFTK printf",
				"LPARENT (",
				"STRCON Hello World",
				"RPARENT )",
				"SEMICN ;",
				"PRINTFTK printf",
				"LPARENT (",
				"IDENFR gets1",
				"LPARENT (",
				"INTCON 10",
				"COMMA ,",
				"INTCON 20",
				"RPARENT )",
				"RPARENT )",
				"SEMICN ;",
				"RBRACE }"
			};
			unique_ptr<istream> input_istream(new std::istringstream(test_case));
			unique_ptr<ostringstream> output_ostream(new std::ostringstream());
			output_ostream.reset(
				dynamic_cast<ostringstream*>(
					lexical_analyze(std::move(input_istream), std::move(output_ostream)).release()
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

		TEST_METHOD(UpperLowerTest1)
		{
			string test_case = "coNst int While1=5; void main(){if(While1==5)printf(\"success!\");else printf(\"fail!\");}\n\n";
			vector<SymbolType> expect_answer =
			{
				SymbolType::key_const,
				SymbolType::key_int,
				SymbolType::identifier,
				SymbolType::assign,
				SymbolType::number,
				SymbolType::semicolon,
				SymbolType::key_void,
				SymbolType::key_main,
				SymbolType::left_paren,
				SymbolType::right_paren,
				SymbolType::left_brance,
				SymbolType::key_if,
				SymbolType::left_paren,
				SymbolType::identifier,
				SymbolType::equal,
				SymbolType::number,
				SymbolType::right_paren,
				SymbolType::key_printf,
				SymbolType::left_paren,
				SymbolType::string,
				SymbolType::right_paren,
				SymbolType::semicolon,
				SymbolType::key_else,
				SymbolType::key_printf,
				SymbolType::left_paren,
				SymbolType::string,
				SymbolType::right_paren,
				SymbolType::semicolon,
				SymbolType::right_brance
			};
			unique_ptr<istream> input_istream(new std::istringstream(test_case));
			LexicalAnalyzer analyzer(std::move(input_istream));
			test(analyzer, expect_answer, [](LexicalAnalyzer& a, SymbolType s)
				{
					if (s == SymbolType::key_const)
					{
						Assert::AreEqual(string("coNst"), a.get_content());
						Assert::AreEqual(string("const"), a.get_lower_ident());
					}
					if (s == SymbolType::identifier)
					{
						Assert::AreEqual(string("While1"), a.get_content());
						Assert::AreEqual(string("while1"), a.get_lower_ident());
					}
					if (s == SymbolType::number)
					{
						Assert::AreEqual(5, std::atoi(a.get_content().c_str()));
					}
						
				});

		}

		TEST_METHOD(ComplexFileTest1)
		{
			file_test(test_resource_path + "\\sample1\\source.c", sample1_expect_answer);
		}

		TEST_METHOD(ComplexNospaceFileTest1)
		{
			file_test(test_resource_path + "\\sample1\\nospace_source.c", sample1_expect_answer);
		}
	};

	const string CorrectInputTest::test_resource_path("D:\\Projects\\C++\\MIPS-Compiler\\Test\\TestResource");

	void file_test(const string& source_file_name, const vector<SymbolType>& expect_answer)
	{
		LexicalAnalyzer analyzer(source_file_name);
		test(analyzer, expect_answer);
	}

	void test(
		LexicalAnalyzer& analyzer,
		const vector<SymbolType>& expect_answer,
		std::function<void(LexicalAnalyzer&, SymbolType)> check)
	{
		auto beg = expect_answer.cbegin();
		auto end = expect_answer.cend();
		int count = 0;

		while (beg != end && analyzer.has_next())
		{
			SymbolType expect = *beg++;
			SymbolType answer = analyzer.next();
			Assert::AreEqual(expect, answer, std::to_wstring(count++).c_str());
			check(analyzer, answer);
		}

		if (beg != end)
		{
			Assert::Fail(L"analyzer's answer is too less!");
		}

		if (analyzer.has_next())
		{
			Assert::Fail(L"analyzer's answer is too long");
		}
	}
}
