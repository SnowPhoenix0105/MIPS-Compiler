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


		TEST_CASE(hw_2_1)
		TEST_CASE(hw_2_2)
		TEST_CASE(hw_2_3)
		TEST_CASE(hw_2_4)
		TEST_CASE(hw_2_5)
		TEST_CASE(hw_2_6)
		TEST_CASE(hw_2_7)
		TEST_CASE(hw_2_8)
		TEST_CASE(hw_2_11)
		TEST_CASE(hw_2_12)
		TEST_CASE(hw_2_13)
		TEST_CASE(hw_2_14)
		TEST_CASE(hw_2_15)
		TEST_CASE(hw_2_16)
		TEST_CASE(hw_2_17)
		TEST_CASE(hw_2_18)
	};

	const string HWBlackboxTest::test_resource_path("D:\\Projects\\C++\\MIPS-Compiler\\UnitTest\\TestResource\\hw_samples");
	const string HWBlackboxTest::test_resource_opposite_path("hw_samples\\");
}
