#pragma once

#include <memory>
#include <atomic>

#include "Environments.hpp"
#include "../Snakevision.hpp"
#include "../SnakeControl.hpp"
#include "../utils/InterruptibleThread.hpp"

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
	//using Policy = mlpack::rl::GreedyPolicy<ml::ContinuousActionEnvironment>; //GreedyPolicy<CartPole> policy(1.0, 1000, 0.1, 0.99);
	using SoftActionCritic = mlpack::rl::SAC<ml::ContinuousActionEnvironment, FeedForwardNetwork, FeedForwardNetwork, ens::AdamUpdate, ReplayMethod>;
	
	class LearningAgent// : public ContinuousActionEnvironment
	{
	public:
		//enum class LearningAgentState { Idle, Process, Processing };
		LearningAgent() = delete;
		LearningAgent(SnakeControl& snakeControl, Board& board, Game& game);
		~LearningAgent();
		//void runLearningAgent();
		//bool isProcessing();
		//bool isIdle();
		//void processNextStep();
		//bool isProcessNextStepSet();

		/*
		auto learningAgentState() const& -> const LearningAgentState& { return state_; }
		auto learningAgentState() & -> std::atomic<LearningAgentState>& { return state_; }
		auto learningAgentState() && -> LearningAgentState&& { return std::move(state_); }
		*/
		std::list<VisionPoints> currentVision();
		size_t stepsPerformed() const;
		size_t maxSteps() const;
		size_t totalSteps() const;

		void run();
		void runEpisode();
		//ml::State& envState() { return *this;  }
		//ml::State InitialSample();
		//bool IsTerminal(const ml::ContinuousActionEnvironment::State& state);
		//double Sample(const ml::ContinuousActionEnvironment::State& currentState, const ml::ContinuousActionEnvironment::Action action, ml::ContinuousActionEnvironment::State& nextState);
		//SnakeControl::Direction actionToDirection(const std::vector<double> action) const;

		void advanceEnvironment();
		void waitUntilStopped();
		//void updateEnvironment();
	protected:
		
		//void updateEnvironment();
		//ml::ContinuousActionEnvironment::State sample(utils::Point<std::size_t> fromPoint);

		//bool isReadyForNextStep();
		//SnakeControl::Direction decodeDirection(const unsigned int rawDirection) const;

		std::unique_ptr<ReplayMethod> replayMethod_;
		std::unique_ptr<Config> trainingConfig_;
		std::unique_ptr<FeedForwardNetwork> policyNetwork_;
		std::unique_ptr<FeedForwardNetwork> qnetwork_;
		std::unique_ptr<SoftActionCritic> agent_;

		//ContinuousActionEnvironment environment_;
		//std::unique_ptr<Policy> policy_;
		//Environment environment_;

		std::unique_ptr<thread::interruptibleThread> agentThread_;

		SnakeControl& snakeControl_;
		SnakeVision snakeVision_;
		Board& board_;
		Game& game_;
		EnvironmentState envState_;
		//std::atomic<bool> episodeRunning_{ false };
		//std::atomic<bool> processNextStep_{ false };
		//std::atomic<LearningAgentState> state_{ LearningAgentState::Idle };
		size_t step_{ 0 };
		//double distanceToFood_{ 0 };
	};

}
