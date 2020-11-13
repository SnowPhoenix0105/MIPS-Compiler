#include "pch.h"
#include "CppUnitTest.h"
#include <memory>
#include "../Develop/SimpleTargetGenerator.h"
#include "../Develop/TargetCodeGenerator.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using std::shared_ptr;
using std::make_shared;



namespace TargetCodeGeneratorTest
{
	TEST_CLASS(TargetCodeGeneratorSimpleTest)
	{
	private:
		template<class GeneratorT>
		void test(shared_ptr<IrTable> ir_table, shared_ptr<IrElemAllocator> allocator)
		{
			test(make_shared<GeneratorT>(ir_table, allocator));
		}

		void test(shared_ptr<ITargetCodeGenerator> generator)
		{
			ostringstream oss;
			generator->translate(oss);
		}
	public:

		TEST_METHOD(SimpleTest1)
		{
			shared_ptr<IrElemAllocator> allocator_ptr;
			IrElemAllocator& allocator = *allocator_ptr;
			IrTableBuilder builder;
			builder.push_back_all(
				{
					{ IrHead::gvar, { allocator.alloc_named(make_shared<string>("var1")), IrType::NIL, IrType::NIL } },
				}
			);
			allocator.set_function(make_shared<string>("main"));
			builder.push_back_all(
				{
					{ IrHead::arr, { allocator.alloc_arr(make_shared<string>("arr1")), IrType::_int, allocator.alloc_imm(4) } },
					{ IrHead::init, { allocator.alloc_imm(18231045), IrType::NIL, IrType::NIL } },
					{ IrHead::init, { allocator.alloc_imm(13572468), IrType::NIL, IrType::NIL } },
					{ IrHead::init, { allocator.alloc_imm(31415926), IrType::NIL, IrType::NIL } },
					{ IrHead::init, { allocator.alloc_imm(1595763343), IrType::NIL, IrType::NIL } },
					{ IrHead::gvar, { allocator.alloc_named(make_shared<string>("var1")), IrType::NIL, IrType::NIL } },
					{ IrHead::gvar, { allocator.alloc_named(make_shared<string>("var1")), IrType::NIL, IrType::NIL } }
				}
			);
		}
	};
}
