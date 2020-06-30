#pragma once

#include <cstddef>
#include "Point.hpp"
#include "Board.hpp"
#include "SnakeMovement.hpp"
#include "Timer.hpp"

class Board;

class Simulation
{
public:
	Simulation(Board& board);
	void start();
	void update(SnakeMovement& snakeMovement);
	
	const Point<std::size_t> getNextSnakePosition(const Point<std::size_t> currentPosition, const SnakeMovement::Direction direction) const noexcept;

	const bool collision() const;

	const bool checkForCollisionWithWall(const Point<std::size_t>& target) const;
	const bool checkForCollisionWithSnakeBody(const Point<std::size_t>& target) const;
	const bool checkForCollisionWithFood(const Point<std::size_t>& target) const;

private:
	Timer timer_;

	double snakeSpeed_{ 10 };
	bool collision_{ false };
	bool running_{ false };
	uint32_t updateDeltaTime_;
	Board& board_;
};

