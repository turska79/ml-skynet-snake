#pragma once

#include "Point.hpp"
#include <cstddef>

class Board;

class Food
{
public:
	void init(Point<std::size_t> position, Board& board);
	void updatePosition(Board& board, const Point<std::size_t> position);

private:
	Point<std::size_t> position_{ 0, 0 };
};

