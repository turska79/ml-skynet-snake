#include "RunningState.hpp"
#include "GameOverState.hpp"
#include "Game.hpp"
#include <iostream>
#include <string>

RunningState::RunningState(Game& game) : State(game)
{
}


RunningState::~RunningState()
{
}

void RunningState::enter()
{
	std::cout << " RunningState::enter()" << std::endl;
	resetBoard();
	initSnake();
	initFood();
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

	Board& board = game_.board();
	snake_.init(initialPosition, board);
	snake_.setDirection(Snake::Direction::right);
}

void RunningState::initFood()
{
	Board& board = game_.board();
	Point<std::size_t> position = board.findRandomEmptyCell();

	food_.init(board, position);
}

void RunningState::newRandomPositionForFood()
{
	Board& board = game_.board();
	Point<std::size_t> position = board.findRandomEmptyCell();

	food_.updatePosition(board, position);
}

void RunningState::update(Renderer & renderer, uint32_t deltaTime)
{
	constexpr unsigned int secondAsMilliseconds{ 1000 };
	updateDeltaTime_ += deltaTime;
	const unsigned int snakeSpeed = snake_.getSpeed();

	if (updateDeltaTime_ > (secondAsMilliseconds / snakeSpeed)) {
		Board& board = game_.board();
		const Point<std::size_t> target = simulation_.getNextSnakePosition(board, snake_);
		
		bool collision = simulation_.checkForCollision(board, target);

		if (collision) {
			game_.pushState<GameOverState>(game_);
			return;
		}

		bool food = simulation_.checkForFood(board, target);

		if (food) {
			snake_.grow(1);
			newRandomPositionForFood();
		}

		simulation_.updateSnakePosition(board, snake_, target);
		updateDeltaTime_ = updateDeltaTime_ - (secondAsMilliseconds / snakeSpeed);
	}
}

void RunningState::exit()
{
	std::cout << " RunningState::exit()" << std::endl;
}

void RunningState::handleInput(const Keyboard & keyboard)
{
	Snake::Direction direction{ snake_.getDirection() };

	if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_RIGHT) == keyboard::ButtonState::pressed && direction != Snake::Direction::left) {
		std::cout << "snake pressed: right" << std::endl;
		direction = Snake::Direction::right;
	}
	else  if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_LEFT) == keyboard::ButtonState::pressed && direction != Snake::Direction::right) {
		std::cout << "snake pressed: left" << std::endl;
		direction = Snake::Direction::left;
	}
	else  if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_UP) == keyboard::ButtonState::pressed && direction != Snake::Direction::down) {
		std::cout << "snake pressed: up" << std::endl;
		direction = Snake::Direction::up;
	}
	else if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_DOWN) == keyboard::ButtonState::pressed && direction != Snake::Direction::up) {
		std::cout << "snake pressed: down" << std::endl;
		direction = Snake::Direction::down;
	}

	snake_.setDirection(direction);
}


