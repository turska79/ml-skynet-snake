#include "RunningStateAI.h"
#include "Utils.hpp"
#include "Game.hpp"
#include "Board.hpp"
#include "GameOverState.hpp"
#include <iostream>
#include <iterator>

RunningStateAI::RunningStateAI(Game& game) : RunningState(game)
{

}

void RunningStateAI::enter()
{
	std::cout << " RunningStateAI::enter()" << std::endl;
	RunningState::enter();

	auto& brain = snake_.brain();
	brain.setBoard(static_cast<Board*>(&game_.board()));
	brain.setRenderer(static_cast<Renderer*>(&game_.renderer()));
	brain.setSimulation(static_cast<Simulation*>(&simulation_));
	brain.setGame(&game_);
	brain.setSnake(&snake_);

}

void RunningStateAI::update(Renderer& renderer)
{
/*	if (running_ == false) {
		running_ = true;
		snake_.runAi();
	}

	updateDeltaTime_ += deltaTime;
	const unsigned int snakeSpeed = snake_.getSpeed();
	const Board& board{ game_.board() };

	//std::array<float, 24> vision = snakeVision_.lookInAllDirections(board, snake_, simulation_, renderer);

	if (updateDeltaTime_ > (utils::commonConstants::secondAsMilliseconds / snakeSpeed)) {
		
		const Board& board = game_.board();
		auto position = snake_.getPosition();
		auto direction = snake_.getDirection();

		const Point<std::size_t> target = simulation_.getNextSnakePosition(position, direction);
		const bool collision = simulation_.checkForCollisionWithWall(board, target);

		if (collision) {
			running_ = false;
			RunningState::enter();
			return;
		}

		const bool food = simulation_.checkForFood(board, target);

		if (food) {
			snake_.grow(1);
			newRandomPositionForFood();
		}
		
		simulation_.updateSnakePosition(snake_, target);

		updateDeltaTime_ = updateDeltaTime_ - (utils::commonConstants::secondAsMilliseconds / snakeSpeed);
	}

	printCurrentScoreToScreen(renderer);
	*/
}

void RunningStateAI::handleInput(const Keyboard& keyboard)
{

}
/*
arma::mat RunningStateAI::snakeVision()
{
	Point<std::size_t> position = snake_.getHeadPosition();

	const Snake::Direction direction = snake_.getDirection();
	const Snake::Direction previousDirection = snake_.previousDirection_;
	float snakeDirection{ 0 };
	bool leftBlocked = false;
	bool rightBlocked = false;
	
	Board& board = game_.board();
	const Point<std::size_t> target = simulation_.getNextSnakePosition(board, snake_);
	bool frontBlocked = simulation_.checkForCollision(board, target);

	bool left = false;
	bool front = false;
	bool right = false;

	if (direction == Snake::Direction::left) {
		snakeDirection = 0.75;

		Point<std::size_t> testTarget = position;

		testTarget.y_ -= 1;
		leftBlocked = simulation_.checkForCollision(board, testTarget);

		testTarget = position;
		testTarget.y_ += 1;
		rightBlocked = simulation_.checkForCollision(board, testTarget);
	}
	else if (direction == Snake::Direction::right) {
		snakeDirection = 0.25;

		Point<std::size_t> testTarget = position;

		testTarget.y_ += 1;
		leftBlocked = simulation_.checkForCollision(board, testTarget);

		testTarget = position;
		testTarget.y_ -= 1;
		rightBlocked = simulation_.checkForCollision(board, testTarget);
	}
	else if (direction == Snake::Direction::up) {
		snakeDirection = 0;

		Point<std::size_t> testTarget = position;

		testTarget.x_ -= 1;
		leftBlocked = simulation_.checkForCollision(board, testTarget);

		testTarget = position;
		testTarget.y_ += 1;
		rightBlocked = simulation_.checkForCollision(board, testTarget);
	}
	else if (direction == Snake::Direction::down) {
		snakeDirection = 0.5;

		Point<std::size_t> testTarget = position;

		testTarget.x_ += 1;
		leftBlocked = simulation_.checkForCollision(board, testTarget);

		testTarget = position;
		testTarget.y_ -= 1;
		rightBlocked = simulation_.checkForCollision(board, testTarget);
	}

	float angle = utils::math::snakeAngleToFood(snake_.getFirstBodyPosition(), snake_.getHeadPosition(), food_.position());
	arma::mat vision(5, 1);
	vision(0, 0) = (double)leftBlocked;
	vision(1, 0) = (double)frontBlocked;
	vision(2, 0) = (double)rightBlocked;
	vision(3, 0) = (double)angle;
	vision(4, 0) = (double)snakeDirection;

	return vision;
}*/
