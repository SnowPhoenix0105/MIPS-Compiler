#include "pch.h"
#include "CppUnitTest.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>
#include "../Develop/global_control.h"
#include "../Develop/SyntacticAnalyzer.h"

using std::shared_ptr;
using std::unique_ptr;
using std::string;
using std::vector;
using std::istream;
using std::ostream;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ErrorTest
{
	TEST_CLASS(NoErrorTest)
	{
	public:
		static const string test_resource_path;

		void test_file(const string& file_name)
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
			Assert::IsTrue(os->str().size() == 0);
		}

		TEST_METHOD(MySample1)
		{
			test_file("testfile1.txt");
		}

		TEST_METHOD(MySample2)
		{
			test_file("testfile2.txt");
		}

		TEST_METHOD(MySample3)
		{
			test_file("testfile3.txt");
		}

		TEST_METHOD(MySample4)
		{
			test_file("testfile4.txt");
		}

		TEST_METHOD(MySample5)
		{
			test_file("testfile5.txt");
		}

		TEST_METHOD(MySample6)
		{
			test_file("testfile6.txt");
		}
	};

	const string NoErrorTest::test_resource_path("D:\\Projects\\C++\\MIPS-Compiler\\UnitTest\\TestResource\\mysamples");
}
