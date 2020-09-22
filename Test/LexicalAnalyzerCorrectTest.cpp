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
			string expect_answer =
				"CONSTTK coNst\n"
				"INTTK int\n"
				"IDENFR cONst1\n"
				"ASSIGN =\n"
				"INTCON 001\n"
				"COMMA ,\n"
				"IDENFR const2\n"
				"ASSIGN =\n"
				"MINU -\n"
				"INTCON 100\n"
				"SEMICN ;\n"
				"CONSTTK const\n"
				"CHARTK char\n"
				"IDENFR const3\n"
				"ASSIGN =\n"
				"CHARCON _\n"
				"SEMICN;\n"
				"INTTK int\n"
				"IDENFR change1\n"
				"SEMICN;\n"
				"CHARTK char\n"
				"IDENFR change3\n"
				"SEMICN;\n"
				"INTTK int\n"
				"IDENFR gets1\n"
				"LPARENT (\n"
				"INTTK int\n"
				"IDENFR var1\n"
				"COMMA ,\n"
				"INTTK int\n"
				"IDENFR var2\n"
				"RPARENT )\n"
				"LBRACE {\n"
				"IDENFR change1\n"
				"ASSIGN =\n"
				"IDENFR var1\n"
				"PLUS +\n"
				"IDENFR var2\n"
				"SEMICN ;\n"
				"RETURNTK return\n"
				"LPARENT (\n"
				"IDENFR change1\n"
				"RPARENT )\n"
				"SEMICN ;\n"
				"RBRACE }\n"
				"VOIDTK void\n"
				"MAINTK main\n"
				"LPARENT (\n"
				"RPARENT )\n"
				"LBRACE {\n"
				"PRINTFTK printf\n"
				"LPARENT (\n"
				"STRCON Hello World\n"
				"RPARENT )\n"
				"SEMICN ;\n"
				"PRINTFTK printf\n"
				"LPARENT (\n"
				"IDENFR gets1\n"
				"LPARENT (\n"
				"INTCON 10\n"
				"COMMA ,\n"
				"INTCON 20\n"
				"RPARENT )\n"
				"RPARENT )\n"
				"SEMICN ;\n"
				"RBRACE }";
			unique_ptr<istream> input_istream(new std::istringstream(test_case));
			unique_ptr<ostringstream> output_ostream(new std::ostringstream());
			unique_ptr<ostream> result(lexical_analyze(std::move(input_istream), std::move(output_ostream)));
			string answer = static_cast<unique_ptr<ostringstream>>(result)-> str();
			Assert::AreEqual(expect_answer, answer);
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

	const string CorrectInputTest::test_resource_path("D:\\Projects\\C++\\Complier\\UnitTest\\TestResource");

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


//
//#define key_case        SymbolType::key_case		// ¹Ø¼ü×Ö case
//#define key_char        SymbolType::key_char		// ¹Ø¼ü×Ö char
//#define key_const       SymbolType::key_const		// ¹Ø¼ü×Ö const
//#define key_default     SymbolType::key_default		// ¹Ø¼ü×Ö default
//#define key_else        SymbolType::key_else		// ¹Ø¼ü×Ö else
//#define key_for         SymbolType::key_for			// ¹Ø¼ü×Ö for
//#define key_if          SymbolType::key_if			// ¹Ø¼ü×Ö if
//#define key_int         SymbolType::key_int			// ¹Ø¼ü×Ö int
//#define key_main        SymbolType::key_main		// ¹Ø¼ü×Ö main
//#define key_printf      SymbolType::key_printf		// ¹Ø¼ü×Ö printf
//#define key_return      SymbolType::key_return		// ¹Ø¼ü×Ö return
//#define key_scanf       SymbolType::key_scanf		// ¹Ø¼ü×Ö scanf
//#define key_switch      SymbolType::key_switch		// ¹Ø¼ü×Ö switch
//#define key_void        SymbolType::key_void		// ¹Ø¼ü×Ö void
//#define key_while       SymbolType::key_while		// ¹Ø¼ü×Ö while
//
//
//#define colon           SymbolType::colon			// Ã°ºÅ :
//#define comma           SymbolType::comma			// ¶ººÅ 
//#define semicolon       SymbolType::semicolon		// ·ÖºÅ 
//#define left_paren      SymbolType::left_paren		// ×óÔ²À¨ºÅ (
//#define right_paren     SymbolType::right_paren		// ÓÒÔ²À¨ºÅ )
//#define left_brance     SymbolType::left_brance		// ×ó»¨À¨ºÅ {
//#define right_brance    SymbolType::right_brance	// ÓÒ»¨À¨ºÅ }
//#define left_square     SymbolType::left_square		// ×ó·½À¨ºÅ [
//#define right_square    SymbolType::right_square	// ÓÒ·½À¨ºÅ ]
//#define plus            SymbolType::plus			// ¼ÓºÅ		+
//#define minus           SymbolType::minus			// ¼õºÅ		-
//#define mult            SymbolType::mult			// ³ËºÅ		*
//#define div             SymbolType::div				// ³ýºÅ		/
//
//
//#define assign          SymbolType::assign			// ¸³ÖµºÅ	=
//#define less            SymbolType::less			// Ð¡ÓÚ		<
//#define greater         SymbolType::greater			// ´óÓÚ		>
//#define less_equal      SymbolType::less_equal		// Ð¡ÓÚµÈÓÚ	<=
//#define greater_equal   SymbolType::greater_equal	// ´óÓÚµÈÓÚ	>=
//#define equal           SymbolType::equal			// µÈÓÚºÅ	==
//#define not_equal       SymbolType::not_equal		// ²»µÈºÅ	!=
//
//#define character       SymbolType::character		// ×Ö·û
//#define string          SymbolType::string			// ×Ö·û´®
//#define number          SymbolType::number			// Êý×Ö
//#define identifier           SymbolType::identifier			// ÆäËüºÏ·¨±êÊ¶·û
//
//
//#define wrong           SymbolType::wrong			// ´íÎó