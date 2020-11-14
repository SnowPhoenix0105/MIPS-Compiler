#include "pch.h"
#include "CppUnitTest.h"
#include "..\Develop\compile_controller.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


using std::string;
using std::make_shared;
using std::shared_ptr;
using std::ostringstream;
using std::ifstream;
using std::endl;


namespace MarsTester
{
	using namespace Microsoft::VisualStudio::CppUnitTestFramework;
	extern const string test_resource_path;
	extern shared_ptr<string> base_result;

	inline void execute_cmd(const string& cmd, const string& input_file_name, const string& output_file_name)
	{
		ostringstream cmd_builder;
		cmd_builder << cmd;
		if (input_file_name.size() != 0)
		{
			cmd_builder << " < " << input_file_name;
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

	/// <summary>
	/// 依照一个汇编源文件, 进行仿真运行, 将输入与输出按照指定的文件进行重定向
	/// </summary>
	/// <param name="assembly_file">汇编源文件的文件名</param>
	/// <param name="input_file_name">输入内容所在文件的文件名</param>
	/// <param name="output_file_name">输出目标的文件名</param>
	inline void assembly_and_simulate(
		const string& assembly_file,
		const string& input_file_name,
		const string& output_file_name)
	{
		ostringstream commond;
		commond
			<< "java "
			<< "-jar " << test_resource_path << "\\blackbox_test\\MARS-JDK7-RE.jar"
			<< ' '//<< " db "
			<< assembly_file;
		execute_cmd(commond.str(), input_file_name, output_file_name);
	}

	inline shared_ptr<string> read_file(const string& file_name)
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

	/// <summary>
	/// 
	/// </summary>
	/// <param name="expect_file_name"></param>
	/// <param name="result_file_name"></param>
	inline void compare_and_judge(const string& expect_file_name, const string& result_file_name)
	{
		shared_ptr<string> expect = read_file(expect_file_name);
		shared_ptr<string> result = read_file(result_file_name);
		shared_ptr<string> header = base_result;
		string real_result = result->substr(header->size() - 1, result->size() - header->size());
		Logger::WriteMessage(("expect:"
			+ string("\n>>>>>>>>>>>>>>>>>>>>>>>>>>\n")
			+ *expect
			+ "\n<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n\n").c_str());
		Logger::WriteMessage(("result:"
			+ string("\n>>>>>>>>>>>>>>>>>>>>>>>>>>\n")
			+ real_result
			+ "\n<<<<<<<<<<<<<<<<<<<<<<<<<<\n").c_str());
		Assert::AreEqual(*expect, real_result);
	}

	// 测试一个系列: 需求的外部资源有:源文件,输入,期望输出 
	// @param series_name: 
	inline void test(const string& series_name)
	{
		string source_file_name = test_resource_path + "\\" + series_name + "_source.cl";
		string input_file_name = test_resource_path + "\\" + series_name + "_input.txt";
		string result_file_name = test_resource_path + "\\" + series_name + "_result.txt";
		string expect_file_name = test_resource_path + "\\" + series_name + "_expect.txt";
		string assembly_fule_name = test_resource_path + "\\" + series_name + "_assembly.asm";
		start_compile(source_file_name, assembly_fule_name);
		assembly_and_simulate(
			assembly_fule_name,
			input_file_name,
			result_file_name
		);
		compare_and_judge(expect_file_name, result_file_name);
	}

	inline void init()
	{
		if (base_result == nullptr)
		{
			// 通过运行空asm获得MARS输出的标头
			ostringstream commond;
			commond
				<< "java "
				<< "-jar " << test_resource_path << "\\blackbox_test\\MARS-JDK7-RE.jar"
				<< ' '//<< " db "
				<< test_resource_path << "\\blackbox_test\\empty\\empty.asm";
			execute_cmd(commond.str(), string(), test_resource_path + "\\blackbox_test\\empty\\empty_result.txt");
			base_result = read_file(test_resource_path + "\\blackbox_test\\empty\\empty_result.txt");
		}
	}

	inline void self_judge()
	{
		// 测试本测试程序的正确性
		string asm_file_name = test_resource_path + "\\blackbox_test\\cmd_test\\test.asm";
		string input_file_name = test_resource_path + "\\blackbox_test\\cmd_test\\input.txt";
		string result_file_name = test_resource_path + "\\blackbox_test\\cmd_test\\result.txt";
		string expect_file_name = test_resource_path + "\\blackbox_test\\cmd_test\\expect.txt";
		assembly_and_simulate(
			asm_file_name,
			input_file_name,
			result_file_name
		);
		compare_and_judge(expect_file_name, result_file_name);
	}
};
