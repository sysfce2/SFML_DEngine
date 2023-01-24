#include "pch.h"
#include "CppUnitTest.h"
#include "Spatial/Grid.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(GridTests)
	{
	public:
		
		TEST_METHOD(Grid_WhenGridIsCreated_HasDimensions)
		{
			drft::spatial::Grid<int> grid(64, 32);

			Assert::AreEqual(64, grid.width());
			Assert::AreEqual(32, grid.height());
		}

		TEST_METHOD(Grid_WhenGridIsCreated_CanAccessUnusedValue)
		{
			drft::spatial::Grid<int> grid(64, 32);

			auto i = grid.at(10, 10);
			Assert::AreEqual(0, i);
		}

		TEST_METHOD(Grid_WhenAccessingOutOfRangeValue_ThrowsException)
		{
			drft::spatial::Grid<int> grid(32, 32);
			

			Assert::ExpectException<std::out_of_range>([&grid]()
				{
					auto i = grid.at(33, 33);
				});
		}

		TEST_METHOD(Grid_WhenCellSet_CellHasValue)
		{
			drft::spatial::Grid<int> grid(64, 32);

			grid.at(10, 12) = 10;
			grid.at(20, 22) = 50;

			Assert::AreEqual(10, grid.at(10, 12));
			Assert::AreEqual(50, grid.at(20, 22));
		}

		TEST_METHOD(Grid_WhenGridCleared_AllValuesDefaultInitialized)
		{
			const int TEST_WIDTH = 32;
			const int TEST_HEIGHT = 32;
			drft::spatial::Grid<int> grid(32
				, 32);

			for (int y = 0; y < 32; ++y)
			{
				for (int x = 0; x < 32; ++x)
				{
					grid.at(x, y) = x + y + 1;
				}
			}

			Assert::AreEqual(5 + 10 + 1, grid.at(5, 10));

			grid.clear();

			for (int y = 0; y < 32; ++y)
			{
				for (int x = 0; x < 32; ++x)
				{
					Assert::AreEqual(0, grid.at(x, y));
				}
			}

		}
	};
}
