#include "RunningStateAI.hpp"
#include "../utils/Utils.hpp"
#include "../Game.hpp"
#include "../Board.hpp"
#include "GameOverState.hpp"
//#include "Scheduler.hpp"
#include "../FontCache.hpp"
#include "../SnakeControl.hpp"
#include <iostream>
#include <iterator>

extern FontCache fontCache;
constexpr unsigned int fontSize{ 20 };
constexpr SDL_Color black = { 0, 0, 0,255 };

RunningStateAI::RunningStateAI(Game& game) : RunningState(game)
{
	mlpack::ann::FFN< mlpack::ann::MeanSquaredError<>, mlpack::ann::GaussianInitialization> model(mlpack::ann::MeanSquaredError<>(), mlpack::ann::GaussianInitialization(0, 0.001));
	model.Add< mlpack::ann::Linear<>>(24, 128);
	model.Add< mlpack::ann::ReLULayer<>>();
	model.Add< mlpack::ann::Linear<>>(128, 128);
	model.Add< mlpack::ann::ReLULayer<>>();
	model.Add< mlpack::ann::Linear<>>(128, 4);

	mlpack::rl::GreedyPolicy<SnakeBrain> policy(1.0, 1000, 0.1);
	mlpack::rl::RandomReplay<SnakeBrain> replayMethod(30, 1000);

	mlpack::rl::TrainingConfig config;
	config.StepSize() = 0.1;
	config.Discount() = 0.5;
	config.TargetNetworkSyncInterval() = 10;
	config.ExplorationSteps() = 10;
	config.DoubleQLearning() = false;
	config.StepLimit() = 500;

	learningAgent_ = new mlpack::rl::QLearning<SnakeBrain, decltype(model), ens::AdamUpdate, decltype(policy)>(std::move(config), std::move(model), std::move(policy), std::move(replayMethod));
	
	auto& snake{ game.snake() };
	auto& brain = snakeBrain();
	brain.setBoard(static_cast<Board*>(&game_.board()));
	brain.setRenderer(static_cast<Renderer*>(&game_.renderer()));
	brain.setSimulation(static_cast<Simulation*>(&simulation_));
	brain.setGame(&game_);
	brain.setSnake(&snake);
	brain.setSnakeMovementInterface(&snakeControl_);

}

void RunningStateAI::enter()
{
	++gameCount_;
	RunningState::enter();
}

void RunningStateAI::update(Renderer& renderer)
{
	if (!running_) {
		running_ = true;

		std::thread worker([this] {
			learningAgent_->Episode();
		});

		worker.detach(); // forget about this thread, let it do it's job
	}

	Board& board = game_.board();

	auto position = snakeControl_.getPosition();
	SnakeControl::Direction direction = snakeControl_.getDirection();

	/*
	const Point<std::size_t> target = simulation_.getNextSnakePosition(position, direction);

	const bool collision = simulation_.checkForCollisionWithWall(target);

	if (collision) {
		enter();
		return;
	}

	const bool food = simulation_.checkForCollisionWithFood(target);

	if (food) {
		snakeControl_.grow(1);
		newRandomPositionForFood();
	}*/
	auto& brain{ snakeBrain() };

	SnakeVision& snakeVision{ brain.snakeVision() };

	const std::list< VisionPoints> points = snakeVision.pointsForRendering(board, position, simulation_);
	
	for (const auto& it : points) {
		const auto fromPosition = it.first;
		const auto toPosition = it.second;

		renderer.DrawDottedLine(fromPosition.x_, fromPosition.y_, toPosition.x_, toPosition.y_);
	}

	//printCurrentScoreToScreen(renderer);
	printStepsToScreen(renderer);
	printGameCountToScreen(renderer);
}

void RunningStateAI::handleInput(const Keyboard& keyboard)
{

}

void RunningStateAI::printStepsToScreen(Renderer& renderer)
{
	auto& brain = snakeBrain();
	
	unsigned int step = static_cast<unsigned int>(brain.StepsPerformed());
	unsigned int maxSteps = static_cast<unsigned int>(brain.MaxSteps());

	std::string score = "Step: ";
	score.append(std::to_string(step));
	score.append(" / ");
	score.append(std::to_string(maxSteps));
	constexpr unsigned int x{ 0 };
	constexpr unsigned int y{ 60 };
	renderer.renderText(x, y, score, *fontCache.getFont(fontSize), black);
}

void RunningStateAI::printGameCountToScreen(Renderer& renderer)
{
	std::string score = "Game: ";
	score.append(std::to_string(gameCount_));
	
	constexpr unsigned int x{ 0 };
	constexpr unsigned int y{ 80 };
	renderer.renderText(x, y, score, *fontCache.getFont(fontSize), black);

}
SnakeBrain& RunningStateAI::snakeBrain() noexcept
{
	return learningAgent_->Environment();
}
