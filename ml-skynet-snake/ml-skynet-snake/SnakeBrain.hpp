#pragma once

#include "SnakeVision.hpp"

#include <vector>
#include <array>

#include <mlpack/core.hpp>
#include <mlpack/methods/ann/ffn.hpp>
#include <mlpack/methods/ann/init_rules/gaussian_init.hpp>
#include <mlpack/methods/ann/layer/layer.hpp>
#include <mlpack/methods/ann/loss_functions/mean_squared_error.hpp>
#include <mlpack/methods/reinforcement_learning/q_learning.hpp>
//#include <mlpack/methods/reinforcement_learning/environment/mountain_car.hpp>
//#include <mlpack/methods/reinforcement_learning/environment/acrobot.hpp>
#include <mlpack/methods/reinforcement_learning/environment/cart_pole.hpp>
//#include <mlpack/methods/reinforcement_learning/environment/double_pole_cart.hpp>
#include <mlpack/methods/reinforcement_learning/policy/greedy_policy.hpp>
#include <mlpack/methods/reinforcement_learning/training_config.hpp>

#include <ensmallen.hpp>

class Board;
class SnakeMovement;
class Renderer;
class Simulation;
class Game;
class Snake;

class SnakeBrain
{
public:
	SnakeBrain() = default;
	~SnakeBrain();
	void setBoard(Board* board);
	void setSnakeMovementInterface(SnakeMovement* snakeMovement);
	void setRenderer(Renderer* renderer);
	void setSimulation(Simulation* simulation);
	void setGame(Game* game);
	void setSnake(Snake* snake);
	//class LearningAgent
	//{
	public:
		class State
		{
		public:
			State() : data(dimension)
			{
			}

			State(const arma::colvec& data) : data(data)
			{
			}

			//! Modify the internal representation of the state.
			arma::colvec& Data() { return data; }
			
			std::array<float, 24> vision() const {
				std::array<float, 24> vision{ 0 };
				auto iter = std::begin(data);
				std::advance(iter, 24);
				std::copy(std::begin(data), iter, std::begin(vision));
				return vision;
			}

			double coordinateX() const {
				return data[24];
			}
			double& coordinateX() {
				return data[24];
			}

			double coordinateY() const {
				return data[25];
			}
			double& coordinateY() {
				return data[25];
			}

			//! Encode the state to a column vector.
			const arma::colvec& Encode() const { return data; }

			//! Dimension of the encoded state.
			static constexpr size_t dimension{ 26 };

		private:
			//! Locally-stored (vision, position x, position y).
			arma::colvec data;

		};

		enum Action
		{
			up,
			down,
			right,
			left,
			// Track the size of the action space.
			size
		};

		double Sample(const State& state, const Action& action, State& nextState);

		double Sample(const State& state, const Action& action);

		State InitialSample();

		bool IsTerminal(const State& state) const;

		size_t StepsPerformed() const;

		size_t MaxSteps() const;

		size_t& MaxSteps();


	private:
		

		//! Locally-stored done reward.
		double doneReward{ 0 };

		const double foodReward{ 10 };

		//! Locally-stored maximum number of steps.
		size_t maxSteps{ 500 };

		//! Locally-stored number of steps performed.
		size_t stepsPerformed{ 0 };
	//};

//public:
	//mlpack::rl::QLearning<SnakeBrain::LearningAgent, mlpack::ann::FFN<mlpack::ann::MeanSquaredError<>, mlpack::ann::GaussianInitialization>, ens::AdamUpdate, mlpack::rl::GreedyPolicy<SnakeBrain::LearningAgent> >* learningAgent_{ nullptr };
private:
	Board* board_{ nullptr };
	SnakeMovement* snakeMovement_{ nullptr };
	Renderer* renderer_{ nullptr };
	Simulation* simulation_{ nullptr };
	SnakeVision snakeVision_;
	Game* game_{ nullptr };
	Snake* snake_{ nullptr };
};

