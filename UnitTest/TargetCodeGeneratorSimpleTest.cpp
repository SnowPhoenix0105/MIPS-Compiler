#include "pch.h"
#include "CppUnitTest.h"
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../Develop/SimpleTargetGenerator.h"
#include "../Develop/TargetCodeGenerator.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using std::shared_ptr;
using std::string;
using std::make_shared;
using std::shared_ptr;
using std::ostringstream;
using std::ifstream;
using std::endl;



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
				<< "-jar " << TargetCodeGeneratorSimpleTest::test_resource_path << "\\MARS-JDK7-RE.jar"
				<< " db "
				<< assembly_file;
			execute_cmd(commond.str(), input_file_name, output_file_name);
		}

		void execute_cmd(const string& cmd, const string& input_file_name, const string& output_file_name)
		{
			ostringstream cmd_builder;
			cmd_builder << cmd;
			if (input_file_name.size() != 0)
			{
				cmd_builder << " <" << input_file_name;
			}
			if (output_file_name.size() != 0)
			{
				cmd_builder << " >" << output_file_name;
			}
			cmd_builder << endl;
			std::system(cmd_builder.str().c_str());
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
			shared_ptr<IrElemAllocator> allocator_ptr = make_shared<IrElemAllocator>();
			IrElemAllocator& allocator = *allocator_ptr;
			IrFactory ir(allocator_ptr);
			IrTableBuilder builder;
			irelem_t gvar1 = allocator.alloc_named(to_ptr("gvar1"));
			irelem_t gvar2 = allocator.alloc_named(to_ptr("gvar2"));
			irelem_t garr1 = allocator.alloc_arr(to_ptr("garr1"));
			irelem_t garr2 = allocator.alloc_arr(to_ptr("garr2"));
			builder.push_back_all(
				{
					ir.gvar(gvar1),
					ir.arr(garr1, IrType::_char, 5),
					ir.init('a'),
					ir.init('b'),
					ir.init('c'),
					ir.init('d'),
					ir.init('e'),
					ir.arr(garr2, IrType::_int, 4),
					ir.gvar(gvar2, 18231045),
				}
			);
			allocator.set_function(to_ptr("main"));
			irelem_t var1 = allocator.alloc_named(to_ptr("var1"));
			irelem_t var2 = allocator.alloc_named(to_ptr("var2"));
			irelem_t tmp1 = allocator.alloc_tmp();
			irelem_t tmp2 = allocator.alloc_tmp();
			irelem_t arr1 = allocator.alloc_arr(to_ptr("arr1"));
			irelem_t arr2 = allocator.alloc_arr(to_ptr("arr2"));
			irelem_t arr3 = allocator.alloc_arr(to_ptr("arr3"));
			builder.push_back_all(
				{
					ir.label(allocator.alloc_func("main").beg()),
					ir.arr(arr1, IrType::_char, 5),
					ir.init('a'),
					ir.init('b'),
					ir.init('c'),
					ir.init('d'),
					ir.init('e'),
					ir.arr(arr2, IrType::_int, 2),
					ir.arr(arr3, IrType::_int, 4),
					ir.init(18231045),
					ir.init(10451823),
					ir.init(188111987),
					ir.init(0xFFFFFFFF),
					ir.label(allocator.alloc_func("main").mid()),
					ir.add(tmp1, allocator.sp(), allocator.alloc_imm(1)),
					ir.lb(var1, tmp1, allocator.cst_add(arr1, allocator.alloc_imm(1))),
					ir.printf(var1, IrType::_char),
					ir.scanf(var2, IrType::_int),
					ir.sw(var2, allocator.sp(), allocator.alloc_imm(8)),
					ir.add(tmp2, allocator.sp(), allocator.alloc_imm(8)),
					ir.lw(tmp2, tmp2, allocator.alloc_imm(0)),
					ir.printf(tmp2, IrType::_int),
					ir.label(allocator.alloc_func("main").end()),
				}
			);
			shared_ptr<IrTable> ir_table = builder.build();
			string mips = ir_to_mips<SimpleCodeGenerator>(allocator_ptr, ir_table);
			Logger::WriteMessage(mips.c_str());
		}
	};

	const string TargetCodeGeneratorSimpleTest::test_resource_path("D:\\Projects\\C++\\MIPS-Compiler\\UnitTest\\TestResource\\blackbox_test");
}
