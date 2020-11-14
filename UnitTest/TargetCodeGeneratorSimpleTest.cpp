#include "pch.h"
#include "CppUnitTest.h"
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../Develop/SimpleTargetGenerator.h"
#include "../Develop/TargetCodeGenerator.h"
#include "TestResource/target_gen_test/sample1.h"


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

		/// <summary>
		/// 依照一个汇编源文件, 进行仿真运行, 将输入与输出按照指定的文件进行重定向
		/// </summary>
		/// <param name="assembly_file">汇编源文件的文件名</param>
		/// <param name="input_file_name">输入内容所在文件的文件名</param>
		/// <param name="output_file_name">输出目标的文件名</param>
		void assembly_and_simulate(
			const string& assembly_file,
			const string& input_file_name,
			const string& output_file_name)
		{
			ostringstream commond;
			commond
				<< "java "
				<< "-jar " << TargetCodeGeneratorSimpleTest::test_resource_path << "\\blackbox_test\\MARS-JDK7-RE.jar"
				<< ' ' //<< " db "
				<< assembly_file;
			execute_cmd(commond.str(), input_file_name, output_file_name);
		}

		void execute_cmd(const string& cmd, const string& input_file_name, const string& output_file_name)
		{
			ostringstream cmd_builder;
			cmd_builder << cmd;
			if (input_file_name.size() != 0)
			{
				cmd_builder << " < " << input_file_name ;
			}
			if (output_file_name.size() != 0)
			{
				cmd_builder << " > " << output_file_name;
			}
			cmd_builder << endl;
			string scmd = cmd_builder.str();
			const char* ccmd = scmd.c_str();
			Logger::WriteMessage(ccmd);
			std::system(ccmd);
		}

		shared_ptr<string> read_file(const string& file_name)
		{
			ifstream file(file_name);
			if (file.good())
			{
				file >> std::noskipws;
				std::istream_iterator<char> it(file);
				std::istream_iterator<char> eof;
				return make_shared<string>(it, eof);
			}
			if (file.eof())
			{
				return make_shared<string>();
			}
			Logger::WriteMessage(("cannot open file:" + file_name).c_str());
			Assert::Fail();
		}
	public:

		TEST_METHOD(SimpleTest1)
		{
			string assembly_file = test_resource_path + "\\target_gen_test\\sample1.asm";
			string input_file_name = test_resource_path + "\\target_gen_test\\sample1.in";
			string output_file_name = test_resource_path + "\\target_gen_test\\sample1.out";
			auto pair = get_sample1();
			shared_ptr<IrElemAllocator> allocator_ptr = pair.first;
			shared_ptr<IrTable> ir_table = pair.second;
			string mips = ir_to_mips<SimpleCodeGenerator>(allocator_ptr, ir_table);
			Logger::WriteMessage(mips.c_str());
			ofstream ofs(assembly_file);
			ofs << mips << endl;
			ofs.close();
			assembly_and_simulate(assembly_file, input_file_name, output_file_name);
		}
	};

	const string TargetCodeGeneratorSimpleTest::test_resource_path("D:\\Projects\\C++\\MIPS-Compiler\\UnitTest\\TestResource");
}
