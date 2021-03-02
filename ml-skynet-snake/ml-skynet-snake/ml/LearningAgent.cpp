
#include "LearningAgent.hpp"
#include "../SnakeControl.hpp"
#include "../Board.hpp"
#include <mutex>
#include <condition_variable>
#include "../utils/InterruptibleThread.hpp"
#include "../utils/Utils.hpp"

#pragma warning(push)  
#pragma warning(disable : 26819 26495 4244 26451 6011 26439 26812 4458 4267 4801 4081)
//#include <mlpack/methods/reinforcement_learning/policy/greedy_policy.hpp>
//#include <mlpack/methods/reinforcement_learning/training_config.hpp>
//#include <mlpack/methods/reinforcement_learning/sac.hpp>
//#include <mlpack/methods/ann/loss_functions/empty_loss.hpp>
//#include <mlpack/methods/ann/ffn.hpp>
//#include <mlpack/core/math/random.hpp>
#pragma warning (pop)

namespace ml {
	constexpr double initialEpsilon{ 1.0 };
	constexpr size_t annealInterval{ 1000 };
	constexpr double minEpsilon{ 0.1 };

	constexpr size_t batchSize{ 32 };
	constexpr size_t capacity{ 10000 };
}

ml::LearningAgent::LearningAgent(SnakeControl& snakeControl, Board& board) : snakeControl_(snakeControl), board_(board)
{
	replayMethod_ = std::make_unique<ml::ReplayMethod>(ml::batchSize, ml::capacity);
	auto& replayMethod{ *(replayMethod_.get()) };

	policyNetwork_ = std::make_unique<ml::FeedForwardNetwork>(mlpack::ann::EmptyLoss<>(), mlpack::ann::GaussianInitialization(0, 0.01));

	policyNetwork_->Add(new  mlpack::ann::Linear<>(utils::commonConstants::ml::input_parameters, 128));
	policyNetwork_->Add(new  mlpack::ann::ReLULayer<>());
	policyNetwork_->Add(new  mlpack::ann::Linear<>(128, 128));
	policyNetwork_->Add(new  mlpack::ann::ReLULayer<>());
	policyNetwork_->Add(new  mlpack::ann::Linear<>(128, utils::commonConstants::ml::actions));
	policyNetwork_->Add(new  mlpack::ann::TanHLayer<>());
	policyNetwork_->ResetParameters();
	FeedForwardNetwork& policyNetwork = *(policyNetwork_.get());

	qnetwork_ = std::make_unique<ml::FeedForwardNetwork>(mlpack::ann::EmptyLoss<>(), mlpack::ann::GaussianInitialization(0, 0.01));
	qnetwork_->Add(new mlpack::ann::Linear<>(utils::commonConstants::ml::input_parameters + utils::commonConstants::ml::actions, 128));
	qnetwork_->Add(new mlpack::ann::ReLULayer<>());
	qnetwork_->Add(new mlpack::ann::Linear<>(128, 128));
	qnetwork_->Add(new mlpack::ann::ReLULayer<>());
	qnetwork_->Add(new mlpack::ann::Linear<>(128, 1));
	qnetwork_->ResetParameters();
	FeedForwardNetwork& qNetwork = *(qnetwork_.get());
		
	trainingConfig_ = std::make_unique<Config>();
	mlpack::rl::TrainingConfig& config{ *(trainingConfig_.get()) };

	config.ExplorationSteps() = 3200;
	config.TargetNetworkSyncInterval() = 1;
	config.UpdateInterval() = 1;

	agent_ = std::make_unique<SoftActionCritic>(config, qNetwork, policyNetwork, replayMethod);
}

ml::LearningAgent::~LearningAgent()
{
}

void ml::LearningAgent::runLearningAgent()
{
	episodeRunning_ = true;

	std::condition_variable cv;
	std::mutex mutex;

	step_ = 0;
	double totalReturn{ 0.0 };
	double episodeReturn{ 0.0 };

	updateEnvironment();
	
	while (episodeRunning_) {
		std::cout << "LearningAgent::runLearningAgent() thinking about next move" << std::endl;
		
		agent_->State().Data() = environment_.Encode();
		agent_->SelectAction();
		arma::mat action = { agent_->Action().action };

		unsigned int maxIndex{ 0 };
		double max{ 0 };
		
		auto cols{ action.n_rows };
		
		for (unsigned int i = 0; i < cols; ++i) {
			if (action[i] > max) {
				max = action[i];
				maxIndex = i;
			}
			//std::cout << "LearningAgent::runLearningAgent() action: " << std::to_string(action[i]) << std::endl;
		}

		switch (maxIndex) {
		case 0:
			snakeControl_.setDirection(SnakeControl::Direction::up);
			std::cout << "LearningAgent::runLearningAgent() action: up" << std::endl;
			break;
		case 1:
			snakeControl_.setDirection(SnakeControl::Direction::right);
			std::cout << "LearningAgent::runLearningAgent() action: right" << std::endl;
			break;
		case 2:
			snakeControl_.setDirection(SnakeControl::Direction::down);
			std::cout << "LearningAgent::runLearningAgent() action: down" << std::endl;
			break;
		case 3:
			snakeControl_.setDirection(SnakeControl::Direction::left);
			std::cout << "LearningAgent::runLearningAgent() action: left" << std::endl;
			break;
		}

		auto nextPosition = snakeControl_.getNextPosition();

		if (board_.isFood(nextPosition)) {
			environment_.reward_ = 100;
		} else if (board_.isWall(nextPosition)) {// || board_.isSnakeBody(nextPosition)) {
			environment_.reward_ = -100;
			environment_.terminal_ = true;
			episodeRunning_ = false;
		} else {
			environment_.reward_ = 1;
		}

		readyForNextStep_ = false;

		try {
			std::unique_lock<std::mutex> lock(mutex);
			auto function = std::bind(&LearningAgent::isReadyForNextStep, this);

			thread::utils::interruptibleWait<decltype(function)>(cv, lock, function);
		}
		catch (const std::exception& e) {
			episodeRunning_ = false;
			continue;
		}

		updateEnvironment();

		ContinuousActionEnvironment::State nextState;
		nextState.Data() = environment_.Encode();

		replayMethod_->Store(agent_->State(), agent_->Action(), environment_.reward_, nextState, environment_.terminal_, 0.99);
		episodeReturn += environment_.reward_;
		agent_->TotalSteps()++;
		step_++;
	}



	/*
	// Running until get to the terminal state.
	while (!environment.IsTerminal(state))
	{
		if (config.StepLimit() && steps >= config.StepLimit())
			break;
		SelectAction();

		// Interact with the environment to advance to next state.
		StateType nextState;
		double reward = environment.Sample(state, action, nextState);

		totalReturn += reward;
		steps++;
		totalSteps++;

		// Store the transition for replay.
		replayMethod.Store(state, action, reward, nextState,
			environment.IsTerminal(nextState), config.Discount());

		// Update current state.
		state = nextState;

		if (deterministic || totalSteps < config.ExplorationSteps())
			continue;
		for (size_t i = 0; i < config.UpdateInterval(); i++)
			Update();
	}
	//return totalReturn;
	*/
	episodeRunning_ = false;
}

void ml::LearningAgent::proceedToNextStep()
{
	if (episodeRunning_ == false) {
		return;
	}
	std::cout << "ml::LearningAgent::proceedToNextStep()" << std::endl;
	readyForNextStep_ = true;
	//ml::ContinuousActionEnvironment& continuousActionEnvironment = agent_->Environment();
	//continuousActionEnvironment.proceedToNextStep();
}

std::list<VisionPoints> ml::LearningAgent::currentVision()
{
	//ml::ContinuousActionEnvironment& continuousActionEnvironment{ agent_->Environment() };
	//SnakeVision& snakeVision{ continuousActionEnvironment.snakeVision() };

	return snakeVision_.pointsForRendering(board_, snakeControl_.getPosition());
	//return std::list<VisionPoints>();
}

size_t ml::LearningAgent::stepsPerformed() const
{
	//ml::SnakeAction& snakeAction = learningAgent_->Environment();
	//return snakeAction.StepsPerformed();
	return step_;
}

size_t ml::LearningAgent::maxSteps() const
{
	//ml::SnakeAction& snakeAction = learningAgent_->Environment();
	//return snakeAction.MaxSteps();
	return 0;
}

size_t ml::LearningAgent::totalSteps() const
{
	return agent_->TotalSteps();
}

void ml::LearningAgent::updateEnvironment()
{
	auto position = snakeControl_.getPosition();
	auto vision = snakeVision_.lookInAllDirections(board_, position);

	arma::colvec& data = environment_.Data();

	std::copy(std::begin(vision), std::end(vision), std::begin(data));

	data[24] = position.x_;
	data[25] = position.y_;

	auto direction = snakeControl_.getDirection();

	if (direction == SnakeControl::Direction::left) {
		data[26] = -1;
	}

	if (direction == SnakeControl::Direction::right) {
		data[26] = 1;
	}

	if (direction == SnakeControl::Direction::up) {
		data[27] = -1;
	}

	if (direction == SnakeControl::Direction::down) {
		data[27] = 1;
	}

	//Cell* cell = board_.findFood();

	//data[28] = cell->x_;
	//data[29] = cell->y_;
}

bool ml::LearningAgent::isReadyForNextStep()
{
	return readyForNextStep_;
}
