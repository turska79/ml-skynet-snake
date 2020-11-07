#pragma once

#include <memory>
#include <atomic>
#include "SnakeAction.hpp"

#pragma warning(push)  
#pragma warning(disable : 26819 26495 4244 26451 6011 26439 26812 4458 4267 4801 4081)
#include <mlpack/methods/ann/ffn.hpp>
#include <mlpack/methods/reinforcement_learning/q_learning.hpp>
#include <mlpack/methods/reinforcement_learning/q_networks/simple_dqn.hpp>
#pragma warning(pop)

class SnakeControl;
class Board;

namespace mlpack {

	namespace rl {
		template<typename T> class GreedyPolicy;
		template<typename T> class RandomReplay;
	}
}

namespace ml {
	class DQN;
	
	using Policy = mlpack::rl::GreedyPolicy<ml::SnakeAction>;
	using Replay = mlpack::rl::RandomReplay<ml::SnakeAction>;
	using Config = mlpack::rl::TrainingConfig;
	using QLearningAgent = mlpack::rl::QLearning<ml::SnakeAction, mlpack::rl::SimpleDQN<>, ens::AdamUpdate, Policy, Replay>;

	class LearningAgent
	{
	public:
		LearningAgent() = delete;
		LearningAgent(SnakeControl& snakeControl, Board& board);
		~LearningAgent();
		void runLearningAgent();
		void proceedToNextStep();
		std::list<VisionPoints> currentVision() const;
		size_t stepsPerformed() const;
		size_t maxSteps() const;
		size_t totalSteps() const;
	protected:
		std::unique_ptr<DQN> model_;
		std::unique_ptr<Config> trainingConfig_;
		std::unique_ptr<Policy> policy_;
		std::unique_ptr<Replay> replay_;
		std::atomic<bool> episodeRunning_;
		std::unique_ptr<QLearningAgent> learningAgent_;
		SnakeControl* control_{ nullptr };
		Board* board_{ nullptr };
	};

}
