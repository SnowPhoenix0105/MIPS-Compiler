#include "pch.h"
#include "CppUnitTest.h"
#include "..\Develop\compile_controller.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "BlackBoxTester.h"

#define TEST_CASE(series_name, num) TEST_METHOD(series_name) { MarsTester::test(test_resource_opposite_path + #series_name, num); }

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::string;
using std::make_shared;
using std::shared_ptr;
using std::ostringstream;
using std::ifstream;
using std::endl;
using std::unique_ptr;


namespace BlackboxTest
{
	TEST_CLASS(BlackboxMultiInputTest)
	{
	private:
		static const string test_resource_path;
		static const string test_resource_opposite_path;
		// static const string test_asm_path;

	public:
		TEST_CLASS_INITIALIZE(init)
		{
			MarsTester::init();
		}

		// 测试本测试程序的正确性
		TEST_METHOD(no_compile_test)
		{
			MarsTester::self_judge();
		}


		TEST_CASE(sample1, 1)
		TEST_CASE(sample2, 4)
		TEST_CASE(sample3, 1)
		// TEST_CASE(sample4, 1)
		TEST_CASE(sample5, 1)
		TEST_CASE(sample6, 1)
		TEST_CASE(sample7, 4)
		TEST_CASE(sample8, 1)
		TEST_CASE(sample9, 1)
		TEST_CASE(sample10, 4)
		TEST_CASE(div, 10)
		TEST_CASE(o1_3, 1)
		TEST_CASE(mov, 10)
	};

	const string BlackboxMultiInputTest::test_resource_path("D:\\Projects\\C++\\MIPS-Compiler\\UnitTest\\TestResource\\auto_samples");
	const string BlackboxMultiInputTest::test_resource_opposite_path("auto_samples\\");
}
