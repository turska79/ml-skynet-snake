#include "Snake.hpp"
#include "Board.hpp"
#include "Input.hpp"
#include <SDL.h>
#include <iostream>

Snake::Snake(Board& board) : board_(board)
{
	mlpack::ann::FFN< mlpack::ann::MeanSquaredError<>, mlpack::ann::GaussianInitialization> model(mlpack::ann::MeanSquaredError<>(), mlpack::ann::GaussianInitialization(0, 0.001));
	model.Add< mlpack::ann::Linear<>>(26, 16);
	model.Add< mlpack::ann::ReLULayer<>>();
	model.Add< mlpack::ann::Linear<>>(16, 8);
	model.Add< mlpack::ann::ReLULayer<>>();
	model.Add< mlpack::ann::Linear<>>(8, 4);

	mlpack::rl::GreedyPolicy<SnakeBrain> policy(1.0, 100, 0.1, 0.99);
	mlpack::rl::RandomReplay<SnakeBrain> replayMethod(30, 1000);

	mlpack::rl::TrainingConfig config;
	config.StepSize() = 0.1;
	config.Discount() = 0.5;
	config.TargetNetworkSyncInterval() = 10;
	config.ExplorationSteps() = 10;
	config.DoubleQLearning() = false;
	config.StepLimit() = 500;

	learningAgent_ = new mlpack::rl::QLearning<SnakeBrain, decltype(model), ens::AdamUpdate, decltype(policy)>(std::move(config), std::move(model), std::move(policy), std::move(replayMethod));

	auto& brain = learningAgent_->Environment();
	brain.setSnakeMovementInterface(static_cast<SnakeMovement*> (this));
}

void Snake::init(const Point<std::size_t> position, const Direction direction)
{
	body_.clear();

	headPosition_ = position;

	Cell* cell = board_.findCell(headPosition_);
	cell->type_ = Cell::Type::head;
	
	setDirection(direction);
	previousDirection_ = direction;
	
	Point<std::size_t> bodyPosition = headPosition_;
	bodyPosition.x_ -= 1;
	cell = board_.findCell(bodyPosition);
	cell->type_ = Cell::Type::body;
	body_.emplace_back(bodyPosition);

	bodyPosition.x_ -= 1;
	cell = board_.findCell(bodyPosition);
	cell->type_ = Cell::Type::body;
	body_.emplace_back(bodyPosition);
}

void Snake::setDirection(const SnakeMovement::Direction direction) noexcept
{
	if (direction_ != direction) {
		previousDirection_ = direction_;
	}

	direction_ = direction;
}

const SnakeMovement::Direction Snake::getDirection() const noexcept
{
	return direction_;
}

Point<std::size_t> Snake::getPosition() const noexcept
{
	return headPosition_;
}

const unsigned int Snake::getSpeed() const noexcept
{
	return speed_;
}

void Snake::grow(const unsigned int length) noexcept
{
	body_.emplace_back(Point<std::size_t>(headPosition_));
}

const unsigned int Snake::length() const noexcept
{
	return static_cast<unsigned int>(body_.size());
}

SnakeBrain& Snake::brain() noexcept
{
	return learningAgent_->Environment();
}

void Snake::runLearningAgentForSingleGame()
{
	learningAgent_->Episode();
}

void Snake::updatePosition(const Point<std::size_t> newPosition)
{
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

	brain().ProceedToNextMove();
}

