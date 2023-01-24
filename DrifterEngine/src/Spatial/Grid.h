#pragma once
#include <vector>

namespace drft::spatial
{
	template<typename T>
	class Grid
	{
	public:
		Grid(int width, int height);

		T& at(int x, int y);
		const T& at(int x, int y) const;

		int width() const;
		int height() const;

		void clear();

	private:
		std::vector<T> _cells;
		int _width;
		int _height;
	};

} // namespace drft

#include "Grid.inl"



