#include "RunningStateAI.h"
#include "Utils.hpp"
#include "Game.hpp"
#include "Board.hpp"
#include "GameOverState.hpp"
#include <iostream>
#include <iterator>
/*
#include <mlpack/core.hpp>

#include <mlpack/methods/ann/ffn.hpp>
#include <mlpack/methods/ann/init_rules/gaussian_init.hpp>
#include <mlpack/methods/ann/layer/layer.hpp>
#include <mlpack/methods/ann/loss_functions/mean_squared_error.hpp>
#include <mlpack/methods/reinforcement_learning/q_learning.hpp>
#include <mlpack/methods/reinforcement_learning/environment/mountain_car.hpp>
#include <mlpack/methods/reinforcement_learning/environment/acrobot.hpp>
#include <mlpack/methods/reinforcement_learning/environment/cart_pole.hpp>
#include <mlpack/methods/reinforcement_learning/environment/double_pole_cart.hpp>
#include <mlpack/methods/reinforcement_learning/policy/greedy_policy.hpp>
#include <mlpack/methods/reinforcement_learning/training_config.hpp>

#include <ensmallen.hpp>

using namespace mlpack;
using namespace mlpack::ann;
using namespace ens;
using namespace mlpack::rl;

using namespace mlpack;
using namespace mlpack::ann;
using namespace ens;
using namespace mlpack::rl;
*/
RunningStateAI::RunningStateAI(Game& game) : RunningState(game)
{
	/*
	// Set up the feed forward neural network.
	FFN<MeanSquaredError<>, GaussianInitialization> model(MeanSquaredError<>(), GaussianInitialization(0, 0.001));    // Gaussian Initialization is how we initialize the weights in the neural network, with mean 0 and standard deviation 0.001
	model.Add<Linear<>>(4, 128);
	model.Add<ReLULayer<>>();
	model.Add<Linear<>>(128, 128);
	model.Add<ReLULayer<>>();
	model.Add<Linear<>>(128, 2);

	// Set up the policy and replay method.
	GreedyPolicy<CartPole> policy(1.0, 1000, 0.1, 0.99);
	RandomReplay<CartPole> replayMethod(10, 10000);

	TrainingConfig config;
	config.StepSize() = 0.01;
	config.Discount() = 0.9;
	config.TargetNetworkSyncInterval() = 100;
	config.ExplorationSteps() = 100;
	config.DoubleQLearning() = false;
	config.StepLimit() = 200;

	// Set up DQN agent.
	QLearning<CartPole, decltype(model), AdamUpdate, decltype(policy)>
		agent(std::move(config), std::move(model), std::move(policy),
			std::move(replayMethod));

	long episodes{ 0 };

	while (true)
	{
		double episodeReturn = agent.Episode();
		double averageReturn(episodeReturn);
		episodes += 1;
	}*/
}

void RunningStateAI::enter()
{
	std::cout << " RunningStateAI::enter()" << std::endl;
	RunningState::enter();

	auto& brain = snake_.brain();
	brain.setBoard(static_cast<Board*>(&game_.board()));
	brain.setRenderer(static_cast<Renderer*>(&game_.renderer()));
	brain.setSimulation(static_cast<Simulation*>(&simulation_));
	brain.setGame(&game_);
	brain.setSnake(&snake_);

	/*
	arma::mat input_training_data;
	arma::mat output_training_data;

	mlpack::data::Load("training_data_input.csv", input_training_data, true);
	mlpack::data::Load("training_data_output.csv", output_training_data, true);

	model.Add<mlpack::ann::Linear<> >(input_training_data.n_rows, 3);
	model.Add<mlpack::ann::ReLULayer<> >();
	model.Add<mlpack::ann::ReLULayer<> >();
	model.Add<mlpack::ann::TanHLayer<> >();

	for (int i = 0; i < 10; ++i)
	{
		model.Train(input_training_data, output_training_data);
	}*/
}

void RunningStateAI::update(Renderer& renderer)
{
/*	if (running_ == false) {
		running_ = true;
		snake_.runAi();
	}

	updateDeltaTime_ += deltaTime;
	const unsigned int snakeSpeed = snake_.getSpeed();
	const Board& board{ game_.board() };

	//std::array<float, 24> vision = snakeVision_.lookInAllDirections(board, snake_, simulation_, renderer);

	if (updateDeltaTime_ > (utils::commonConstants::secondAsMilliseconds / snakeSpeed)) {
		
		const Board& board = game_.board();
		auto position = snake_.getPosition();
		auto direction = snake_.getDirection();

		const Point<std::size_t> target = simulation_.getNextSnakePosition(position, direction);
		const bool collision = simulation_.checkForCollisionWithWall(board, target);

		if (collision) {
			running_ = false;
			RunningState::enter();
			return;
		}

		const bool food = simulation_.checkForFood(board, target);

		if (food) {
			snake_.grow(1);
			newRandomPositionForFood();
		}
		
		simulation_.updateSnakePosition(snake_, target);

		updateDeltaTime_ = updateDeltaTime_ - (utils::commonConstants::secondAsMilliseconds / snakeSpeed);
	}

	printCurrentScoreToScreen(renderer);
	*/
}

void RunningStateAI::handleInput(const Keyboard& keyboard)
{

}
/*
arma::mat RunningStateAI::snakeVision()
{
	Point<std::size_t> position = snake_.getHeadPosition();

	const Snake::Direction direction = snake_.getDirection();
	const Snake::Direction previousDirection = snake_.previousDirection_;
	float snakeDirection{ 0 };
	bool leftBlocked = false;
	bool rightBlocked = false;
	
	Board& board = game_.board();
	const Point<std::size_t> target = simulation_.getNextSnakePosition(board, snake_);
	bool frontBlocked = simulation_.checkForCollision(board, target);

	bool left = false;
	bool front = false;
	bool right = false;

	if (direction == Snake::Direction::left) {
		snakeDirection = 0.75;

		Point<std::size_t> testTarget = position;

		testTarget.y_ -= 1;
		leftBlocked = simulation_.checkForCollision(board, testTarget);

		testTarget = position;
		testTarget.y_ += 1;
		rightBlocked = simulation_.checkForCollision(board, testTarget);
	}
	else if (direction == Snake::Direction::right) {
		snakeDirection = 0.25;

		Point<std::size_t> testTarget = position;

		testTarget.y_ += 1;
		leftBlocked = simulation_.checkForCollision(board, testTarget);

		testTarget = position;
		testTarget.y_ -= 1;
		rightBlocked = simulation_.checkForCollision(board, testTarget);
	}
	else if (direction == Snake::Direction::up) {
		snakeDirection = 0;

		Point<std::size_t> testTarget = position;

		testTarget.x_ -= 1;
		leftBlocked = simulation_.checkForCollision(board, testTarget);

		testTarget = position;
		testTarget.y_ += 1;
		rightBlocked = simulation_.checkForCollision(board, testTarget);
	}
	else if (direction == Snake::Direction::down) {
		snakeDirection = 0.5;

		Point<std::size_t> testTarget = position;

		testTarget.x_ += 1;
		leftBlocked = simulation_.checkForCollision(board, testTarget);

		testTarget = position;
		testTarget.y_ -= 1;
		rightBlocked = simulation_.checkForCollision(board, testTarget);
	}

	float angle = utils::math::snakeAngleToFood(snake_.getFirstBodyPosition(), snake_.getHeadPosition(), food_.position());
	arma::mat vision(5, 1);
	vision(0, 0) = (double)leftBlocked;
	vision(1, 0) = (double)frontBlocked;
	vision(2, 0) = (double)rightBlocked;
	vision(3, 0) = (double)angle;
	vision(4, 0) = (double)snakeDirection;

	return vision;
}*/
