#pragma once

#include "RunningState.hpp"
#include "../SnakeBrain.hpp"
#include <atomic>

#pragma warning (push, 0)
#include <mlpack/core.hpp>
#include <mlpack/methods/ann/ffn.hpp>
#include <mlpack/methods/ann/init_rules/gaussian_init.hpp>
#include <mlpack/methods/ann/layer/layer.hpp>
#include <mlpack/methods/ann/loss_functions/mean_squared_error.hpp>
#include <mlpack/methods/reinforcement_learning/q_learning.hpp>
#include <mlpack/methods/reinforcement_learning/environment/mountain_car.hpp>
//#include <mlpack/methods/reinforcement_learning/environment/acrobot.hpp>
//#include <mlpack/methods/reinforcement_learning/environment/cart_pole.hpp>
//#include <mlpack/methods/reinforcement_learning/environment/double_pole_cart.hpp>
#include <mlpack/methods/reinforcement_learning/policy/greedy_policy.hpp>
#include <mlpack/methods/reinforcement_learning/training_config.hpp>

#include <ensmallen.hpp>
#pragma warning (pop)

class RunningStateAI : public RunningState
{
public:
	explicit RunningStateAI(Game& game);
	virtual ~RunningStateAI() = default;
	RunningStateAI(const RunningStateAI&) = default;
	RunningStateAI& operator=(const RunningStateAI&) = default;
	RunningStateAI(RunningStateAI&&) = default;
	RunningStateAI& operator=(RunningStateAI&&) = default;

	void enter() override;
	void update(Renderer& renderer) override;
	void handleInput(const Keyboard& keyboard) override;
private:
	void printStepsToScreen(Renderer& renderer);
	void printGameCountToScreen(Renderer& renderer);
	SnakeBrain& snakeBrain() noexcept;

	std::atomic<bool> running_{ false };
	std::size_t gameCount_{ 0 };

	mlpack::rl::QLearning<SnakeBrain, mlpack::ann::FFN<mlpack::ann::MeanSquaredError<>, mlpack::ann::GaussianInitialization>, ens::AdamUpdate, mlpack::rl::GreedyPolicy<SnakeBrain> >* learningAgent_{ nullptr };
};

