#include "RunningState.hpp"
#include "GameOverState.hpp"
#include "Game.hpp"
#include "FontCache.hpp"
#include "Simulation.hpp"
//#include "Snake.hpp"
#include "SnakeMovement.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

extern FontCache fontCache;
constexpr unsigned int fontSize{ 20 };
constexpr SDL_Color black = { 0, 0, 0,255 };

RunningState::RunningState(Game& game) : State(game), snakeMovement_(game.snake()), simulation_(game.simulation())
{

}

void RunningState::enter()
{
	resetBoard();
	initSnake();
	initFood();
	simulation_.start();
}

void RunningState::resetBoard() const
{
	Board& board = game_.board();
	board.resetBoard();
}

void RunningState::initSnake()
{
	Point<std::size_t> initialPosition;
	initialPosition.x_ = 10;
	initialPosition.y_ = 10;

	snakeMovement_.init(initialPosition, Snake::Direction::right);
}

void RunningState::initFood()
{
	Board& board = game_.board();
	const Point<std::size_t> position = board.findRandomEmptyCell();

	food_.init(board, position);
}

void RunningState::newRandomPositionForFood()
{
	Board& board = game_.board();
	const Point<std::size_t> position = board.findRandomEmptyCell();

	food_.updatePosition(board, position);
}

void RunningState::update(Renderer& renderer)
{
	Board& board = game_.board();
	const bool collision = simulation_.collision();

	if (collision) {
		game_.pushState<GameOverState>(game_);
		return;
	}

	auto position = snakeMovement_.getPosition();
	SnakeMovement::Direction direction = snakeMovement_.getDirection();

	const Point<std::size_t> target = simulation_.getNextSnakePosition(position, direction);
	const bool food = simulation_.checkForCollisionWithFood(target);

	if (food) {
		snakeMovement_.grow(1);
		newRandomPositionForFood();
	}

	//printCurrentScoreToScreen(renderer);
}

void RunningState::exit()
{
}

void RunningState::handleInput(const Keyboard& keyboard)
{
	Snake::Direction direction{ snakeMovement_.getDirection() };

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

	snakeMovement_.setDirection(direction);
}


