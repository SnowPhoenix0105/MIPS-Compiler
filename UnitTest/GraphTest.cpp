#include "pch.h"
#include "CppUnitTest.h"
#include "../Develop/Graph.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GraphTest
{
	TEST_CLASS(GraphTest)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			Graph<bool> g1(false);
			auto p1 = g1.alloc_point();
			auto p2 = g1.alloc_point();
			g1.link(p1, p2, true);
			Assert::IsTrue(g1[p1][p2]);
		}

		TEST_METHOD(TestMethod2)
		{
			Graph<bool> g1(false);
			auto p1 = g1.alloc_point();
			auto p2 = g1.alloc_point();
			g1.d_link(p1, p2, true);
			Assert::IsTrue(g1[p1][p2]);
			Assert::IsTrue(g1[p2][p1]);
		}

		TEST_METHOD(TestMethod3)
		{
			Graph<bool> g1(false);
			auto p1 = g1.alloc_point();
			auto p2 = g1.alloc_point();
			g1[p1][p2] = true;
			g1[p2][p1] = false;
			Assert::IsTrue(g1[p1][p2]);
			Assert::IsFalse(g1[p2][p1]);
		}

		TEST_METHOD(TestMethod4)
		{
			Graph<int> g1(1 << 31);
			auto p1 = g1.alloc_point();
			auto p2 = g1.alloc_point();
			auto p3 = g1.alloc_point();
			auto p4 = g1.alloc_point();
			auto p5 = g1.alloc_point();
			auto p6 = g1.alloc_point();
			auto p7 = g1.alloc_point();
			auto p8 = g1.alloc_point();
			g1[p1][p2] = 1;
			g1[p2][p1] = 2;
			Assert::IsTrue(g1[p1][p2] == 1);
			Assert::IsTrue(2 == g1[p2][p1]);
			g1[p3][p2] = 3;
			g1[p2][p1] = 4;
			Assert::IsTrue(g1[p3][p2] == 3);
			Assert::IsTrue(4 == g1[p2][p1]);
			g1[p3][p7] = 1;
			g1[p2][p6] = 2;
			Assert::IsTrue(g1[p3][p7] == 1);
			Assert::IsTrue(2 == g1[p2][p6]);
		}

		TEST_METHOD(TestMethod5)
		{
			Graph<unsigned> g1(~0);
			auto p1 = g1.alloc_point();
			auto p2 = g1.alloc_point();
			auto p3 = g1.alloc_point();
			auto p4 = g1.alloc_point();
			auto p5 = g1.alloc_point();
			auto p6 = g1.alloc_point();
			auto p7 = g1.alloc_point();
			auto p8 = g1.alloc_point();
			g1.link(p1, p2, 1).link(p2, p3, 2).link(p5, p7, 3);
			Graph<unsigned> g2(std::move(g1));
			Assert::AreEqual(1u, g2[p1][p2]);
			Assert::AreEqual(2u, g2[p2][p3]);
			Assert::AreEqual(3u, g2[p5][p7]);
			Assert::AreEqual(~0u, g2[p6][p7]);
		}

		TEST_METHOD(TestMethod6)
		{
			Graph<unsigned> g1(~0u);
			auto p1 = g1.alloc_point();
			auto p2 = g1.alloc_point();
			auto p3 = g1.alloc_point();
			auto p4 = g1.alloc_point();
			auto p5 = g1.alloc_point();
			auto p6 = g1.alloc_point();
			auto p7 = g1.alloc_point();
			auto p8 = g1.alloc_point();
			g1.link(p1, p2, 1).link(p2, p3, 2).link(p5, p7, 3);
			Graph<unsigned> g2(1);
			g2.alloc_point();
			g2.alloc_point();
			g2.link(0, 1, 2);
			g2 = std::move(g1);
			Assert::AreEqual(1u, g2[p1][p2]);
			Assert::AreEqual(2u, g2[p2][p3]);
			Assert::AreEqual(3u, g2[p5][p7]);
			Assert::AreEqual(~0u, g2[p6][p7]);
		}
	};
}
