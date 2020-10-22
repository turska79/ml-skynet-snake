#include "SnakeBrain.hpp"
#include "Board.hpp"
#include "Renderer.hpp"
#include "Simulation.hpp"
#include "SnakeControl.hpp"
#include "Game.hpp"
#include "Snake.hpp"

void SnakeBrain::setBoard(Board* board)
{
	board_ = board;
}

void SnakeBrain::setSnakeMovementInterface(SnakeControl* snakeControl)
{
	snakeControl_ = snakeControl;
//	SnakeObserver* observer = static_cast<SnakeObserver*>(this);
//	snakeControl_->attach(observer);
}

void SnakeBrain::setRenderer(Renderer* renderer)
{
	renderer_ = renderer;
}

void SnakeBrain::setSimulation(Simulation* simulation)
{
	simulation_ = simulation;
}

void SnakeBrain::setGame(Game* game)
{
	game_ = game;
}

void SnakeBrain::setSnake(Snake* snake)
{
	snake_ = snake;
}

SnakeVision& SnakeBrain::snakeVision()
{
	return snakeVision_;
}
/*
void SnakeBrain::positionUpdated()
{
	ProceedToNextMove();
}

void SnakeBrain::collision()
{
	//ProceedToNextMove();
}*/


void SnakeBrain::ProceedToNextMove()
{
	std::cout << "SnakeBrain::ProceedToNextMove()" << std::endl;
	moveCv_->notify_one();
}


SnakeBrain::SnakeBrain()
{
	moveMutex_ = new std::mutex;
	moveCv_ = new std::condition_variable;
}

double SnakeBrain::Sample(const State& state, const Action& action, State& nextState)
{
	// Update the number of steps performed.
	stepsPerformed++;

	auto position = snakeControl_->getPosition();
	SnakeControl::Direction direction;

	if (action == up) {
		direction = SnakeControl::Direction::up;
		//std::cout << "SnakeBrain::Sample() Direction: up" << std::endl;
	}
	else if (action == down) {
		direction = SnakeControl::Direction::down;
		//std::cout << "SnakeBrain::Sample() Direction: down" << std::endl;
	}
	else if (action == right) {
		direction = SnakeControl::Direction::right;
		//std::cout << "SnakeBrain::Sample() Direction: right" << std::endl;
	}
	else if (action == left) {
		direction = SnakeControl::Direction::left;
		//std::cout << "SnakeBrain::Sample() Direction: left" << std::endl;
	}
	
	snake_->setDirection(direction);

//	auto target = simulation_->getNextSnakePosition(position, direction);
	/*auto& x = nextState.coordinateX();
	auto& y = nextState.coordinateY();

	x = static_cast<double>(target.x_);
	y = static_cast<double>(target.y_);
	*/
	bool done = IsTerminal(nextState);

	if (done && maxSteps != 0 && stepsPerformed >= maxSteps) {
		return doneReward;
	} else if (done) {
		return -100;
	}
	
	std::cout << "SnakeBrain::Sample() before wait" << std::endl;
	std::unique_lock<std::mutex> lk(*moveMutex_);
	moveCv_->wait(lk);

	std::cout << "SnakeBrain::Sample() after wait" << std::endl;

	bool food{ false };
	/*
	Cell* cell = board_->findCell(target);
	
	if (cell->type_ != Cell::Type::wall) {
		auto vision = snakeVision_.lookInAllDirections(*board_, target, *simulation_, *renderer_);
		auto& data = nextState.Data();
		std::copy(std::begin(vision), std::end(vision), std::begin(data));
	}

	food = simulation_->checkForCollisionWithFood(target);
	
	if (food) {
		if (stepsPerformed > 100) {
			stepsPerformed -= 100;
		} else {
			stepsPerformed = 0;
		}

		return foodReward;
	}
	*/
	return 0;
}

double SnakeBrain::Sample(const State& state, const Action& action)
{
	State nextState;
	return Sample(state, action, nextState);
}

SnakeBrain::State SnakeBrain::InitialSample()
{
	stepsPerformed = 0;

	auto vision = snakeVision_.lookInAllDirections(*board_, snakeControl_->getPosition(), *simulation_, *renderer_);
	auto position = snakeControl_->getPosition();

	arma::colvec data(24);
	std::copy(std::begin(vision), std::end(vision), std::begin(data));
	//data[24] = position.x_;
	//data[25] = position.y_;

	//State state(data);
	//state.coordinateX() = position.x_;
	//state.coordinateY() = position.y_;
	return State (data);
}

bool SnakeBrain::IsTerminal(const State& state) const
{
	if (maxSteps != 0 && stepsPerformed >= maxSteps) {
		std::cout << "Episode terminated due to the maximum number of steps being taken." << std::endl;
		return true;
	}

	utils::Point<std::size_t> current{ snakeControl_->getPosition() };
	//auto target = simulation_->getNextSnakePosition(current, snakeControl_->getDirection());
	/*
	target.x_ = static_cast<std::size_t>(state.coordinateX());
	target.y_ = static_cast<std::size_t>(state.coordinateY());
	*/
	//return simulation_->checkForCollisionWithWall(target);
	return false;
}

size_t SnakeBrain::StepsPerformed() const
{
	return stepsPerformed;
}

size_t SnakeBrain::MaxSteps() const
{
	return maxSteps;
}

size_t& SnakeBrain::MaxSteps()
{
	return maxSteps;
}
