#include "Simulation.hpp"
#include "Board.hpp"
#include "Snake.hpp"
#include <iostream>


Simulation::Simulation()
{
}


Simulation::~Simulation()
{
}

const Point<std::size_t> Simulation::getNextSnakePosition(const Board& board, const Snake& snake) const
{
	Snake::Direction direction = snake.getDirection();
	Point<std::size_t> position = snake.getHeadPosition();

	switch (direction) {
	case Snake::Direction::right:
		std::cout << "snake direction right" << std::endl;
		position.x_ += 1;
		break;
	case Snake::Direction::left:
		std::cout << "nake directionrun left" << std::endl;
		position.x_ -= 1;
		break;
	case Snake::Direction::up:
		std::cout << "nake direction run up" << std::endl;
		position.y_ -= 1;
		break;
	case Snake::Direction::down:
		std::cout << "nake direction run down" << std::endl;
		position.y_ += 1;
		break;
	default:
		break;
	}

	return position;
}

const bool Simulation::checkForCollision(Board& board, const Point<std::size_t>& target) const
{
	Cell* cell = board.findCell(target);

	if (cell->type_ == Cell::Type::wall || cell->type_ == Cell::Type::head)
		return true;

	return false;
}

const bool Simulation::checkForFood(Board& board, const Point<std::size_t>& target) const
{
	Cell* cell = board.findCell(target);

	if (cell->type_ == Cell::Type::food)
		return true;

	return false;
}

void Simulation::updateSnakePosition(Board& board, Snake& snake, const Point<std::size_t> target)
{
	snake.updatePosition(board, target);
}