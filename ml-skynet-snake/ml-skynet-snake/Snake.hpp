#pragma once

#include <cstddef>
#include <list>
#include "Point.hpp"

class Board;

class Snake
{
public:
	Snake();
	~Snake();

	void init(Point<std::size_t> position, Board& board);
	void updatePosition(Board& board);
	//void Grow(int amount);
	//void UpdatePosition(Board& board);

	enum class Move { up, down, left, right };

	//Move last_dir = Move::up;
	//Move dir = Move::up;

private:
	Point<std::size_t> headPosition_{ 0,0 };
	std::list<Point<std::size_t>> body;

	//int size = 1;
	//float speed = 0.5f;
	//int growing = 0;
};

