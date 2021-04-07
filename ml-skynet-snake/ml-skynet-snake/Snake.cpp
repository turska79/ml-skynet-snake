#include "Snake.hpp"
#include "Board.hpp"
#include "Input.hpp"
//#include <SDL.h>
#include <iostream>
#include <string>

constexpr unsigned int growByOne{ 1 };

Snake::Snake(Board& board) : board_(board)
{

}

void Snake::init(const utils::Point<std::size_t> position, const Direction direction)
{
	body_.clear();

	headPosition_ = position;

	Cell* cell = board_.findCell(headPosition_);
	cell->type_ = Cell::Type::head;
	
	setDirection(direction);
	
	utils::Point<std::size_t> bodyPosition = headPosition_;
	bodyPosition.x_ -= 1;
	cell = board_.findCell(bodyPosition);
	cell->type_ = Cell::Type::body;
	body_.emplace_back(bodyPosition);

	bodyPosition.x_ -= 1;
	cell = board_.findCell(bodyPosition);
	cell->type_ = Cell::Type::body;
	body_.emplace_back(bodyPosition);
}

void Snake::setDirection(const SnakeControl::Direction direction) noexcept
{
	direction_ = direction;

	switch (direction_) {
	case SnakeControl::Direction::up:
		std::cout << "Snake::setDirection(): up" << std::endl;
		break;
	case SnakeControl::Direction::right:
		std::cout << "Snake::setDirection(): right" << std::endl;
		break;
	case SnakeControl::Direction::down:
		std::cout << "Snake::setDirection(): down" << std::endl;
		break;
	case SnakeControl::Direction::left:
		std::cout << "Snake::setDirection(): left" << std::endl;
		break;
	}
}

const SnakeControl::Direction Snake::getDirection() const noexcept
{
	return direction_;
}

const utils::Point<std::size_t> Snake::getPosition() const noexcept
{
	return headPosition_;
}

void Snake::grow(const unsigned int) noexcept
{
	body_.emplace_back(utils::Point<std::size_t>(headPosition_));
}

const unsigned int Snake::length() const noexcept
{
	return static_cast<unsigned int>(body_.size());
}

void Snake::update(const uint32_t delta) noexcept
{
	lastUpdateTime_ += delta;

	if (lastUpdateTime_ >= updateRate) {
		const utils::Point<std::size_t> target = getNextPosition();

		const bool collision = checkForCollision(target);
		
		lastUpdateTime_ = 0;

		const bool food = board_.isFood(target);

		if (food) {
			notifyFoodEatenObservers();
			grow(growByOne);
		}

		if (!collision) {
			updatePosition(target);
		}
		
		notifyPositionObservers();
		

		if (collision) {
			notifyCollisionObservers();
			//return;
		}
	}
}

subjects::SnakePositionUpdatedSubject& Snake::positionUpdateSubject() noexcept
{
	return positionUpdateSubject_;
}

subjects::SnakeCollisionSubject & Snake::snakeCollisionSubject() noexcept
{
	return snakeCollisionSubject_;
}

subjects::FoodEatenSubject& Snake::foodEatenSubject() noexcept
{
	return foodEatenSubject_;
}

const utils::Point<std::size_t> Snake::getNextPosition() const noexcept
{
	utils::Point<std::size_t> position{ getPosition() };
	const auto direction{ getDirection() };

	switch (direction) {
	case SnakeControl::Direction::right:
		position.x_ += 1;
		break;

	case SnakeControl::Direction::left:
		position.x_ -= 1;
		break;

	case SnakeControl::Direction::up:
		position.y_ -= 1;
		break;

	case SnakeControl::Direction::down:
		position.y_ += 1;
		break;

	default:
		break;
	}

	return position;
}

const bool Snake::checkForCollision(const utils::Point<std::size_t> target)
{
	const Cell* cell = board_.findCell(target);
	
	if (cell->type_ == Cell::Type::wall){// || cell->type_ == Cell::Type::body) {
		return true;
	}

	return false;
}

void Snake::notifyPositionObservers() noexcept
{
	positionUpdateSubject_.invoke();
}

void Snake::notifyCollisionObservers() noexcept
{
	snakeCollisionSubject_.invoke();
}

void Snake::notifyFoodEatenObservers() noexcept
{
	foodEatenSubject_.invoke();
}

void Snake::updatePosition(const utils::Point<std::size_t> newPosition)
{
	std::cout << "Snake::updatePosition() x: " << std::to_string(newPosition.x_) << " y: " << std::to_string(newPosition.y_) << std::endl;
	Cell* currentHeadCell = board_.findCell(headPosition_);

	if (body_.empty()) {
		currentHeadCell->type_ = Cell::Type::empty;
	} else {
		currentHeadCell->type_ = Cell::Type::body;
		body_.push_back(headPosition_);
		
		Cell* currentTail = board_.findCell(body_.front());
		currentTail->type_ = Cell::Type::empty;
		body_.pop_front();
	}

	headPosition_ = newPosition;
	Cell* newHeadCell = board_.findCell(headPosition_);
	newHeadCell->type_ = Cell::Type::head;
}

