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

		// ���Ա����Գ������ȷ��
		TEST_METHOD(no_compile_test)
		{
			MarsTester::self_judge();
		}

		
		TEST_CASE(sample2, 4)
	};

	const string BlackboxMultiInputTest::test_resource_path("D:\\Projects\\C++\\MIPS-Compiler\\UnitTest\\TestResource\\auto_samples");
	const string BlackboxMultiInputTest::test_resource_opposite_path("auto_samples\\");
}
