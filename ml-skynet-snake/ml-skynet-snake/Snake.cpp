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

void Snake::setDirection(const Direction direction) noexcept
{
	direction_ = direction;
}

const Snake::Direction Snake::getDirection() const noexcept
{
	return direction_;
}

Point<std::size_t> Snake::getHeadPosition() const noexcept
{
	return headPosition_;
}

const unsigned int Snake::getSpeed() const noexcept
{
	return speed_;
}

void Snake::grow(const unsigned int length)
{
	body_.emplace_back(Point<std::size_t>(headPosition_));
}

void Snake::updatePosition(Board& board, const Point<std::size_t> newHeadPosition)
{
	Cell* currentHeadCell = board.findCell(headPosition_);

	if (body_.empty()) {
		currentHeadCell->type_ = Cell::Type::empty;
	} else {
		currentHeadCell->type_ = Cell::Type::body;
		body_.push_back(headPosition_);
		
		Cell* currentTail = board.findCell(body_.front());
		currentTail->type_ = Cell::Type::empty;
		body_.pop_front();
	}

	headPosition_ = newHeadPosition;
	Cell* newHeadCell = board.findCell(headPosition_);
	newHeadCell->type_ = Cell::Type::head;
}

