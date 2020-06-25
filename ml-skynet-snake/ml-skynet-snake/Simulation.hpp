#pragma once

#include <cstddef>
#include "Point.hpp"
#include "SnakeMovement.hpp"

class Board;
class Snake;

class Simulation
{
public:
	const Point<std::size_t> getNextSnakePosition(const Point<std::size_t> currentPosition, const SnakeMovement::Direction direction) const noexcept;
	const bool checkForCollisionWithWall(const Board& board, const Point<std::size_t>& target) const;
	const bool checkForCollisionWithSnakeBody(const Board& board, const Point<std::size_t>& target) const;
	const bool checkForFood(const Board& board, const Point<std::size_t>& target) const;
	void updateSnakePosition(Snake& snake, const Point<std::size_t> target);

};

