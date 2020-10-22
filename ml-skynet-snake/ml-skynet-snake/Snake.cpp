#include "Snake.hpp"
#include "Board.hpp"
#include "Input.hpp"
#include <SDL.h>
#include <iostream>

constexpr unsigned int growByOne{ 1 };

Snake::Snake(Board& board) : board_(board)
{
/*
FFN<MeanSquaredError<>, GaussianInitialization> model(MeanSquaredError<>(), GaussianInitialization(0, 0.001));
model.Add<Linear<>>(2, 128);		// "2" indicates there are two values to specify state for MountainCar
model.Add<ReLULayer<>>();
model.Add<Linear<>>(128, 128);
model.Add<ReLULayer<>>();
model.Add<Linear<>>(128, 3);		// "3" indicates there are three possible actions for MountainCar

*/
	/*
	mlpack::ann::FFN< mlpack::ann::MeanSquaredError<>, mlpack::ann::GaussianInitialization> model(mlpack::ann::MeanSquaredError<>(), mlpack::ann::GaussianInitialization(0, 0.001));
	model.Add< mlpack::ann::Linear<>>(26, 128);
	model.Add< mlpack::ann::ReLULayer<>>();
	model.Add< mlpack::ann::Linear<>>(128, 128);
	model.Add< mlpack::ann::ReLULayer<>>();
	model.Add< mlpack::ann::Linear<>>(128, 4);

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
	brain.setSnakeMovementInterface(static_cast<SnakeControl*> (this));
	*/
}

void Snake::init(const Point<std::size_t> position, const Direction direction)
{
	body_.clear();

	headPosition_ = position;

	Cell* cell = board_.findCell(headPosition_);
	cell->type_ = Cell::Type::head;
	
	setDirection(direction);
	
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

void Snake::setDirection(const SnakeControl::Direction direction) noexcept
{
	direction_ = direction;
}

const SnakeControl::Direction Snake::getDirection() const noexcept
{
	return direction_;
}

Point<std::size_t> Snake::getPosition() const noexcept
{
	return headPosition_;
}

void Snake::grow(const unsigned int length) noexcept
{
	body_.emplace_back(Point<std::size_t>(headPosition_));
}

const unsigned int Snake::length() const noexcept
{
	return static_cast<unsigned int>(body_.size());
}

void Snake::update(const uint32_t delta) noexcept
{
	lastUpdateTime_ += delta;

	if (lastUpdateTime_ >= updateRate) {
		auto position = getPosition();
		SnakeControl::Direction direction = getDirection(); 
		
		const Point<std::size_t> target = getNextSnakePosition(position, direction);

		const bool collision = checkForCollision(target);
		
		lastUpdateTime_ = 0;

		if (collision) {
			notifyCollisionObservers();
			return;
		} 

		const bool food = board_.isFoodCell(target);

		if (food) {
			notifyFoodEatenObservers();
			grow(growByOne);
		}

		updatePosition(target);
		notifyPositionObservers();
	}
}

SnakePositionUpdatedSubject& Snake::positionUpdateSubject() noexcept
{
	return positionUpdateSubject_;
}

SnakeCollisionSubject & Snake::snakeCollisionSubject() noexcept
{
	return snakeCollisionSubject_;
}

FoodEatenSubject& Snake::foodEatenSubject() noexcept
{
	return foodEatenSubject_;
}

const Point<std::size_t> Snake::getNextSnakePosition(const Point<std::size_t> currentPosition, const SnakeControl::Direction direction) const noexcept
{
	Point<std::size_t> position = currentPosition;

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

const bool Snake::checkForCollision(const Point<std::size_t> target)
{
	const Cell* cell = board_.findCell(target);
	
	if (cell->type_ == Cell::Type::wall || cell->type_ == Cell::Type::body) {
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


/*
SnakeBrain& Snake::brain() noexcept
{
	return learningAgent_->Environment();
}

void Snake::runLearningAgentForSingleGame()
{
	learningAgent_->Episode();
}
*/
void Snake::updatePosition(const Point<std::size_t> newPosition)
{
	//std::cout << "Snake::updatePosition()" << std::endl;

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

