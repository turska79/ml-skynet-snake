#include "Simulation.hpp"
#include "Board.hpp"
#include "Snake.hpp"
#include <iostream>
#include <algorithm>

constexpr unsigned int secondAsMilliseconds{ 1000 };

Simulation::Simulation(Board& board) : board_(board)
{
}

void Simulation::start()
{
	running_ = true;
	collision_ = false;
	timer_.start();
}

void Simulation::update(SnakeMovement& snakeMovement)
{
	if (!running_) {
		return;
	}

	const uint32_t deltaTime = timer_.deltaTime();

	updateDeltaTime_ += deltaTime;

	if (!collision_ && updateDeltaTime_ > (secondAsMilliseconds / snakeSpeed_)) {

		auto position = snakeMovement.getPosition();
		SnakeMovement::Direction direction = snakeMovement.getDirection();

		const Point<std::size_t> target = getNextSnakePosition(position, direction);

		collision_ = checkForCollisionWithWall(target);

		if (collision_) {
			return;
		}

		snakeMovement.updatePosition(target);

		updateDeltaTime_ = updateDeltaTime_ - (secondAsMilliseconds / snakeSpeed_);

		if (updateDeltaTime_ > 500) {
			updateDeltaTime_ = 0;
		}
	}
	
}

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
const bool Simulation::collision() const
{
	return collision_;
}

const bool Simulation::checkForCollisionWithFood(const Point<std::size_t>& target) const
{
	const Cell* cell = board_.findCell(target);

	if (cell->type_ == Cell::Type::food)
		return true;

	return false;
}

const bool Simulation::checkForCollisionWithWall(const Point<std::size_t>& target) const
{
	const Cell* cell = board_.findCell(target);

	if (cell->type_ == Cell::Type::wall)
		return true;

	return false;
}

const bool Simulation::checkForCollisionWithSnakeBody(const Point<std::size_t>& target) const
{
	const Cell* cell = board_.findCell(target);

	if (cell->type_ == Cell::Type::body)
		return true;

	return false;
}
