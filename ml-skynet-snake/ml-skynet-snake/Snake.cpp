#include "Snake.hpp"
#include "Board.hpp"
#include "Input.hpp"
#include <SDL.h>
#include <iostream>

void Snake::init(const Point<std::size_t> position, const Direction direction, Board& board)
{
	body_.clear();

	headPosition_ = position;

	Cell* cell = board.findCell(headPosition_);
	cell->type_ = Cell::Type::head;
	
	setDirection(direction);
	previousDirection_ = direction;
	
	Point<std::size_t> bodyPosition = headPosition_;
	bodyPosition.x_ -= 1;
	cell = board.findCell(bodyPosition);
	cell->type_ = Cell::Type::body;
	body_.emplace_back(bodyPosition);

	bodyPosition.x_ -= 1;
	cell = board.findCell(bodyPosition);
	cell->type_ = Cell::Type::body;
	body_.emplace_back(bodyPosition);
}

void Snake::setDirection(const Direction direction) noexcept
{
	if (direction_ != direction) {
		previousDirection_ = direction_;
	}

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

Point<std::size_t> Snake::getFirstBodyPosition() noexcept
{
	if (body_.empty() == false) {
		return body_.back();
	}
	return Point<std::size_t>();
}

const unsigned int Snake::getSpeed() const noexcept
{
	return speed_;
}

void Snake::grow(const unsigned int length)
{
	body_.emplace_back(Point<std::size_t>(headPosition_));
}

const unsigned int Snake::length() const noexcept
{
	return static_cast<unsigned int>(body_.size());
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

