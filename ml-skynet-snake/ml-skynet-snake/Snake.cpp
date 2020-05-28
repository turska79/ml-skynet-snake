#include "Snake.hpp"
#include "Board.hpp"
#include "Input.hpp"
#include <SDL.h>
#include <iostream>

void Snake::init(Point<std::size_t> position, Board& board)
{
	headPosition_ = position;

	Cell* cell = board.findCell(headPosition_);
	cell->type_ = Cell::Type::head;
}

void Snake::setDirection(const Direction direction)
{
	direction_ = direction;
}

const Snake::Direction Snake::getDirection() const
{
	return direction_;
}

Point<std::size_t> Snake::getHeadPosition() const
{
	return headPosition_;
}

const unsigned int Snake::getSpeed() const
{
	return speed_;
}

void Snake::grow(const unsigned int length)
{
	body_.emplace_back(Point<std::size_t>(headPosition_));
}

void Snake::updatePosition(Board& board, const Point<std::size_t> newHeadPosition)
{
	Cell* cell = board.findCell(headPosition_);
	if (body_.empty()) {
		cell->type_ = Cell::Type::empty;
	} else {
		cell->type_ = Cell::Type::body;
		body_.pop_front();
	}

	headPosition_ = newHeadPosition;
	cell = board.findCell(headPosition_);
	cell->type_ = Cell::Type::head;
}

