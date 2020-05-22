#include "Snake.hpp"
#include "Board.hpp"
#include "Input.hpp"
#include <SDL.h>
#include <iostream>

Snake::Snake()
{

}


Snake::~Snake()
{
}

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

void Snake::updatePosition(Board& board, const Point<std::size_t> newHeadPosition)
{
	Cell* cell = board.findCell(headPosition_);
	cell->type_ = Cell::Type::empty;

	headPosition_ = newHeadPosition;
	cell = board.findCell(headPosition_);
	cell->type_ = Cell::Type::head;
	int arr[10];
	int* p = arr;
}
/*
void Snake::updateHeadPosition(Board& board)
{
	Cell* cell = board.findCell(headPosition_);

	switch (direction_) {
	case Direction::right:
		std::cout << "snake direction right" << std::endl;
		headPosition_.x_ += 1;
		break;
	case Direction::left:
		std::cout << "nake directionrun left" << std::endl;
		headPosition_.x_ -= 1;
	case Direction::up:
		std::cout << "nake direction run up" << std::endl;
		headPosition_.y_ -= 1;
	case Direction::down:
		std::cout << "nake direction run down" << std::endl;
		headPosition_.y_ += 1;
	default:
		break;

	}

	cell->type_ = Cell::Type::empty;
	cell = board.findCell(headPosition_);
	cell->type_ = Cell::Type::head;
}


*/
