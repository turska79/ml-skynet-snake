#pragma once

#include <cstddef>
#include <list>
#include "Point.hpp"
#include "SnakeBrain.hpp"
#include "SnakeMovement.hpp"

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

class Board;
class Keyboard;

class Snake : public SnakeMovement
{
public:
	Snake(Board& board);
	
	void init(const Point<std::size_t> position, const SnakeMovement::Direction direction) override;
	void updatePosition(const Point<std::size_t> newPosition) override;
	void setDirection(const SnakeMovement::Direction direction) noexcept override;
	const SnakeMovement::Direction getDirection() const noexcept override;
	Point<std::size_t> getPosition() const noexcept override;
	const unsigned int getSpeed() const noexcept;
	void grow(const unsigned int length) noexcept override;
	const unsigned int length() const noexcept;
	SnakeBrain& brain() noexcept;
	void runLearningAgentForSingleGame();
private:
	Point<std::size_t> headPosition_{ 0,0 };
	std::list<Point<std::size_t>> body_;
	SnakeMovement::Direction direction_{ SnakeMovement::Direction::right };
public:
	SnakeMovement::Direction previousDirection_{ SnakeMovement::Direction::right };
private:
	unsigned int speed_{ 10 };
	Board& board_;

	mlpack::rl::QLearning<SnakeBrain, mlpack::ann::FFN<mlpack::ann::MeanSquaredError<>, mlpack::ann::GaussianInitialization>, ens::AdamUpdate, mlpack::rl::GreedyPolicy<SnakeBrain> >* learningAgent_ { nullptr };
};

