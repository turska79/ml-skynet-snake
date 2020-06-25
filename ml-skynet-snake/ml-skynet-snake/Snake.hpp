#pragma once

#include <cstddef>
#include <list>
#include "Point.hpp"
#include "SnakeBrain.hpp"
#include "SnakeMovement.hpp"

class Board;
class Keyboard;

class Snake : public SnakeMovement
{
public:
	Snake(Board& board);
	
	void init(const Point<std::size_t> position, const SnakeMovement::Direction direction);
	void updatePosition(const Point<std::size_t> newHeadPosition);
	void setDirection(const SnakeMovement::Direction direction) noexcept override;
	const SnakeMovement::Direction getDirection() const noexcept override;
	Point<std::size_t> getPosition() const noexcept override;
	//Point<std::size_t> getFirstBodyPosition() noexcept;
	const unsigned int getSpeed() const noexcept;
	void grow(const unsigned int length);
	const unsigned int length() const noexcept;
	SnakeBrain& brain() noexcept;
	void runAi();
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

