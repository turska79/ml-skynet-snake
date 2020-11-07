
#include "LearningAgent.hpp"
#include "../SnakeControl.hpp"
#include "../Board.hpp"

#pragma warning(push)  
#pragma warning(disable : 26819 26495 4244 26451 6011 26439 26812 4458 4267 4801 4081)
#include <mlpack/methods/reinforcement_learning/policy/greedy_policy.hpp>
#include <mlpack/methods/reinforcement_learning/training_config.hpp>
#pragma warning (pop)

#include "DQN.hpp"

namespace ml {
	constexpr double initialEpsilon{ 1.0 };
	constexpr size_t annealInterval{ 1000 };
	constexpr double minEpsilon{ 0.1 };

	constexpr size_t batchSize{ 30 };
	constexpr size_t capacity{ 1000 };
}

ml::LearningAgent::LearningAgent(SnakeControl& snakeControl, Board& board)
{
	trainingConfig_ = std::make_unique<Config>();
	mlpack::rl::TrainingConfig& config{ *(trainingConfig_.get()) };
	config.StepSize() = 0.1;
	config.Discount() = 0.5;
	config.TargetNetworkSyncInterval() = 10;
	config.ExplorationSteps() = 10;
	config.DoubleQLearning() = false;
	config.StepLimit() = 500;
	
	model_ = std::make_unique<ml::DQN>();
	replay_ = std::make_unique<ml::Replay>(ml::batchSize, ml::capacity);
	policy_ = std::make_unique<ml::Policy>(ml::initialEpsilon, ml::annealInterval, ml::minEpsilon);
	auto& network{ model_.get()->model() };
	auto& policy{ *(policy_.get()) };
	auto& replayMethod{ *(replay_.get()) };

	learningAgent_ = std::make_unique<QLearningAgent>(config, network, policy, replayMethod);

	ml::SnakeAction& snakeAction = learningAgent_->Environment();
	snakeAction.setSnakeControl(&snakeControl);
	snakeAction.setBoard(&board);

	control_ = &snakeControl;
	board_ = &board;
}

ml::LearningAgent::~LearningAgent()
{
}

void ml::LearningAgent::runLearningAgent()
{
	episodeRunning_ = true;
	learningAgent_->Episode();
	episodeRunning_ = false;
}

void ml::LearningAgent::proceedToNextStep()
{
	if (episodeRunning_ == false) {
		return;
	}
	//std::cout << "ml::LearningAgent::proceedToNextStep()" << std::endl;
	ml::SnakeAction& snakeAction = learningAgent_->Environment();
	snakeAction.proceedToNextStep();
}

std::list<VisionPoints> ml::LearningAgent::currentVision() const
{
	ml::SnakeAction& snakeAction{ learningAgent_->Environment() };
	SnakeVision& snakeVision{ snakeAction.snakeVision() };

	return snakeVision.pointsForRendering(*board_, control_->getPosition());
}

size_t ml::LearningAgent::stepsPerformed() const
{
	ml::SnakeAction& snakeAction = learningAgent_->Environment();
	return snakeAction.StepsPerformed();
}

size_t ml::LearningAgent::maxSteps() const
{
	ml::SnakeAction& snakeAction = learningAgent_->Environment();
	return snakeAction.MaxSteps();
}

size_t ml::LearningAgent::totalSteps() const
{
	return learningAgent_->TotalSteps();
}
