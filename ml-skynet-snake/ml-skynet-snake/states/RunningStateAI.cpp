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
	learningAgent_ = std::make_unique<ml::LearningAgent>(snakeControl, board, game_);
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

	//if (running_) {
		runLearningAgent();
		//ai_ = new thread::interruptibleThread(&RunningStateAI::runLearningAgent, this);
	//	running_ = false;
	//}
}

void gamestates::state::RunningStateAI::runLearningAgent()
{
	std::cout << "RunningStateAI::runLearningAgent()" << std::endl;
	//learningAgent_->runLearningAgent();
	learningAgent_->run();
}

void gamestates::state::RunningStateAI::exit()
{
	std::cout << "RunningStateAI::exit()" << std::endl;
	//running_ = false;

	//if (ai_) {
	//	ai_->waitUntilInterrupted();
	//}


	Simulation& simulation = game_.simulation();
	simulation.stop();

	learningAgent_->waitUntilStopped();

	RunningState::exit();

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

	printGameCountToScreen(renderer);
	printStepsToScreen(renderer);
}

void gamestates::state::RunningStateAI::handleInput(const Keyboard&)
{

}

void gamestates::state::RunningStateAI::snakeCollisionCallback()
{
	if (game_.currentState() != this) {
		return;
	}

	std::cout << "RunningStateAI::snakeCollisionCallback()" << std::endl;

	//ai_->waitUntilInterrupted();

	//Simulation& simulation = game_.simulation();
	//simulation.stop();

	//learningAgent_->waitUntilStopped();
	
	game_.nextState<GameOverState>(game_);
}

void gamestates::state::RunningStateAI::printStepsToScreen(Renderer& renderer)
{
	auto step{ learningAgent_->stepsPerformed() };
	auto maxSteps{ learningAgent_->maxSteps() };
	auto totalSteps{ learningAgent_->totalSteps() };
	std::string totalStepsText = "Total steps: ";
	totalStepsText.append(std::to_string(totalSteps));
	std::string episodeSteps ="Episode step: ";
	episodeSteps.append(std::to_string(step));
	episodeSteps.append(" / ");
	episodeSteps.append(std::to_string(maxSteps));
	constexpr unsigned int x{ 0 };
	constexpr unsigned int y{ 80 };
	renderer.renderText(x, y, totalStepsText, *fontCache.getFont(utils::commonConstants::fontSize::twenty), utils::commonConstants::color::black);
	renderer.renderText(x, y + 20, episodeSteps, *fontCache.getFont(utils::commonConstants::fontSize::twenty), utils::commonConstants::color::black);
	
}

void gamestates::state::RunningStateAI::printGameCountToScreen(Renderer& renderer)
{
	std::string score = "Game: ";
	score.append(std::to_string(gameCount_));
	
	constexpr unsigned int x{ 0 };
	constexpr unsigned int y{ 60 };
	renderer.renderText(x, y, score, *fontCache.getFont(utils::commonConstants::fontSize::twenty), utils::commonConstants::color::black);
}

void gamestates::state::RunningStateAI::snakePositionUpdated()
{
	std::cout << "RunningStateAI::snakePositionUpdated()" << std::endl;
	learningAgent_->advanceEnvironment();
	/*
	//learningAgent_->processNextStep();
	try {
		std::mutex mutex;
		std::condition_variable cv;
		std::unique_lock<std::mutex> lock(mutex);
		//auto function = std::bind(&LearningAgent::isIdle, this);
		auto function = [&]() -> bool { return learningAgent_->learningAgentState() == ml::LearningAgent::LearningAgentState::Idle; };

		thread::utils::interruptibleWait<decltype(function)>(cv, lock, function);

		auto& agentSate{ learningAgent_->learningAgentState() };
		agentSate = ml::LearningAgent::LearningAgentState::Process;
	}
	catch (const std::exception&) {
		std::cout << "LearningAgent::runLearningAgent() exception from processNextStep" << std::endl;
	}
	*/
}
