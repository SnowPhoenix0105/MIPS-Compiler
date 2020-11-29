#include "pch.h"
#include "CppUnitTest.h"
#include "../Develop/Detectors.h"
#include "../Develop/global_control.h"
#include "../Develop/compile_controller.h"
#include <iostream>
#include <memory>
#include <fstream>

using std::ifstream;
using std::string;
using std::shared_ptr;
using std::unique_ptr;
using std::cout;
using std::cin;
using std::endl;
using std::istream;
using std::ostream;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DetectorTest
{
	TEST_CLASS(DetectorTest)
	{
		static const string test_resource_path;

		void test_block_in_and_total_in_is_same(string source_path)
		{
			unique_ptr<istream> input_file(new ifstream(test_resource_path + source_path));
			unique_ptr<LexicalAnalyzer> lexical_analyzer(new LexicalAnalyzer(std::move(input_file)));
			SyntacticAnalyzer syntactic_analyzer(std::move(lexical_analyzer));
			syntactic_analyzer.parse();
			shared_ptr<IrElemAllocator> allocator_ptr = syntactic_analyzer.get_allocator_ptr();
			shared_ptr<IrTable> ir_table_ptr = syntactic_analyzer.get_ir_table();
			for (size_t idx = 0; idx != ir_table_ptr->size(); ++idx)
			{
				const auto& code = ir_table_ptr->at(idx);
				if (code.head == IrHead::label && IrType::is_func(code.elem[0]) && IrType::is_beg(code.elem[0]))
				{
					auto block_detect_result
						= IrDetectors::func_block_detect(*ir_table_ptr, *allocator_ptr, idx);
					auto block_var_activition_analyze_result
						= IrDetectors::block_var_activition_analyze(*ir_table_ptr, *allocator_ptr, idx, *block_detect_result);
					auto var_activition_analyze_result
						= IrDetectors::var_activition_analyze(*ir_table_ptr, *allocator_ptr, idx, *block_detect_result, *block_var_activition_analyze_result);
					const auto& blocks = block_detect_result->get_blocks();
					for (size_t i = 0; i != blocks.size(); ++i)
					{
						const auto& block = blocks.at(i);
						Assert::IsTrue(block_var_activition_analyze_result->get_infos().in[i] == var_activition_analyze_result->get_in(block.beg));
						Assert::IsTrue(block_var_activition_analyze_result->get_infos().out[i] == var_activition_analyze_result->get_out(block.end - 1));
					}
				}
			}
		}

		void test_block_in_is_sub_of_param(string source_path)
		{
			unique_ptr<istream> input_file(new ifstream(test_resource_path + source_path));
			unique_ptr<LexicalAnalyzer> lexical_analyzer(new LexicalAnalyzer(std::move(input_file)));
			SyntacticAnalyzer syntactic_analyzer(std::move(lexical_analyzer));
			syntactic_analyzer.parse();
			shared_ptr<IrElemAllocator> allocator_ptr = syntactic_analyzer.get_allocator_ptr();
			shared_ptr<IrTable> ir_table_ptr = syntactic_analyzer.get_ir_table();
			for (size_t idx = 0; idx != ir_table_ptr->size(); ++idx)
			{
				const auto& code = ir_table_ptr->at(idx);
				if (code.head == IrHead::label && IrType::is_func(code.elem[0]) && IrType::is_beg(code.elem[0]))
				{
					auto block_detect_result
						= IrDetectors::func_block_detect(*ir_table_ptr, *allocator_ptr, idx);
					auto block_var_activition_analyze_result
						= IrDetectors::block_var_activition_analyze(*ir_table_ptr, *allocator_ptr, idx, *block_detect_result);
					auto var_activition_analyze_result
						= IrDetectors::var_activition_analyze(*ir_table_ptr, *allocator_ptr, idx, *block_detect_result, *block_var_activition_analyze_result);
					const auto& blocks = block_detect_result->get_blocks();
					unordered_set<irelem_t> params;
					for (size_t i = idx; ; ++i)
					{
						const auto& code = ir_table_ptr->at(i);
						if (code.head == IrHead::label && IrType::is_func(code.elem[0]) && IrType::is_mid(code.elem[0]))
						{
							break;
						}
						if (code.head == IrHead::param)
						{
							params.insert(code.elem[0]);
						}
					}
					for (irelem_t var : block_var_activition_analyze_result->get_infos().in.at(0))
					{
						Assert::IsTrue(params.count(var) != 0);
					}
				}
			}
		}

	public:
		
		TEST_METHOD(BlockInAndTotalInIsSame1)
		{
			test_block_in_and_total_in_is_same("\\sample1\\source.c");
		}

		TEST_METHOD(BlockInIsSubOfParam1)
		{
			test_block_in_is_sub_of_param("\\sample1\\source.c");
		}

		TEST_METHOD(BlockInAndTotalInIsSame2)
		{
			test_block_in_and_total_in_is_same("\\sample2\\source.c");
		}

		TEST_METHOD(BlockInIsSubOfParam2)
		{
			test_block_in_is_sub_of_param("\\sample2\\source.c");
		}
	};

	const string DetectorTest::test_resource_path("D:\\Projects\\C++\\MIPS-Compiler\\UnitTest\\TestResource\\detector_test");
}
