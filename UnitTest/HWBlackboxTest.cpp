#include "pch.h"
#include "CppUnitTest.h"
#include "..\Develop\compile_controller.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "BlackBoxTester.h"

#define TEST_CASE(series_name) TEST_METHOD(series_name) { MarsTester::test(test_resource_opposite_path + #series_name, 1); }

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
	TEST_CLASS(HWBlackboxTest)
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


		TEST_CASE(hw_1_1_1)
		TEST_CASE(hw_1_1_2)
		TEST_CASE(hw_1_1_3)
		TEST_CASE(hw_1_1_4)
		TEST_CASE(hw_1_1_5)
		TEST_CASE(hw_1_1_6)
		TEST_CASE(hw_1_1_7)
		TEST_CASE(hw_1_1_8)
		TEST_CASE(hw_1_1_9)
		TEST_CASE(hw_1_1_10)

		TEST_CASE(hw_1_2_1)
		TEST_CASE(hw_1_2_2)
		TEST_CASE(hw_1_2_3)
		TEST_CASE(hw_1_2_4)
		TEST_CASE(hw_1_2_5)
		TEST_CASE(hw_1_2_6)
		TEST_CASE(hw_1_2_7)
		TEST_CASE(hw_1_2_8)
		TEST_CASE(hw_1_2_9)
		TEST_CASE(hw_1_2_10)

		TEST_CASE(hw_1_3_1)
		TEST_CASE(hw_1_3_2)
		TEST_CASE(hw_1_3_3)
		TEST_CASE(hw_1_3_4)
		TEST_CASE(hw_1_3_5)
		TEST_CASE(hw_1_3_6)
		TEST_CASE(hw_1_3_7)
		TEST_CASE(hw_1_3_8)
		TEST_CASE(hw_1_3_9)
		TEST_CASE(hw_1_3_10)

		TEST_CASE(hw_1_4_1)
		TEST_CASE(hw_1_4_2)
		TEST_CASE(hw_1_4_3)
		TEST_CASE(hw_1_4_4)
		TEST_CASE(hw_1_4_5)
		TEST_CASE(hw_1_4_6)
		TEST_CASE(hw_1_4_7)
		TEST_CASE(hw_1_4_8)
		TEST_CASE(hw_1_4_9)
		TEST_CASE(hw_1_4_10)

		TEST_CASE(hw_1_5_1)
		TEST_CASE(hw_1_5_2)
		TEST_CASE(hw_1_5_3)
		TEST_CASE(hw_1_5_4)
		TEST_CASE(hw_1_5_5)
		TEST_CASE(hw_1_5_6)
		TEST_CASE(hw_1_5_7)
		TEST_CASE(hw_1_5_8)
		TEST_CASE(hw_1_5_9)
		TEST_CASE(hw_1_5_10)

		TEST_CASE(hw_1_6_1)
		TEST_CASE(hw_1_6_2)
		TEST_CASE(hw_1_6_3)
		TEST_CASE(hw_1_6_4)
		TEST_CASE(hw_1_6_5)
		TEST_CASE(hw_1_6_6)
		TEST_CASE(hw_1_6_7)
		TEST_CASE(hw_1_6_8)
		TEST_CASE(hw_1_6_9)
		TEST_CASE(hw_1_6_10)



		TEST_CASE(hw_2_1_1)
		TEST_CASE(hw_2_1_2)
		TEST_CASE(hw_2_1_3)
		TEST_CASE(hw_2_1_4)
		TEST_CASE(hw_2_1_5)
		TEST_CASE(hw_2_1_6)
		TEST_CASE(hw_2_1_7)
		TEST_CASE(hw_2_1_8)

		TEST_CASE(hw_2_2_1)
		TEST_CASE(hw_2_2_2)
		TEST_CASE(hw_2_2_3)
		TEST_CASE(hw_2_2_4)
		TEST_CASE(hw_2_2_5)
		TEST_CASE(hw_2_2_6)
		TEST_CASE(hw_2_2_7)
		TEST_CASE(hw_2_2_8)

		TEST_CASE(hw_2_3_1)
		TEST_CASE(hw_2_3_2)
		TEST_CASE(hw_2_3_3)
		TEST_CASE(hw_2_3_4)
		TEST_CASE(hw_2_3_5)
		TEST_CASE(hw_2_3_6)
		TEST_CASE(hw_2_3_7)
		TEST_CASE(hw_2_3_8)
	};

	const string HWBlackboxTest::test_resource_path("D:\\Projects\\C++\\MIPS-Compiler\\UnitTest\\TestResource\\hw_samples");
	const string HWBlackboxTest::test_resource_opposite_path("hw_samples\\");
}
