#pragma once

#include "Point.hpp"
#include <cstddef>

class Board;

class Food
{
public:
	void init(Board& board, const Point<std::size_t> position);
	void updatePosition(Board& board, const Point<std::size_t> position);
	const Point<std::size_t> position() const;
private:
	Point<std::size_t> position_{ 0, 0 };
};

