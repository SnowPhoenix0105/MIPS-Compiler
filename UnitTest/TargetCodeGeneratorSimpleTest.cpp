#include "pch.h"
#include "CppUnitTest.h"
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../Develop/SimpleTargetGenerator.h"
#include "../Develop/TargetCodeGenerator.h"
#include "TestResource/target_gen_test/sample1.h"
#include "BlackBoxTester.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using std::shared_ptr;
using std::string;
using std::make_shared;
using std::shared_ptr;
using std::ostringstream;
using std::ifstream;
using std::endl;
using std::ofstream;



namespace TargetCodeGeneratorTest
{
	TEST_CLASS(TargetCodeGeneratorSimpleTest)
	{
	private:
		static const string test_resource_path;
		shared_ptr<string> to_ptr(const char* s)
		{
			return make_shared<string>(s);
		}

		template<class GeneratorT>
		string ir_to_mips(shared_ptr<IrElemAllocator> allocator, shared_ptr<IrTable> ir_table)
		{
			return ir_to_mips(make_shared<GeneratorT>(allocator, ir_table));
		}

		string ir_to_mips(shared_ptr<ITargetCodeGenerator> generator)
		{
			ostringstream oss;
			generator->translate(oss);
			return oss.str();
		}


	public:
		TEST_CLASS_INITIALIZE(init)
		{
			MarsTester::init();
		}

		TEST_METHOD(SimpleTest1)
		{
			string assembly_file = test_resource_path + "\\target_gen_test\\sample1.asm";
			string input_file_name = test_resource_path + "\\target_gen_test\\sample1.in";
			string output_file_name = test_resource_path + "\\target_gen_test\\sample1.out";
			string expect_file_name = test_resource_path + "\\target_gen_test\\sample1.expect";
			auto pair = get_sample1();
			shared_ptr<IrElemAllocator> allocator_ptr = pair.first;
			shared_ptr<IrTable> ir_table = pair.second;
			string mips = ir_to_mips<SimpleCodeGenerator>(allocator_ptr, ir_table);
			Logger::WriteMessage(mips.c_str());
			ofstream ofs(assembly_file);
			ofs << mips << endl;
			ofs.close();
			MarsTester::assembly_and_simulate(assembly_file, input_file_name, output_file_name);
			MarsTester::compare_and_judge(expect_file_name, output_file_name);
		}
	};

	const string TargetCodeGeneratorSimpleTest::test_resource_path("D:\\Projects\\C++\\MIPS-Compiler\\UnitTest\\TestResource");
}
