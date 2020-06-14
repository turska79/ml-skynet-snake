#pragma once

#include <cstddef>
#include <list>
#include "Point.hpp"

class Board;
class Keyboard;

class Snake
{
public:
	enum class Direction { up, down, left, right };

	void init(const Point<std::size_t> position, const Direction direction, Board& board);
	void updatePosition(Board& board, const Point<std::size_t> newHeadPosition);
	void setDirection(const Direction direction) noexcept;
	const Snake::Direction getDirection() const noexcept;
	Point<std::size_t> getHeadPosition() const noexcept;
	Point<std::size_t> getFirstBodyPosition() noexcept;
	const unsigned int getSpeed() const noexcept;
	void grow(const unsigned int length);
	const unsigned int length() const noexcept;
private:
	Point<std::size_t> headPosition_{ 0,0 };
	std::list<Point<std::size_t>> body_;
	Direction direction_{ Direction::right };
public:
	Direction previousDirection_{ Direction::right };
private:
	unsigned int speed_{ 10 };
};

