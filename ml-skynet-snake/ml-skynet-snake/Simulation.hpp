#pragma once

#include <cstddef>
#include "Point.hpp"

class Board;
class Snake;

class Simulation
{
public:
	Simulation();
	virtual ~Simulation();

	const Point<std::size_t> getNextSnakePosition(const Board& board, const Snake& snake) const;
	const bool checkForCollision(Board& board, const Point<std::size_t>& target) const;
	const bool checkForFood(Board& board, const Point<std::size_t>& target) const;
	void updateSnakePosition(Board& board, Snake& snake, const Point<std::size_t> target);
};

