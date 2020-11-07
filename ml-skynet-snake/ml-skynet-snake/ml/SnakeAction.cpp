#include "SnakeAction.hpp"
#include "../Board.hpp"
#include "../SnakeControl.hpp"
#include "../InterruptibleThread.hpp"
#include <string>
#pragma warning(disable : 26819 26812)
#include <SDL_timer.h>
#pragma warning( pop )

static uint32_t oldTime{ 0 };

void ml::SnakeAction::setBoard(Board* board)
{
	board_ = board;
}

void ml::SnakeAction::setSnakeControl(SnakeControl* snakeControl)
{
	snakeControl_ = snakeControl;
}

SnakeVision& ml::SnakeAction::snakeVision()
{
	return snakeVision_;
}

bool ml::SnakeAction::isReadyForNextStep()
{
	return readyForNextStep_;
}

bool ml::SnakeAction::notReadyForNextStep()
{
	return !readyForNextStep_;
}

void ml::SnakeAction::proceedToNextStep()
{
	//std::cout << "SnakeAction::proceedToNextStep() ready for next step: " << std::to_string(readyForNextStep_) << std::endl;

	std::condition_variable cv;
	std::mutex mutex;
	std::unique_lock<std::mutex> lock(mutex);

	auto function = std::bind(&SnakeAction::notReadyForNextStep, this);
	thread::utils::interruptibleWait<decltype(function)>(cv, lock, function);

	readyForNextStep_ = true;
}

ml::SnakeAction::SnakeAction()
{
	//std::cout << "SnakeAction::SnakeAction()" << std::endl;
	//mutex_ = std::make_unique<std::mutex>();
	//cv_ = std::make_unique<std::condition_variable>();
}

double ml::SnakeAction::Sample(const ml::State& state, const Action& action, ml::State& nextState)
{
	uint32_t now = SDL_GetTicks();
	uint32_t delta = now - oldTime;

	std::cout << "SnakeAction::Sample() action: " << std::to_string(action.action) << std::endl;
	oldTime = now;

	stepsPerformed++;
	double stupidSnakeReward{ -1 };
	bool stupid = false;

	auto position = snakeControl_->getPosition();
	SnakeControl::Direction newDirection{};
	SnakeControl::Direction currentDirection = snakeControl_->getDirection();

	auto selectedAction = action.action;

	if (selectedAction == Action::actions::up) {
		if (currentDirection == SnakeControl::Direction::down) {
			stupid = true;
			newDirection = SnakeControl::Direction::down;
		} else {
			newDirection = SnakeControl::Direction::up;
		}
		std::cout << "SnakeBrain::Sample() Direction: up" << std::endl;
	} else if (selectedAction == Action::actions::down) {
		if (currentDirection == SnakeControl::Direction::up) {
			stupid = true;
			newDirection = SnakeControl::Direction::up;
		} else {
			newDirection = SnakeControl::Direction::down;
		}
		std::cout << "SnakeBrain::Sample() Direction: down" << std::endl;
	} else if (selectedAction == Action::actions::right) {
		if (currentDirection == SnakeControl::Direction::left) {
			stupid = true;
			newDirection = SnakeControl::Direction::left;
		} else {
			newDirection = SnakeControl::Direction::right;
		}
		std::cout << "SnakeBrain::Sample() Direction: right" << std::endl;
	} else if (selectedAction == Action::actions::left) {
		if (currentDirection == SnakeControl::Direction::right) {
			stupid = true;
			newDirection = SnakeControl::Direction::right;
		} else {
			newDirection = SnakeControl::Direction::left;
		}
		std::cout << "SnakeBrain::Sample() Direction: left" << std::endl;
	}
		
	snakeControl_->setDirection(newDirection);

	if (newDirection == SnakeControl::Direction::left) {
		nextState.directionX() = -1;
	}

	if (newDirection == SnakeControl::Direction::right) {
		nextState.directionX() = 1;
	}

	if (newDirection == SnakeControl::Direction::up) {
		nextState.directionY() = -1;
	}

	if (newDirection == SnakeControl::Direction::down) {
		nextState.directionY() = +1;
	}

	const utils::Point<std::size_t> nextPosition{ snakeControl_->getNextPosition() };
	nextState.x() = nextPosition.x_;
	nextState.y() = nextPosition.y_;

	bool terminal = IsTerminal(nextState);

	if (terminal) {
		return static_cast<double>(terminalReward);
	} else if (stepsPerformed >= maxSteps) {
		return maxStepsReward;
	}

	std::condition_variable cv;
	std::mutex mutex;
	std::unique_lock<std::mutex> lock(mutex);

	auto function = std::bind(&SnakeAction::isReadyForNextStep, this);
	//std::cout << "SnakeBrain::Sample() before wait" << std::endl;

	thread::utils::interruptibleWait<decltype(function)>(cv, lock, function);
	readyForNextStep_ = false;

	//std::cout << "SnakeBrain::Sample() after wait" << std::endl;

	bool food{ false };
	
	Cell* cell = board_->findCell(nextPosition);
	
	if (cell->type_ != Cell::Type::wall) {
		auto vision = snakeVision_.lookInAllDirections(*board_, nextPosition);
		auto& data = nextState.Data();
		std::copy(std::begin(vision), std::end(vision), std::begin(data));
	}

	food = cell->type_ == Cell::Type::food ? true : false;
	
	if (food) {
		if (stepsPerformed > 100) {
			stepsPerformed -= 100;
		} else {
			stepsPerformed = 0;
		}

		return foodFoundReward;
	}
	if (stupid) {
		return stupidSnakeReward;
	}
	return emptyStepReward;
}

double ml::SnakeAction::Sample(const ml::State& state, const Action& action)
{
	State nextState;
	return Sample(state, action, nextState);
}

ml::State ml::SnakeAction::InitialSample()
{
	stepsPerformed = 0;

	auto vision = snakeVision_.lookInAllDirections(*board_, snakeControl_->getPosition());
	auto position = snakeControl_->getPosition();

	arma::colvec data(dimension);
	std::copy(std::begin(vision), std::end(vision), std::begin(data));
	data[24] = position.x_;
	data[25] = position.y_;

	auto direction = snakeControl_->getDirection();

	if (direction == SnakeControl::Direction::left) {
		data[26] = -1;
	}

	if (direction == SnakeControl::Direction::right) {
		data[26] = 1;
	}

	if (direction == SnakeControl::Direction::up) {
		data[27] = -1;
	}

	if (direction == SnakeControl::Direction::down) {
		data[27] = 1;
	}
	return State(data);
}

bool ml::SnakeAction::IsTerminal(const ml::State& state) const
{
	if (maxSteps != 0 && stepsPerformed >= maxSteps) {
		std::cout << "Episode terminated due to the maximum number of steps being taken." << std::endl;
		return true;
	}

	utils::Point<std::size_t> position{ state.position() };

	if (board_->isWall(position) || board_->isSnakeBody(position)) {
		std::cout << "Episode terminated due hitting the wall." << std::endl;
		return true;
	}

	return false;
}

size_t ml::SnakeAction::StepsPerformed() const
{
	return stepsPerformed;
}

size_t ml::SnakeAction::MaxSteps() const
{
	return maxSteps;
}

size_t& ml::SnakeAction::MaxSteps()
{
	return maxSteps;
}
