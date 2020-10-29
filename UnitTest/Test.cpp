#include "pch.h"
#include "CppUnitTest.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test
{
	TEST_CLASS(Test)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			std::cout << sizeof(std::string) << std::endl;
			std::cout << sizeof(std::shared_ptr<std::string>) << std::endl;

		}
	};
}
