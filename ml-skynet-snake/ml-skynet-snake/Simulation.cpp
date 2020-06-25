#include "Simulation.hpp"
#include "Board.hpp"
#include "Snake.hpp"
#include <iostream>

const Point<std::size_t> Simulation::getNextSnakePosition(const Point<std::size_t> currentPosition, const SnakeMovement::Direction direction) const noexcept
{
	Point<std::size_t> position = currentPosition;

	switch (direction) {
	case SnakeMovement::Direction::right:
		position.x_ += 1;
		break;
	case SnakeMovement::Direction::left:
		position.x_ -= 1;
		break;
	case SnakeMovement::Direction::up:
		position.y_ -= 1;
		break;
	case SnakeMovement::Direction::down:
		position.y_ += 1;
		break;
	default:
		break;
	}

	return position;
}

const bool Simulation::checkForCollisionWithWall(const Board& board, const Point<std::size_t>& target) const
{
	const Cell* cell = board.findCell(target);

	if (cell->type_ == Cell::Type::wall)
		return true;

	return false;
}

const bool Simulation::checkForCollisionWithSnakeBody(const Board & board, const Point<std::size_t>& target) const
{
	const Cell* cell = board.findCell(target);

	if (cell->type_ == Cell::Type::body)
		return true;

	return false;
}

const bool Simulation::checkForFood(const Board& board, const Point<std::size_t>& target) const
{
	const Cell* cell = board.findCell(target);

	if (cell->type_ == Cell::Type::food) {
		std::cout << "Simulation::checkForFood targetx: " << std::to_string(static_cast<int>(target.x_)) << " y: " << std::to_string(static_cast<int>(target.y_)) << std::endl;
		std::cout << "Simulation::checkForFood Cell x: " << std::to_string(static_cast<int>(cell->x_)) << " y: " << std::to_string(static_cast<int>(cell->y_)) << std::endl;
		return true;
	}

	return false;
}

void Simulation::updateSnakePosition(Snake& snake, const Point<std::size_t> target)
{
	snake.updatePosition(target);
}


