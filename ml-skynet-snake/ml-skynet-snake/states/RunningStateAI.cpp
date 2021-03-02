#include "RunningStateAI.hpp"
#include "../Game.hpp"
#include "../Board.hpp"
#include "GameOverState.hpp"
#include "../utils/FontCache.hpp"
#include "../SnakeControl.hpp"
#include "../utils/InterruptibleThread.hpp"
#include "../ml/LearningAgent.hpp"
#include <iostream>

extern FontUtils::FontCache fontCache;

gamestates::state::RunningStateAI::RunningStateAI(Game& game) noexcept : RunningState(game)
{
	SnakeControl& snakeControl{ game.snake() };
	Board& board{ game.board() };
	learningAgent_ = std::make_unique<ml::LearningAgent>(snakeControl, board);
}

gamestates::state::RunningStateAI::~RunningStateAI()
{

}

void gamestates::state::RunningStateAI::enter()
{
	std::cout << "RunningStateAI::enter()" << std::endl;
	running_ = true;
	++gameCount_;
	RunningState::enter();
	registerSnakePositionUpdatedCallback();

	if (running_) {
		ai_ = new thread::interruptibleThread(&RunningStateAI::runLearningAgent, this);
		running_ = false;
	}
}

void gamestates::state::RunningStateAI::runLearningAgent()
{
	learningAgent_->runLearningAgent();
}

void gamestates::state::RunningStateAI::exit()
{
	running_ = false;

	if (ai_) {
		ai_->interrupt();
	}

	RunningState::exit();
	unregisterSnakePositionUpdatedCallback();

	std::cout << "RunningStateAI::exit()" << std::endl;
}

void gamestates::state::RunningStateAI::update(Renderer& renderer)
{
	RunningState::update(renderer);

	auto position = snakeControl_.getPosition();
	const std::list< VisionPoints> points = learningAgent_->currentVision();

	
	for (const auto& it : points) {
		const auto fromPosition = it.first;
		const auto toPosition = it.second;

		renderer.DrawDottedLine(fromPosition.x_, fromPosition.y_, toPosition.x_, toPosition.y_);
	}

	printStepsToScreen(renderer);
	printGameCountToScreen(renderer);
	
}

void gamestates::state::RunningStateAI::handleInput(const Keyboard& keyboard)
{

}

void gamestates::state::RunningStateAI::printStepsToScreen(Renderer& renderer)
{
	auto step{ learningAgent_->stepsPerformed() };
	auto maxSteps{ learningAgent_->maxSteps() };
	auto totalSteps{ learningAgent_->totalSteps() };
	std::string score = "Total steps: ";
	score.append(std::to_string(totalSteps));
	score.append(" Episode step: ");
	score.append(std::to_string(step));
	score.append(" / ");
	score.append(std::to_string(maxSteps));
	constexpr unsigned int x{ 0 };
	constexpr unsigned int y{ 60 };
	renderer.renderText(x, y, score, *fontCache.getFont(utils::commonConstants::fontSize::twenty), utils::commonConstants::color::black);
	
}

void gamestates::state::RunningStateAI::printGameCountToScreen(Renderer& renderer)
{
	std::string score = "Game: ";
	score.append(std::to_string(gameCount_));
	
	constexpr unsigned int x{ 0 };
	constexpr unsigned int y{ 80 };
	renderer.renderText(x, y, score, *fontCache.getFont(utils::commonConstants::fontSize::twenty), utils::commonConstants::color::black);
}
void gamestates::state::RunningStateAI::registerSnakePositionUpdatedCallback()
{
/*	Snake& snake = game_.snake();
	subjects::SnakePositionUpdatedSubject& positionUpdated = snake.positionUpdateSubject();
	positionUpdated.addObserver(this, &RunningStateAI::snakePositionUpdated);*/
}
void gamestates::state::RunningStateAI::unregisterSnakePositionUpdatedCallback()
{
	/*Snake& snake = game_.snake();
	subjects::SnakePositionUpdatedSubject& positionUpdated = snake.positionUpdateSubject();
	positionUpdated.removeObserver(this, &RunningStateAI::snakePositionUpdated);*/
}
void gamestates::state::RunningStateAI::snakePositionUpdated()
{
	learningAgent_->proceedToNextStep();
}
/*
SnakeBrain& RunningStateAI::snakeBrain() noexcept
{
	return learningAgent_->Environment();
}*/
