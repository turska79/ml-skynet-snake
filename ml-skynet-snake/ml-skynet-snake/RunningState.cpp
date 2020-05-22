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
	Board& board = game_.board();
	board.resetBoard();
	
	Point<std::size_t> initialPosition;
	initialPosition.x_ = 10;
	initialPosition.y_ = 10;

	snake_.init(initialPosition, board);
}

void RunningState::update(Renderer & renderer, uint32_t deltaTime)
{
	constexpr unsigned int secondAsMilliseconds{ 1000 };
	updateDeltaTime += deltaTime;
	const unsigned int snakeSpeed = snake_.getSpeed();

	if (updateDeltaTime > (secondAsMilliseconds / snakeSpeed)) {
		std::cout << "updateDeltaTime: " << std::to_string(updateDeltaTime) << std::endl;
		Board& board = game_.board();
		const Point<std::size_t> target = simulation_.getNextSnakePosition(board, snake_);
		
		bool collision = simulation_.checkForCollision(board, target);

		if (collision) {
			game_.pushState<GameOverState>(game_);
			return;
		}

		simulation_.updateSnakePosition(board, snake_, target);
		updateDeltaTime = updateDeltaTime - (secondAsMilliseconds / snakeSpeed);
	}
}

void RunningState::exit()
{

}

void RunningState::handleInput(const Keyboard & keyboard)
{
	Snake::Direction direction{ snake_.getDirection() };

	if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_RIGHT) == keyboard::ButtonState::pressed) {
		std::cout << "snake pressed: right" << std::endl;
		direction = Snake::Direction::right;
	}
	else  if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_LEFT) == keyboard::ButtonState::pressed) {
		std::cout << "snake pressed: left" << std::endl;
		direction = Snake::Direction::left;
	}
	else  if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_UP) == keyboard::ButtonState::pressed) {
		std::cout << "snake pressed: up" << std::endl;
		direction = Snake::Direction::up;
	}
	else if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_DOWN) == keyboard::ButtonState::pressed) {
		std::cout << "snake pressed: down" << std::endl;
		direction = Snake::Direction::down;
	}

	snake_.setDirection(direction);
}
