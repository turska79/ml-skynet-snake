#include "SnakeBrain.hpp"
#include "Board.hpp"
#include "Renderer.hpp"
#include "Simulation.hpp"
#include "SnakeMovement.hpp"
#include "Game.hpp"
#include "Snake.hpp"

void SnakeBrain::setBoard(Board* board)
{
	board_ = board;
}

void SnakeBrain::setSnakeMovementInterface(SnakeMovement* snakeMovement)
{
	snakeMovement_ = snakeMovement;
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

void SnakeBrain::ProceedToNextMove()
{
	//std::cout << "SnakeBrain::ProceedToNextMove()" << std::endl;
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

	auto position = snakeMovement_->getPosition();
	SnakeMovement::Direction direction;

	if (action == up) {
		direction = SnakeMovement::Direction::up;
		//std::cout << "SnakeBrain::Sample() Direction: up" << std::endl;
	}
	else if (action == down) {
		direction = SnakeMovement::Direction::down;
		//std::cout << "SnakeBrain::Sample() Direction: down" << std::endl;
	}
	else if (action == right) {
		direction = SnakeMovement::Direction::right;
		//std::cout << "SnakeBrain::Sample() Direction: right" << std::endl;
	}
	else if (action == left) {
		direction = SnakeMovement::Direction::left;
		//std::cout << "SnakeBrain::Sample() Direction: left" << std::endl;
	}
	
	snake_->setDirection(direction);

	auto target = simulation_->getNextSnakePosition(position, direction);
	double& x = nextState.coordinateX();
	double& y = nextState.coordinateY();

	x = static_cast<double>(target.x_);
	y = static_cast<double>(target.y_);

	bool done = IsTerminal(nextState);

	if (done && maxSteps != 0 && stepsPerformed >= maxSteps) {
		return doneReward;
	} else if (done) {
		return -100;
	}
	
	std::unique_lock<std::mutex> lk(*moveMutex_);
	moveCv_->wait(lk);

	bool food{ false };

	Cell* cell = board_->findCell(target);
	
	if (cell->type_ != Cell::Type::wall) {
		auto vision = snakeVision_.lookInAllDirections(*board_, target, *simulation_, *renderer_);
		auto data = nextState.Data();
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

	auto vision = snakeVision_.lookInAllDirections(*board_, snakeMovement_->getPosition(), *simulation_, *renderer_);
	auto position = snakeMovement_->getPosition();

	arma::colvec data(26);
	std::copy(std::begin(vision), std::end(vision), std::begin(data));
	data[24] = position.x_;
	data[25] = position.y_;

	return State(data);
}

bool SnakeBrain::IsTerminal(const State& state) const
{
	if (maxSteps != 0 && stepsPerformed >= maxSteps) {
		std::cout << "Episode terminated due to the maximum number of steps being taken." << std::endl;
		return true;
	}

	//Point<std::size_t> target;
	//target.x_ = static_cast<std::size_t>(state.coordinateX());
	//target.y_ = static_cast<std::size_t>(state.coordinateY());
	
	return simulation_->collision();
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
