#include "RunningState.hpp"
#include "GameOverState.hpp"
#include "../Game.hpp"
#include "../FontCache.hpp"
#include "../Simulation.hpp"
//#include "Snake.hpp"
#include "../SnakeControl.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

extern FontCache fontCache;

RunningState::RunningState(Game& game) : State(game), snakeControl_(game.snake()), simulation_(game.simulation())
{

}

void RunningState::snakeCollisionCallback()
{
	if (game_.currentState() != this) {
		return;
	}

	Simulation& simulation = game_.simulation();
	simulation.stop();

	game_.pushState<GameOverState>(game_);
}

void RunningState::enter()
{
	resetBoard();
	initSnake();
	newRandomPositionForFood();
	registerCallbacks();
	simulation_.start();
}

void RunningState::resetBoard() const
{
	Board& board = game_.board();
	board.resetBoard();
}

void RunningState::initSnake()
{
	utils::Point<std::size_t> initialPosition{ 10, 10 };

	snakeControl_.init(initialPosition, SnakeControl::Direction::right);
}

void RunningState::newRandomPositionForFood()
{
	Board& board = game_.board();
	const utils::Point<std::size_t> position = board.findRandomEmptyCell();

	food_.updatePosition(board, position);
}

void RunningState::registerCallbacks()
{
	registerCollisionCallback();
	registerFoodEatenCallback();
}

void RunningState::unregisterCallbacks()
{
	unregisterCollisionCallback();
	unregisterFoodEatenCallback();
}

void RunningState::registerCollisionCallback()
{
	Snake& snake = game_.snake();
	subjects::SnakeCollisionSubject& collisionSubject = snake.snakeCollisionSubject();
	collisionSubject.addObserver(this, &RunningState::snakeCollisionCallback);
}

void RunningState::unregisterCollisionCallback()
{
	Snake& snake = game_.snake();
	subjects::SnakeCollisionSubject& collisionSubject = snake.snakeCollisionSubject();
	collisionSubject.removeObserver(this, &RunningState::snakeCollisionCallback);
}

void RunningState::registerFoodEatenCallback()
{
	Snake& snake = game_.snake();
	subjects::FoodEatenSubject& foodEatenSubject = snake.foodEatenSubject();
	foodEatenSubject.addObserver(this, &RunningState::newRandomPositionForFood);
}

void RunningState::unregisterFoodEatenCallback()
{
	Snake& snake = game_.snake();
	subjects::FoodEatenSubject& foodEatenSubject = snake.foodEatenSubject();
	foodEatenSubject.removeObserver(this, &RunningState::newRandomPositionForFood);
}

void RunningState::update(Renderer& renderer)
{
}

void RunningState::exit()
{
	unregisterCallbacks();
}

void RunningState::handleInput(const Keyboard& keyboard)
{
	SnakeControl::Direction direction{ snakeControl_.getDirection() };

	if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_RIGHT) == keyboard::ButtonState::pressed && direction != Snake::Direction::left) {
		std::cout << "snake pressed: right" << std::endl;
		direction = Snake::Direction::right;
	} else  if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_LEFT) == keyboard::ButtonState::pressed && direction != Snake::Direction::right) {
		std::cout << "snake pressed: left" << std::endl;
		direction = Snake::Direction::left;
	} else  if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_UP) == keyboard::ButtonState::pressed && direction != Snake::Direction::down) {
		std::cout << "snake pressed: up" << std::endl;
		direction = Snake::Direction::up;
	} else if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_DOWN) == keyboard::ButtonState::pressed && direction != Snake::Direction::up) {
		std::cout << "snake pressed: down" << std::endl;
		direction = Snake::Direction::down;
	}

	snakeControl_.setDirection(direction);
}


