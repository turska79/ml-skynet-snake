
#include "LearningAgent.hpp"
#include "../SnakeControl.hpp"
#include "../Board.hpp"
#include <mutex>
#include <condition_variable>
#include <memory>
#include "../utils/InterruptibleThread.hpp"
#include "../utils/Utils.hpp"
#include "../Game.hpp"
#include "../states/GameOverState.hpp"
#pragma warning(push)  
#pragma warning(disable : 26819 26812)
#include <SDL.h>
#pragma warning( pop )

#pragma warning(push)  
#pragma warning(disable : 26819 26495 4244 26451 6011 26439 26812 4458 4267 4801 4081)
#pragma warning (pop)

namespace ml {
	constexpr size_t batchSize{ 5 };
	constexpr size_t capacity{ 100000 };
	constexpr double alpha{ 0.6 };
}


ml::LearningAgent::LearningAgent(SnakeControl& snakeControl, Board& board, Game& game) : snakeControl_(snakeControl), board_(board), game_(game)
{
	replayMethod_ = std::make_unique<ml::ReplayMethod>(ml::batchSize, ml::capacity, ml::alpha);
	auto& replayMethod{ *(replayMethod_.get()) };

	policyNetwork_ = std::make_unique<ml::FeedForwardNetwork>(mlpack::ann::EmptyLoss<>(), mlpack::ann::GaussianInitialization(0, 1.0));

	policyNetwork_->Add(new  mlpack::ann::Linear<>(utils::commonConstants::ml::inputParameters::count, utils::commonConstants::ml::number_of_hidden_neurons));
	policyNetwork_->Add(new  mlpack::ann::ReLULayer<>());
	policyNetwork_->Add(new  mlpack::ann::Linear<>(utils::commonConstants::ml::number_of_hidden_neurons, utils::commonConstants::ml::number_of_hidden_neurons));
	policyNetwork_->Add(new  mlpack::ann::ReLULayer<>());
	policyNetwork_->Add(new  mlpack::ann::Linear<>(utils::commonConstants::ml::number_of_hidden_neurons, utils::commonConstants::ml::number_of_hidden_neurons));
	policyNetwork_->Add(new  mlpack::ann::ReLULayer<>());
	policyNetwork_->Add(new  mlpack::ann::Linear<>(utils::commonConstants::ml::number_of_hidden_neurons, utils::commonConstants::ml::number_of_possible_actions));
	policyNetwork_->Add(new  mlpack::ann::SigmoidLayer<>());
	policyNetwork_->ResetParameters();
	FeedForwardNetwork& policyNetwork = *(policyNetwork_.get());

	qnetwork_ = std::make_unique<ml::FeedForwardNetwork>(mlpack::ann::EmptyLoss<>(), mlpack::ann::GaussianInitialization(0, 1.0));
	qnetwork_->Add(new mlpack::ann::Linear<>(utils::commonConstants::ml::inputParameters::count + utils::commonConstants::ml::number_of_possible_actions, utils::commonConstants::ml::number_of_hidden_neurons));
	qnetwork_->Add(new mlpack::ann::ReLULayer<>());
	qnetwork_->Add(new mlpack::ann::Linear<>(utils::commonConstants::ml::number_of_hidden_neurons, utils::commonConstants::ml::number_of_hidden_neurons));
	qnetwork_->Add(new mlpack::ann::ReLULayer<>());
	qnetwork_->Add(new mlpack::ann::Linear<>(utils::commonConstants::ml::number_of_hidden_neurons, utils::commonConstants::ml::number_of_hidden_neurons));
	qnetwork_->Add(new mlpack::ann::ReLULayer<>());

	qnetwork_->Add(new mlpack::ann::Linear<>(utils::commonConstants::ml::number_of_hidden_neurons, 1));
	qnetwork_->ResetParameters();
	FeedForwardNetwork& qNetwork = *(qnetwork_.get());
		
	trainingConfig_ = std::make_unique<Config>();
	mlpack::rl::TrainingConfig& config{ *(trainingConfig_.get()) };

	config.ExplorationSteps() = 10;
	config.TargetNetworkSyncInterval() = 1;
	config.UpdateInterval() = 4;
	config.StepLimit() = 500;
	config.DoubleQLearning() = true;

	ContinuousActionEnvironment environment(std::addressof(snakeControl_), std::addressof(snakeVision_), std::addressof(board_), std::addressof(envState_));

	agent_ = std::make_unique<SoftActionCritic>(config, qNetwork, policyNetwork, replayMethod, ens::AdamUpdate(), ens::AdamUpdate(), environment);
}

ml::LearningAgent::~LearningAgent()
{
}

subjects::EpisodeCompleteSubject& ml::LearningAgent::episodeCompleteSubject()
{
	return episodeCompleteSubject_;
}

std::list<VisionPoints> ml::LearningAgent::currentVision()
{
	return snakeVision_.pointsForRendering(board_, snakeControl_.getPosition());
}

size_t ml::LearningAgent::stepsPerformed() const
{
	return agent_->State().step();
}

size_t ml::LearningAgent::maxSteps() const
{
	return utils::commonConstants::ml::max_steps;
}

size_t ml::LearningAgent::totalSteps() const
{
	return agent_->TotalSteps();
}

void ml::LearningAgent::run()
{
	agentThread_ = std::make_unique<thread::interruptibleThread>(this, &LearningAgent::runEpisode, "ml agent");
}

bool ml::LearningAgent::running()
{
	return running_;
}

void ml::LearningAgent::runEpisode()
{
	std::cout << "LearningAgent::runEpisode() enter" << std::endl;
	running_ = true;
	double reward = agent_->Episode();
	
	auto& agentSate{ envState_.environmentState() };
	agentSate = ml::EnvironmentState::State::Stopped;

	running_ = false;
	episodeCompleteSubject_.invoke();
	std::cout << "LearningAgent::runEpisode() reward: " << std::to_string(reward) <<  std::endl;

}

void ml::LearningAgent::advanceEnvironment()
{
	try {
		std::cout << "LearningAgent::advanceEnvironment() " << std::endl;
		std::mutex mutex;
		std::condition_variable cv;
		std::unique_lock<std::mutex> lock(mutex);

		auto function = [&]() -> bool { return envState_.environmentState() == ml::EnvironmentState::State::Idle; };

		std::cout << "LearningAgent::advanceEnvironment() waiting for environment to finish" << std::endl;
		thread::utils::interruptibleWait<decltype(function)>(cv, lock, function);

		auto& agentSate{ envState_.environmentState() };
		std::cout << "LearningAgent::advanceEnvironment() set process for environment" << std::endl;
		agentSate = ml::EnvironmentState::State::Process;
	}
	catch (const std::exception&) {
		std::cout << "LearningAgent::advanceEnvironment() exception" << std::endl;
	}
}
void ml::LearningAgent::waitUntilStopped()
{
	std::cout << "LearningAgent::waitUntilStopped()" << std::endl;

	envState_.environmentState() = ml::EnvironmentState::State::Stop;

	std::mutex mutex;
	std::condition_variable cv;
	std::unique_lock<std::mutex> lock(mutex);

	auto function = [&]() -> bool { 
		return envState_.environmentState() == ml::EnvironmentState::State::Stopped;
	};

	thread::utils::interruptibleWait<decltype(function)>(cv, lock, function);
	std::cout << "LearningAgent::waitUntilStopped() exit" << std::endl;
}
