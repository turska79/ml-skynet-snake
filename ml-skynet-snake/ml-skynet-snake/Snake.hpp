#pragma once

#include <cstddef>
#include <list>
#include "Point.hpp"
//#include <SDL.h>
class Board;
class Keyboard;

class Snake
{
public:
	Snake();
	~Snake();

	enum class Direction { up, down, left, right };

	void init(Point<std::size_t> position, Board& board);
	void updatePosition(Board& board, const Point<std::size_t> newHeadPosition);
	void processInput(const Keyboard& keyboard);
	const Snake::Direction getDirection() const;
	Point<std::size_t> getHeadPosition() const;
	const unsigned int getSpeed() const;
	//void Grow(int amount);
	//void UpdatePosition(Board& board);

	

	//Move last_dir = Move::up;
	//Move dir = Move::up;

private:
	Point<std::size_t> headPosition_{ 0,0 };
	std::list<Point<std::size_t>> body;
	Direction direction_{ Direction::right };
	//void updateHeadPosition(Board& board);

	unsigned int speed_{ 10 };
	//int size = 1;
	//float speed = 0.5f;
	//int growing = 0;
};

