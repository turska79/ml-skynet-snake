#include "Environments.hpp"
#include "../SnakeControl.hpp"
#include "../Board.hpp"
#include "../utils/InterruptibleThread.hpp"

constexpr int foodReward{ 1000 };
constexpr int deathReward{ 1000 };
constexpr int snakeMovesCloserToFoodReward{ 10 };
constexpr int snakeMovesAwayFromFoodReward{ -10 };


double distance(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

SnakeControl::Direction actionToDirection(const unsigned int action, SnakeControl::Direction currentDirection)
{
	auto newDirection{ currentDirection };

	constexpr unsigned int left{ 0 };
	constexpr unsigned int forward{ 1 };
	constexpr unsigned int right{ 2 };

	if (action == forward) {
		return newDirection;
	}

	if (currentDirection == SnakeControl::Direction::up) {
		if (action == left) {
			newDirection = SnakeControl::Direction::left;
		}
		else if (action == right) {
			newDirection = SnakeControl::Direction::right;
		}
	} else if (currentDirection == SnakeControl::Direction::right) {
		if (action == left) {
			newDirection = SnakeControl::Direction::up;
		}
		else if (action == right) {
			newDirection = SnakeControl::Direction::down;
		}
	} else if (currentDirection == SnakeControl::Direction::down) {
		if (action == left) {
			newDirection = SnakeControl::Direction::left;
		}
		else if (action == right) {
			newDirection = SnakeControl::Direction::right;
		}
	} else if (currentDirection == SnakeControl::Direction::left) {
		if (action == left) {
			newDirection = SnakeControl::Direction::down;
		}
		else if (action == right) {
			newDirection = SnakeControl::Direction::up;
		}
	}

	return newDirection;
}

SnakeControl::Direction actionsToDirection(const std::vector<double> actions, SnakeControl::Direction currentDirection)
{
	unsigned int action{ 0 };
	double max{ 0 };

	auto size{ actions.size() };

	for (unsigned int i = 0; i < size; ++i) {
		if (actions[i] > max) {
			max = actions[i];
			action = i;
		}
	}

	SnakeControl::Direction newDirection{ actionToDirection(action, currentDirection) };

	return newDirection;
}

utils::Vec2<double> directionToVector(SnakeControl::Direction currentDirection)
{
	utils::Vec2<double> vector;

	if (currentDirection == SnakeControl::Direction::up) {
		vector.y = 1;
	} else if (currentDirection == SnakeControl::Direction::down) {
		vector.y = -1;
	} else {
		vector.y = 0;
	}

	if (currentDirection == SnakeControl::Direction::right) {
		vector.x = 1;
	}
	else if (currentDirection == SnakeControl::Direction::left) {
		vector.x = -1;
	}
	else {
		vector.x = 0;
	}

	return vector;
}

ml::ContinuousActionEnvironment::ContinuousActionEnvironment()
{

}

ml::ContinuousActionEnvironment::ContinuousActionEnvironment(SnakeControl* snakeControl, SnakeVision* snakeVision, Board* board, EnvironmentState* envState) 
	: snakeControl_(snakeControl), snakeVision_(snakeVision), board_(board), envState_(envState)
{

}

ml::ContinuousActionEnvironment::State ml::ContinuousActionEnvironment::InitialSample()
{
	std::cout << "ContinuousActionEnvironment::InitialSample()" << std::endl;
	auto position = snakeControl_->getPosition();
	State state{ snapshot(position) };
	
	return state;
}

ml::ContinuousActionEnvironment::State ml::ContinuousActionEnvironment::snapshot(utils::Point<std::size_t> fromPoint)
{
	ml::ContinuousActionEnvironment::State state;

	auto vision = snakeVision_->lookInAllDirections(*board_, fromPoint);

	arma::colvec& data = state.Data();

	auto iter = std::begin(data);
	std::advance(iter, utils::commonConstants::ml::inputParameters::vision_start);

	std::copy(std::begin(vision), std::end(vision), iter);

	state.position(fromPoint);

	Cell* foodCell = board_->findFood();
	utils::Point<size_t> foodPosition(foodCell->x_, foodCell->y_);

	state.foodPosition(foodPosition);

	const auto vector = directionToVector(snakeControl_->getDirection());
	state.directionVector(vector);

	return state;
}

double ml::ContinuousActionEnvironment::Sample(const ml::ContinuousActionEnvironment::State& currentState, const ml::ContinuousActionEnvironment::Action action, ml::ContinuousActionEnvironment::State& nextState)
{
	double reward{ 0.0 };

	utils::Point<size_t> foodPosition(currentState.foodPosition());
	auto currentPosition{ currentState.position() };
	auto distanceToFood_ = distance(currentPosition.x_, currentPosition.y_, foodPosition.x_, foodPosition.y_);
	auto snakeDirection{ actionsToDirection(action.action, snakeControl_->getDirection()) };

	snakeControl_->setDirection(snakeDirection);

	envState_->environmentState() = ml::EnvironmentState::State::Idle;

	try {
		std::condition_variable cv;
		std::mutex mutex;
		std::unique_lock<std::mutex> lock(mutex);
		auto function = [&]() -> bool { return envState_->environmentState() == ml::EnvironmentState::State::Process || envState_->environmentState() == ml::EnvironmentState::State::Stop; };

		thread::utils::interruptibleWait<decltype(function)>(cv, lock, function);
	}
	catch (const std::exception&) {
		std::cout << "ContinuousActionEnvironment::Sample() exception" << std::endl;
	}

	envState_->environmentState() = ml::EnvironmentState::State::Processing;

	nextState = snapshot(snakeControl_->getPosition());
	nextState.step() = currentState.step() + 1;

	auto nextPosition{ nextState.position() };

	if (foodPosition == nextPosition) {
		reward = foodReward;
	}
	else if (board_->isWall(nextPosition)) { // || step_ >= maxSteps()) {// || board_.isSnakeBody(nextPosition)) {
		reward = deathReward;
	} else {
		double newDistanceToFood = distance(nextPosition.x_, nextPosition.y_, foodPosition.x_, foodPosition.y_);
		
		if (newDistanceToFood > distanceToFood_) {
			reward = snakeMovesAwayFromFoodReward;
		}
		else {
			reward = snakeMovesCloserToFoodReward;
		}
	}

	return reward;
}

bool ml::ContinuousActionEnvironment::IsTerminal(const ml::ContinuousActionEnvironment::State& state)
{
	bool terminal{ false };
	const auto position{ state.position() };

	terminal = board_->isWall(position);// || board_->isSnakeBody(position);

	return terminal;
}


