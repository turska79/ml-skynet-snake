#pragma once

#include <memory>
#include <atomic>

#include "Environments.hpp"
#include "../Snakevision.hpp"
#include "../SnakeControl.hpp"
//#include "FFNNetwork.hpp"

#pragma warning(push)  
#pragma warning(disable : 26819 26495 4244 26451 6011 26439 26812 4458 4267 4801 4081)
//#include <mlpack/>
//#include <mlpack/methods/ann/ffn.hpp>
//#include <mlpack/methods/reinforcement_learning/sac.hpp>

//#include <mlpack/methods/reinforcement_learning/q_learning.hpp>
//#include <mlpack/methods/reinforcement_learning/q_networks/simple_dqn.hpp>
#include <mlpack/prereqs.hpp>
#include <mlpack/core.hpp>
#include <mlpack/methods/ann/ffn.hpp>
#include <mlpack/methods/reinforcement_learning/sac.hpp>
#include <mlpack/methods/ann/loss_functions/empty_loss.hpp>
#include <mlpack/methods/ann/init_rules/gaussian_init.hpp>
//#include <mlpack/methods/reinforcement_learning/environment/env_type.hpp>
//#include <mlpack/methods/reinforcement_learning/training_config.hpp>
#pragma warning(pop)

class SnakeControl;
class Board;

namespace ml {
	using ReplayMethod = mlpack::rl::RandomReplay<ContinuousActionEnvironment>;
	//using ReplayMethod = mlpack::rl::RandomReplay<Environment>;
	using Config = mlpack::rl::TrainingConfig;
	using FeedForwardNetwork = mlpack::ann::FFN<mlpack::ann::EmptyLoss<>, mlpack::ann::GaussianInitialization>;
	using SoftActionCritic = mlpack::rl::SAC<ml::ContinuousActionEnvironment, FeedForwardNetwork, FeedForwardNetwork, ens::AdamUpdate>;
	//using SoftActionCritic = mlpack::rl::SAC<ml::Environment, FeedForwardNetwork, FeedForwardNetwork, ens::AdamUpdate>;

	class LearningAgent
	{
	public:
		LearningAgent() = delete;
		LearningAgent(SnakeControl& snakeControl, Board& board);
		~LearningAgent();
		void runLearningAgent();
		void proceedToNextStep();
		std::list<VisionPoints> currentVision();
		size_t stepsPerformed() const;
		size_t maxSteps() const;
		size_t totalSteps() const;
		
	protected:
		void updateEnvironment();

		bool isReadyForNextStep();
		SnakeControl::Direction decodeDirection(const unsigned int rawDirection);

		std::unique_ptr<ReplayMethod> replayMethod_;
		std::unique_ptr<Config> trainingConfig_;
		std::unique_ptr<FeedForwardNetwork> policyNetwork_;
		std::unique_ptr<FeedForwardNetwork> qnetwork_;
		std::unique_ptr<SoftActionCritic> agent_;

		Environment environment_;

		SnakeControl& snakeControl_;
		SnakeVision snakeVision_;
		Board& board_;
		std::atomic<bool> episodeRunning_;
		bool readyForNextStep_{ false };
		size_t step_{ 0 };
		double distanceToFood_{ 0 };
	};

}
