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
	TEST_CLASS(ComplexErrorTest)
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

		TEST_METHOD(Correct)
		{
			auto rlt = assert_contain_expect(unique_ptr<istream>(new std::ifstream(test_resource_path + "\\correct.c")), vector<string>());
			Assert::IsTrue(rlt.size() == 0);
		}

		TEST_METHOD(Complex1)
		{
			test_file("source1.c", "answer1.txt");
		}

		TEST_METHOD(Complex2)
		{
			test_file("source2.c", "answer2.txt");
		}
	};

	const string ComplexErrorTest::test_resource_path("D:\\Projects\\C++\\MIPS-Compiler\\UnitTest\\TestResource\\complex_error");
}