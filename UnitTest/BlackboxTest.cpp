#include "pch.h"
#include "CppUnitTest.h"
#include "..\Develop\compile_controller.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "BlackBoxTester.h"

#define TEST_CASE(series_name) TEST_METHOD(series_name) { test(#series_name); }

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::string;
using std::make_shared;
using std::shared_ptr;
using std::ostringstream;
using std::ifstream;
using std::endl;
using std::unique_ptr;

namespace MarsTester
{
	const string test_resource_path("D:\\Projects\\C++\\MIPS-Compiler\\UnitTest\\TestResource");
	shared_ptr<string> base_result = nullptr;
}

namespace BlackboxTest
{
	TEST_CLASS(BlackboxTest)
	{
	private:
		static const string test_resource_path;
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

		//TEST_CASE(fuck_you)
	};

	const string BlackboxTest::test_resource_path("D:\\Projects\\C++\\MIPS-Compiler\\UnitTest\\TestResource\\blackbox_test");
}
