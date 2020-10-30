#include "pch.h"
#include "CppUnitTest.h"
#include "../Develop/global_control.h"
#include "../Develop/SyntacticAnalyzer.h"
#include <fstream>
#include <string>
#include <sstream>
#include <memory>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using std::shared_ptr;
using std::unique_ptr;
using std::string;
using std::vector;
using std::istream;
using std::ostream;

namespace ErrorDealingTest
{
	TEST_CLASS(NoErrorTest)
	{
	public:
		static const string test_resource_path;

		void test_file_no_error(const string& file_name)
		{
			unique_ptr<istream> input_file(new std::ifstream(test_resource_path + '\\' + file_name));
			assert_no_error(std::move(input_file));
		}

		void assert_no_error(std::unique_ptr<std::istream> input_file)
		{
			unique_ptr<ostringstream> os(new std::ostringstream());
			unique_ptr<LexicalAnalyzer> lexical_analyzer(new LexicalAnalyzer(std::move(input_file)));
			SyntacticAnalyzer syntactic_analyzer(std::move(lexical_analyzer));
			syntactic_analyzer.parse();
			syntactic_analyzer.print_all_error(*os);
			string str = os->str();
			Assert::IsTrue(str.size() == 0);
		}

		TEST_METHOD(MySample1)
		{
			test_file_no_error("testfile1.txt");
		}

		TEST_METHOD(MySample2)
		{
			test_file_no_error("testfile2.txt");
		}

		TEST_METHOD(MySample3)
		{
			test_file_no_error("testfile3.txt");
		}

		TEST_METHOD(MySample4)
		{
			test_file_no_error("testfile4.txt");
		}

		TEST_METHOD(MySample5)
		{
			test_file_no_error("testfile5.txt");
		}

		TEST_METHOD(MySample6)
		{
			test_file_no_error("testfile6.txt");
		}
	};

	const string NoErrorTest::test_resource_path("D:\\Projects\\C++\\MIPS-Compiler\\UnitTest\\TestResource\\mysamples");
}
