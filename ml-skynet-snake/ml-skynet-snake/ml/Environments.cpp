#include "Environments.hpp"
#include "../SnakeControl.hpp"
#include "../Board.hpp"
#include "../utils/InterruptibleThread.hpp"
//#include "../ml/LearningAgent.hpp"

// Function to calculate distance 
float distance(int x1, int y1, int x2, int y2)
{
	// Calculating distance 
	return sqrt(pow(x2 - x1, 2) +
		pow(y2 - y1, 2) * 1.0);
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
	}
	else if (currentDirection == SnakeControl::Direction::right) {
		if (action == left) {
			newDirection = SnakeControl::Direction::up;
		}
		else if (action == right) {
			newDirection = SnakeControl::Direction::down;
		}
	}
	else if (currentDirection == SnakeControl::Direction::down) {
		if (action == left) {
			newDirection = SnakeControl::Direction::left;
		}
		else if (action == right) {
			newDirection = SnakeControl::Direction::right;
		}
	}
	else if (currentDirection == SnakeControl::Direction::left) {
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
		//std::cout << "LearningAgent::runLearningAgent() action: " << std::to_string(action[i]) << std::endl;
	}

	SnakeControl::Direction newDirection{ actionToDirection(action, currentDirection) };

	return newDirection;
}

ml::ContinuousActionEnvironment::ContinuousActionEnvironment()
{

}

ml::ContinuousActionEnvironment::ContinuousActionEnvironment(SnakeControl* snakeControl, SnakeVision* snakeVision, Board* board, EnvironmentState* envState) :
	snakeControl_(snakeControl), snakeVision_(snakeVision), board_(board), envState_(envState)
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
	//std::cout << "ContinuousActionEnvironment::snapshot()" << std::endl;
	ml::ContinuousActionEnvironment::State state;

	auto vision = snakeVision_->lookInAllDirections(*board_, fromPoint);

	arma::colvec& data = state.Data();

	std::copy(std::begin(vision), std::end(vision), std::begin(data));

	state.x() = fromPoint.x_;
	state.y() = fromPoint.y_;

	/*
	state.x() = fromPoint.x_;
	state.y() = fromPoint.y_;

	auto direction = snakeControl_->getDirection();

	if (direction == SnakeControl::Direction::left) {
		state.directionVectorX() = -1;
	} else if (direction == SnakeControl::Direction::right) {
		state.directionVectorX() = 1;
	} else {
		state.directionVectorX() = 0;
	}

	if (direction == SnakeControl::Direction::up) {
		state.directionVectorY() = -1;
	}  else if (direction == SnakeControl::Direction::down) {
		state.directionVectorY() = 1;
	} else {
		state.directionVectorY() = 0;
	}
	*/
	//Cell* cell = board_->findFood();

	//state.foodPositionX() = cell->x_;
	//state.foodPositionY() = cell->y_;

	return state;
}

double ml::ContinuousActionEnvironment::Sample(const ml::ContinuousActionEnvironment::State& currentState, const ml::ContinuousActionEnvironment::Action action, ml::ContinuousActionEnvironment::State& nextState)
{
	//std::cout << "ContinuousActionEnvironment::Sample()" << std::endl;
	double reward{ 0.0 };

	Cell* foodCell = board_->findFood();
	utils::Point<size_t> foodPosition(foodCell->x_, foodCell->y_);
	auto currentPosition{ currentState.position() };

	//std::cout << "ContinuousActionEnvironment::Sample() current position x: " << std::to_string(currentPosition.x_) << " y: " << std::to_string(currentPosition.y_) << std::endl;

	auto distanceToFood_ = distance(currentPosition.x_, currentPosition.y_, foodCell->x_, foodCell->y_);

	auto snakeDirection{ actionsToDirection(action.action, snakeControl_->getDirection()) };

	snakeControl_->setDirection(snakeDirection);

	envState_->environmentState() = ml::EnvironmentState::State::Idle;

	try {
		std::condition_variable cv;
		std::mutex mutex;
		std::unique_lock<std::mutex> lock(mutex);
		//auto function = std::bind(&LearningAgent::processNextStep, this);
		//wait([&]() {return check(a, b, c); });
		auto function = [&]() -> bool { return envState_->environmentState() == ml::EnvironmentState::State::Process || envState_->environmentState() == ml::EnvironmentState::State::Stop; };

		//std::cout << "ContinuousActionEnvironment::Sample() waiting for Process" << std::endl;
		thread::utils::interruptibleWait<decltype(function)>(cv, lock, function);

		//std::cout << "ContinuousActionEnvironment::Sample() processing" << std::endl;

	}
	catch (const std::exception&) {
		std::cout << "ContinuousActionEnvironment::Sample() exception" << std::endl;
	}

	envState_->environmentState() = ml::EnvironmentState::State::Processing;

	//auto nextPosition{ snakeControl_.getNextPosition() };

	//nextState.Data() = snapshot(snakeControl_->getPosition()).Encode();
	//auto statesnap(snapshot(snakeControl_->getPosition()));
	nextState = snapshot(snakeControl_->getPosition());

	//nextState.setData(statesnap.Encode());
	//nextState.x() = statesnap.x();
	//nextState.y() = statesnap.y();

	nextState.step() = currentState.step() + 1;

	auto nextPosition{ nextState.position() };

	//std::cout << "ContinuousActionEnvironment::Sample() nextPosition x: " << std::to_string(nextPosition.x_) << " y: " << std::to_string(nextPosition.y_) << std::endl;

	
	//foodCell->
	if (foodPosition == nextPosition) {
		reward = 500;
	}
	else if (board_->isWall(nextPosition)) { // || step_ >= maxSteps()) {// || board_.isSnakeBody(nextPosition)) {
		reward = -500;
		//episodeRunning_ = false;
	} else {
		double newDistanceToFood = distance(nextPosition.x_, nextPosition.y_, foodPosition.x_, foodPosition.y_);

		if (newDistanceToFood > distanceToFood_) {
			reward = -50;
		}
		else {
			reward = 50;
		}
	}

	//std::cout << "ContinuousActionEnvironment::Sample() reward: " << std::to_string(reward) << std::endl;

	return reward;
}

bool ml::ContinuousActionEnvironment::IsTerminal(const ml::ContinuousActionEnvironment::State& state)
{
	bool terminal{ false };
	const auto position{ state.position() };

	terminal = board_->isWall(position);// || board_->isSnakeBody(position);

	std::cout << "ContinuousActionEnvironment::IsTerminal() x: " << std::to_string(position.x_) << " y: " << std::to_string(position.y_) << " terminal: " << terminal << std::endl;

	return terminal;
}


