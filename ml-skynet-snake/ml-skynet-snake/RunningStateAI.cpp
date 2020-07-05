#include "RunningStateAI.h"
#include "Utils.hpp"
#include "Game.hpp"
#include "Board.hpp"
#include "GameOverState.hpp"
#include "Scheduler.hpp"
#include "FontCache.hpp"
#include <iostream>
#include <iterator>

extern FontCache fontCache;
constexpr unsigned int fontSize{ 20 };
constexpr SDL_Color black = { 0, 0, 0,255 };

RunningStateAI::RunningStateAI(Game& game) : RunningState(game)
{
	/*
	auto& brain = snake_.brain();
	brain.setBoard(static_cast<Board*>(&game_.board()));
	brain.setRenderer(static_cast<Renderer*>(&game_.renderer()));
	brain.setSimulation(static_cast<Simulation*>(&simulation_));
	brain.setGame(&game_);
	brain.setSnake(&snake_);
	*/
}

void RunningStateAI::enter()
{
	/*
	++gameCount_;
	RunningState::enter();
	
	std::thread worker([this] {
		snake_.runLearningAgentForSingleGame();
	});

	worker.detach(); // forget about this thread, let it do it's job
	*/
}

void RunningStateAI::update(Renderer& renderer)
{
	/*
	Board& board = game_.board();
	const bool collision = simulation_.collision();

	if (collision) {
		enter();
		return;
	}

	auto position = snake_.getPosition();
	SnakeMovement::Direction direction = snake_.getDirection();

	const Point<std::size_t> target = simulation_.getNextSnakePosition(position, direction);
	const bool food = simulation_.checkForCollisionWithFood(target);

	if (food) {
		snake_.grow(1);
		newRandomPositionForFood();
	}

	SnakeVision& snakeVision{ snake_.brain().snakeVision() };

	const std::list< VisionPoints> points = snakeVision.pointsForRendering(board, position, simulation_);
	
	for (const auto& it : points) {
		const auto fromPosition = it.first;
		const auto toPosition = it.second;

		renderer.DrawDottedLine(fromPosition.x_, fromPosition.y_, toPosition.x_, toPosition.y_);
	}

	printCurrentScoreToScreen(renderer);
	printStepsToScreen(renderer);
	printGameCountToScreen(renderer);
	*/
}

void RunningStateAI::handleInput(const Keyboard& keyboard)
{

}

void RunningStateAI::printStepsToScreen(Renderer& renderer)
{
	/*
	auto& brain = snake_.brain();
	
	unsigned int step = static_cast<unsigned int>(brain.StepsPerformed());
	unsigned int maxSteps = static_cast<unsigned int>(brain.MaxSteps());

	std::string score = "Step: ";
	score.append(std::to_string(step));
	score.append(" / ");
	score.append(std::to_string(maxSteps));
	constexpr unsigned int x{ 0 };
	constexpr unsigned int y{ 60 };
	renderer.renderText(x, y, score, *fontCache.getFont(fontSize), black);
	*/
}

void RunningStateAI::printGameCountToScreen(Renderer& renderer)
{
	/*
	std::string score = "Game: ";
	score.append(std::to_string(gameCount_));
	
	constexpr unsigned int x{ 0 };
	constexpr unsigned int y{ 80 };
	renderer.renderText(x, y, score, *fontCache.getFont(fontSize), black);
	*/
}