#include "RunningState.hpp"
#include "GameOverState.hpp"
#include "Game.hpp"
#include "FontCache.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

extern FontCache fontCache;
constexpr unsigned int secondAsMilliseconds{ 1000 };
constexpr unsigned int fontSize{ 20 };
constexpr SDL_Color black = { 0, 0, 0,255 };

RunningState::RunningState(Game& game) : State(game), snake_(game.board())
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

	snake_.init(initialPosition, Snake::Direction::right);
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

void RunningState::printCurrentScoreToScreen(Renderer& renderer)
{
	std::string score = "Score: ";
	score.append(std::to_string(snake_.length()));
	constexpr unsigned int x{ 0 };
	constexpr unsigned int y{ 40 };
	renderer.renderText(x, y, score, *fontCache.getFont(fontSize), black);
}

void RunningState::update(Renderer& renderer, uint32_t deltaTime)
{
	updateDeltaTime_ += deltaTime;
	const unsigned int snakeSpeed = snake_.getSpeed();

	if (updateDeltaTime_ > (secondAsMilliseconds / snakeSpeed)) {
		Board& board = game_.board();

		auto position = snake_.getPosition();
		SnakeMovement::Direction direction = snake_.getDirection();

		const Point<std::size_t> target = simulation_.getNextSnakePosition(position, direction);

		const bool collision = simulation_.checkForCollisionWithWall(board, target);

		if (collision) {
			game_.pushState<GameOverState>(game_);
			return;
		}

		const bool food = simulation_.checkForFood(board, target);

		if (food) {
			snake_.grow(1);
			newRandomPositionForFood();
		}

		simulation_.updateSnakePosition(snake_, target);
		updateDeltaTime_ = updateDeltaTime_ - (secondAsMilliseconds / snakeSpeed);
	}

	printCurrentScoreToScreen(renderer);
}

void RunningState::exit()
{
	std::cout << " RunningState::exit()" << std::endl;
}

void RunningState::handleInput(const Keyboard& keyboard)
{
	Snake::Direction direction{ snake_.getDirection() };

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

	snake_.setDirection(direction);
}


