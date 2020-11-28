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
	public:
		
		TEST_METHOD(TestMethod1)
		{
			unique_ptr<istream> input_file(new ifstream("D:\\Projects\\C++\\MIPS-Compiler\\UnitTest\\TestResource\\detector_test\\sample1\\source.c"));
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
	};
}
