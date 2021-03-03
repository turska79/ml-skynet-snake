
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
	policyNetwork_->Add(new  mlpack::ann::Linear<>(128, utils::commonConstants::ml::number_of_possible_actions));
	policyNetwork_->Add(new  mlpack::ann::TanHLayer<>());
	policyNetwork_->ResetParameters();
	FeedForwardNetwork& policyNetwork = *(policyNetwork_.get());

	qnetwork_ = std::make_unique<ml::FeedForwardNetwork>(mlpack::ann::EmptyLoss<>(), mlpack::ann::GaussianInitialization(0, 0.01));
	qnetwork_->Add(new mlpack::ann::Linear<>(utils::commonConstants::ml::input_parameters + utils::commonConstants::ml::number_of_possible_actions, 128));
	qnetwork_->Add(new mlpack::ann::ReLULayer<>());
	qnetwork_->Add(new mlpack::ann::Linear<>(128, 128));
	qnetwork_->Add(new mlpack::ann::ReLULayer<>());
	qnetwork_->Add(new mlpack::ann::Linear<>(128, 1));
	qnetwork_->ResetParameters();
	FeedForwardNetwork& qNetwork = *(qnetwork_.get());
		
	trainingConfig_ = std::make_unique<Config>();
	mlpack::rl::TrainingConfig& config{ *(trainingConfig_.get()) };

	config.ExplorationSteps() = 10;
	config.TargetNetworkSyncInterval() = 1;
	config.UpdateInterval() = 1;

	agent_ = std::make_unique<SoftActionCritic>(config, qNetwork, policyNetwork, replayMethod);
}

ml::LearningAgent::~LearningAgent()
{
}

// Function to calculate distance 
float distance(int x1, int y1, int x2, int y2)
{
	// Calculating distance 
	return sqrt(pow(x2 - x1, 2) +
		pow(y2 - y1, 2) * 1.0);
}

void ml::LearningAgent::runLearningAgent()
{
	episodeRunning_ = true;

	std::condition_variable cv;
	std::mutex mutex;

	step_ = 0;
	double totalReturn{ 0.0 };
	double reward{ 0 };
	double episodeReturn{ 0.0 };

	updateEnvironment();
	
	while (episodeRunning_) {
		std::cout << "LearningAgent::runLearningAgent() thinking about next move" << std::endl;

		Cell* cell = board_.findFood();
		auto currentPosition{ snakeControl_.getPosition() };
		
		distanceToFood_ = distance(currentPosition.x_, currentPosition.y_, cell->x_, cell->y_);
		std::cout << "LearningAgent::runLearningAgent() distance to food: " << std::to_string(distanceToFood_) << std::endl;
		
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

		SnakeControl::Direction newDirection{ decodeDirection(maxIndex) };
		
		switch (newDirection) {
		case SnakeControl::Direction::up:
			std::cout << "LearningAgent::runLearningAgent() action: up" << std::endl;
			break;
		case SnakeControl::Direction::right:
			std::cout << "LearningAgent::runLearningAgent() action: right" << std::endl;
			break;
		case SnakeControl::Direction::down:
			std::cout << "LearningAgent::runLearningAgent() action: down" << std::endl;
			break;
		case SnakeControl::Direction::left:
			std::cout << "LearningAgent::runLearningAgent() action: left" << std::endl;
			break;
		}

		snakeControl_.setDirection(newDirection);

		auto nextPosition = snakeControl_.getNextPosition();

		if (board_.isFood(nextPosition)) {
			reward = 500;
		} else if (board_.isWall(nextPosition) || step_ >= maxSteps()) {// || board_.isSnakeBody(nextPosition)) {
			reward = -500;
			episodeRunning_ = false;
		} else {
			double newDistanceToFood = distance(nextPosition.x_, nextPosition.y_, cell->x_, cell->y_);

			if (newDistanceToFood > distanceToFood_) {
				reward = -10;
			} else {
				reward = 20;
			}
		}

		readyForNextStep_ = false;

		try {
			std::unique_lock<std::mutex> lock(mutex);
			auto function = std::bind(&LearningAgent::isReadyForNextStep, this);

			thread::utils::interruptibleWait<decltype(function)>(cv, lock, function);
		}
		catch (const std::exception&) {
			episodeRunning_ = false;
		}

		updateEnvironment();

		Environment::State nextState;
		nextState.Data() = environment_.Encode();

		replayMethod_->Store(agent_->State(), agent_->Action(), reward, nextState, episodeRunning_, 0.99);
		episodeReturn += reward;
		std::cout << "LearningAgent::runLearningAgent() episode reward: " << std::to_string(episodeReturn) << std::endl;
		agent_->TotalSteps()++;
		step_++;
	}

	episodeRunning_ = false;
}

void ml::LearningAgent::proceedToNextStep()
{
	if (episodeRunning_ == false) {
		return;
	}
	std::cout << "ml::LearningAgent::proceedToNextStep()" << std::endl;
	readyForNextStep_ = true;
}

std::list<VisionPoints> ml::LearningAgent::currentVision()
{
	return snakeVision_.pointsForRendering(board_, snakeControl_.getPosition());
}

size_t ml::LearningAgent::stepsPerformed() const
{
	return step_;
}

size_t ml::LearningAgent::maxSteps() const
{
	return utils::commonConstants::ml::max_steps;
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
	} else if (direction == SnakeControl::Direction::right) {
		data[26] = 1;
	} else {
		data[26] = 0;
	}

	if (direction == SnakeControl::Direction::up) {
		data[27] = -1;
	} else if (direction == SnakeControl::Direction::down) {
		data[27] = 1;
	} else {
		data[27] = 0;
	}

	Cell* cell = board_.findFood();

	data[28] = cell->x_;
	data[29] = cell->y_;
}

bool ml::LearningAgent::isReadyForNextStep()
{
	return readyForNextStep_;
}

SnakeControl::Direction ml::LearningAgent::decodeDirection(const unsigned int rawDirection)
{
	const auto currentDirection{ snakeControl_.getDirection() };
	SnakeControl::Direction newDirection{ SnakeControl::Direction::right };
	
	const int left{ 0 };
	const int forward{ 1 };
	const int right{ 2 };

	if (rawDirection == forward) {
		return currentDirection;
	}
	
	if (currentDirection == SnakeControl::Direction::up) {
		if (rawDirection == left) {
			newDirection = SnakeControl::Direction::left;
		} else if (rawDirection == right) {
			newDirection = SnakeControl::Direction::right;
		}
	} else if (currentDirection == SnakeControl::Direction::right) {
		if (rawDirection == left) {
			newDirection = SnakeControl::Direction::up;
		} else if (rawDirection == right) {
			newDirection = SnakeControl::Direction::down;
		}
	} else if (currentDirection == SnakeControl::Direction::down) {
		if (rawDirection == left) {
			newDirection = SnakeControl::Direction::left;
		}
		else if (rawDirection == right) {
			newDirection = SnakeControl::Direction::right;
		}
	} else if (currentDirection == SnakeControl::Direction::left) {
		if (rawDirection == left) {
			newDirection = SnakeControl::Direction::down;
		}
		else if (rawDirection == right) {
			newDirection = SnakeControl::Direction::up;
		}
	}

	return newDirection;
}
