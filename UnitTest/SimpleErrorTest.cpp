#include "pch.h"
#include "CppUnitTest.h"
#include "../Develop/global_control.h"
#include "../Develop/SyntacticAnalyzer.h"
#include <fstream>
#include <string>
#include <sstream>
#include <memory>
#include <unordered_set>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using std::shared_ptr;
using std::unique_ptr;
using std::string;
using std::vector;
using std::istream;
using std::ostream;
using std::ifstream;
using std::istringstream;
using std::unordered_set;

namespace ErrorDealingTest
{
	TEST_CLASS(SimpleErrorTest)
	{
	public:
		static const string test_resource_path;

		unordered_set<string> test_file(const string& source_file_name, const string& answer_file_name)
		{
			ifstream is(test_resource_path + '\\' + answer_file_name);
			string buf;
			vector<string> expect_answer;
			while (std::getline(is, buf))
			{
				expect_answer.push_back(buf);
			}
			unique_ptr<istream> input_file(new std::ifstream(test_resource_path + '\\' + source_file_name));
			return assert_contain_expect(std::move(input_file), expect_answer);
		}

		unordered_set<string> assert_contain_expect(unique_ptr<istream> input_file, const vector<string>& expect_answer)
		{
			unique_ptr<ostringstream> os(new std::ostringstream());
			unique_ptr<LexicalAnalyzer> lexical_analyzer(new LexicalAnalyzer(std::move(input_file)));
			SyntacticAnalyzer syntactic_analyzer(std::move(lexical_analyzer));
			syntactic_analyzer.parse();
			syntactic_analyzer.print_all_error(*os);
			string str = os->str();
			string expect;
			for (const auto& s : expect_answer)
			{
				expect = expect + s + '\n';
			}

			Logger::WriteMessage(("expect:"
				+ string("\n>>>>>>>>>>>>>>>>>>>>>>>>>>\n")
				+ expect
				+ "\n<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n\n").c_str());
			Logger::WriteMessage(("result:"
				+ string("\n>>>>>>>>>>>>>>>>>>>>>>>>>>\n")
				+ str
				+ "\n<<<<<<<<<<<<<<<<<<<<<<<<<<\n").c_str());
			istringstream iss(str);
			string buf;
			unordered_set<string> answer;
			while (std::getline(iss, buf))
			{
				answer.insert(buf);
			}
			for (const auto& ex : expect_answer)
			{
				Assert::IsTrue(answer.count(ex) != 0);
			}
			return answer;
		}

		TEST_METHOD(HW_EXAMPLE)
		{
			string test_case =
				"const int const1 = 1, const2 = -100;\n"
				"const char const3 = '?';\n"
				"int change1;\n"
				"char change3;\n"
				"int gets1(int var1, int var2) {\n"
				"	change1 = var1 + var2          return (change1);\n"
				"}\n"
				"void main() {\n"
				"	change1 = 10;\n"
				"	printf(\"Hello World\");\n"
				"	printf(gets1(10, 20));\n"
				"}";
			vector<string> expect =
			{
				"2 a",
				"6 k"
			};
			
			assert_contain_expect(unique_ptr<istringstream>(new istringstream(test_case)), expect);
		}

		TEST_METHOD(Semicolon1)
		{
			test_file("source1.c", "answer1.txt");
		}

		TEST_METHOD(CharIndex1)
		{
			auto result = test_file("source2.c", "answer2.txt");
			Assert::IsTrue(result.count("46 i") == 0);
		}

		TEST_METHOD(ConstantType1)
		{
			auto result = test_file("source3.c", "answer3.txt");
		}

		TEST_METHOD(Parameter1)
		{
			auto result = test_file("source4.c", "answer4.txt");
			Assert::IsTrue(result.count("32 e") == 0);
		}

		TEST_METHOD(RightSquare1)
		{
			auto result = test_file("source5.c", "answer5.txt");
		}

		TEST_METHOD(LexicalError1)
		{
			auto result = test_file("source6.c", "answer6.txt");
		}

		TEST_METHOD(Return1)
		{
			auto result = test_file("source7.c", "answer7.txt");
		}

		TEST_METHOD(RightParen)
		{
			auto result = test_file("source8.c", "answer8.txt");
		}

		TEST_METHOD(Switch1)
		{
			auto result = test_file("source9.c", "answer9.txt");
		}

		TEST_METHOD(Undifined1)
		{
			auto result = test_file("source10.c", "answer10.txt");
		}

		TEST_METHOD(Empty)
		{
			auto result = test_file("source11.c", "answer11.txt");
			Assert::IsTrue(result.size() == 0);
		}

		TEST_METHOD(ComplexIndex1)
		{
			auto result = test_file("source12.c", "answer12.txt");
		}
	};

	const string SimpleErrorTest::test_resource_path("D:\\Projects\\C++\\MIPS-Compiler\\UnitTest\\TestResource\\error_dealing");
}