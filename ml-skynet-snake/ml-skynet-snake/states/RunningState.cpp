#include "RunningState.hpp"
#include "GameOverState.hpp"
#include "../Game.hpp"
#include "../utils/FontCache.hpp"
#include "../simulation/Simulation.hpp"
#include "../SnakeControl.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

extern FontUtils::FontCache fontCache;

gamestates::state::RunningState::RunningState(Game& game) noexcept : gamestates::state::BaseState(game), snakeControl_(game.snake()), simulation_(game.simulation())
{

}

void gamestates::state::RunningState::snakeCollisionCallback()
{
	if (game_.currentState() != this) {
		return;
	}

	std::cout << " RunningState::snakeCollisionCallback()" << std::endl;

	//Simulation& simulation = game_.simulation();
	//simulation.stop();

	game_.nextState<GameOverState>(game_);
}

void gamestates::state::RunningState::enter()
{
	std::cout << " RunningState::enter()" << std::endl;
	resetBoard();
	initSnake();
	newRandomPositionForFood();
	registerCallbacks();
	simulation_.start();
}

void gamestates::state::RunningState::resetBoard() const
{
	Board& board = game_.board();
	board.resetBoard();
}

void gamestates::state::RunningState::initSnake()
{
	utils::Point<std::size_t> initialPosition{ 10, 10 };

	snakeControl_.init(initialPosition, SnakeControl::Direction::right);
}

void gamestates::state::RunningState::newRandomPositionForFood()
{
	Board& board = game_.board();
	const utils::Point<std::size_t> position = board.findRandomEmptyCell();

	food_.updatePosition(board, position);
}

void gamestates::state::RunningState::snakePositionUpdated()
{
}

void gamestates::state::RunningState::registerCallbacks()
{
	registerCollisionCallback();
	registerFoodEatenCallback();
	registerPositionUpdatedCallback();
}

void gamestates::state::RunningState::unregisterCallbacks()
{
	unregisterCollisionCallback();
	unregisterFoodEatenCallback();
	unregisterPositionUpdatedCallback();
}

void gamestates::state::RunningState::registerCollisionCallback()
{
	Snake& snake = game_.snake();
	subjects::SnakeCollisionSubject& collisionSubject = snake.snakeCollisionSubject();
	collisionSubject.addObserver(this, &RunningState::snakeCollisionCallback);
}

void gamestates::state::RunningState::unregisterCollisionCallback()
{
	Snake& snake = game_.snake();
	subjects::SnakeCollisionSubject& collisionSubject = snake.snakeCollisionSubject();
	collisionSubject.removeObserver(this, &RunningState::snakeCollisionCallback);
}

void gamestates::state::RunningState::registerFoodEatenCallback()
{
	Snake& snake = game_.snake();
	subjects::FoodEatenSubject& foodEatenSubject = snake.foodEatenSubject();
	foodEatenSubject.addObserver(this, &RunningState::newRandomPositionForFood);
}

void gamestates::state::RunningState::unregisterFoodEatenCallback()
{
	Snake& snake = game_.snake();
	subjects::FoodEatenSubject& foodEatenSubject = snake.foodEatenSubject();
	foodEatenSubject.removeObserver(this, &RunningState::newRandomPositionForFood);
}

void gamestates::state::RunningState::registerPositionUpdatedCallback()
{
	Snake& snake = game_.snake();
	subjects::SnakePositionUpdatedSubject& positionUpdated = snake.positionUpdateSubject();
	positionUpdated.addObserver(this, &RunningState::snakePositionUpdated);
}

void gamestates::state::RunningState::unregisterPositionUpdatedCallback()
{
	Snake& snake = game_.snake();
	subjects::SnakePositionUpdatedSubject& positionUpdated = snake.positionUpdateSubject();
	positionUpdated.removeObserver(this, &RunningState::snakePositionUpdated);
}

void gamestates::state::RunningState::update(Renderer& renderer)
{
	/*if (collision) {
		game_.nextState<GameOverState>(game_);
	}*/
}

void gamestates::state::RunningState::exit()
{
	std::cout << " RunningState::exit()" << std::endl;
	Simulation& simulation = game_.simulation();
	simulation.stop();
	
	unregisterCallbacks();
}

void gamestates::state::RunningState::handleInput(const Keyboard& keyboard)
{
	SnakeControl::Direction direction{ snakeControl_.getDirection() };

	if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_RIGHT) == keyboard::ButtonState::pressed && direction != Snake::Direction::left) {
		direction = Snake::Direction::right;
	} else  if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_LEFT) == keyboard::ButtonState::pressed && direction != Snake::Direction::right) {
		direction = Snake::Direction::left;
	} else  if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_UP) == keyboard::ButtonState::pressed && direction != Snake::Direction::down) {
		direction = Snake::Direction::up;
	} else if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_DOWN) == keyboard::ButtonState::pressed && direction != Snake::Direction::up) {
		direction = Snake::Direction::down;
	}

	snakeControl_.setDirection(direction);
}


