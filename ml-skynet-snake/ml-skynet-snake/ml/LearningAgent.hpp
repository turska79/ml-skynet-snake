#pragma once

#include <memory>
#include <atomic>

#include "Environments.hpp"
#include "../Snakevision.hpp"
#include "../SnakeControl.hpp"
#include "../utils/InterruptibleThread.hpp"
#include "../subjects/EpisodeCompleteSubject.hpp"

#pragma warning(push)  
#pragma warning(disable : 26819 26495 4244 26451 6011 26439 26812 4458 4267 4801 4081)
#include <mlpack/prereqs.hpp>
#include <mlpack/core.hpp>
#include <mlpack/methods/ann/ffn.hpp>
#include <mlpack/methods/reinforcement_learning/policy/greedy_policy.hpp>
#include <mlpack/methods/reinforcement_learning/replay/prioritized_replay.hpp>
#include <mlpack/methods/reinforcement_learning/sac.hpp>
#include <mlpack/methods/ann/loss_functions/empty_loss.hpp>
#include <mlpack/methods/ann/init_rules/gaussian_init.hpp>
#pragma warning(pop)

class SnakeControl;
class Board;
class Game;

namespace ml {
	using ReplayMethod = mlpack::rl::PrioritizedReplay<ContinuousActionEnvironment>;
	using Config = mlpack::rl::TrainingConfig;
	using FeedForwardNetwork = mlpack::ann::FFN<mlpack::ann::EmptyLoss<>, mlpack::ann::GaussianInitialization>;
	using SoftActionCritic = mlpack::rl::SAC<ml::ContinuousActionEnvironment, FeedForwardNetwork, FeedForwardNetwork, ens::AdamUpdate, ReplayMethod>;
	
	class LearningAgent// : public ContinuousActionEnvironment
	{
	public:
		LearningAgent() = delete;
		LearningAgent(SnakeControl& snakeControl, Board& board, Game& game);
		~LearningAgent();

		subjects::EpisodeCompleteSubject& episodeCompleteSubject();

		std::list<VisionPoints> currentVision();
		size_t stepsPerformed() const;
		size_t maxSteps() const;
		size_t totalSteps() const;

		void run();
		bool running();
		void runEpisode();

		void advanceEnvironment();
		void waitUntilStopped();

	protected:
		

		std::unique_ptr<ReplayMethod> replayMethod_;
		std::unique_ptr<Config> trainingConfig_;
		std::unique_ptr<FeedForwardNetwork> policyNetwork_;
		std::unique_ptr<FeedForwardNetwork> qnetwork_;
		std::unique_ptr<SoftActionCritic> agent_;

		subjects::EpisodeCompleteSubject episodeCompleteSubject_;

		std::unique_ptr<thread::interruptibleThread> agentThread_;

		SnakeControl& snakeControl_;
		SnakeVision snakeVision_;
		Board& board_;
		Game& game_;
		EnvironmentState envState_;
		size_t step_{ 0 };
		std::atomic<bool> running_{ false };
	};

}
