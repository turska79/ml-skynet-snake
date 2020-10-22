#pragma once

#include <cstddef>
#include <list>
#include "Point.hpp"
#include "SnakeControl.hpp"
#include "SimulationObject.hpp"
#include "SnakePositionUpdatedSubject.hpp"
#include "SnakeCollisionSubject.hpp"
#include "FoodEatenSubject.hpp"

class Board;

class Snake : public SnakeControl, public simulationObject
{
public:
	Snake(Board& board);
	
	void init(const Point<std::size_t> position, const SnakeControl::Direction direction) override;
	void updatePosition(const Point<std::size_t> newPosition) override;
	void setDirection(const SnakeControl::Direction direction) noexcept override;
	const SnakeControl::Direction getDirection() const noexcept override;
	Point<std::size_t> getPosition() const noexcept override;
	void grow(const unsigned int length) noexcept override;

	const unsigned int length() const noexcept;

//	virtual void attach(SnakeObserver *observer) override;
//	virtual void detach(SnakeObserver *observer) override;
//	virtual void notify() override;

	virtual void update(const uint32_t delta) noexcept override;
	//SnakeBrain& brain() noexcept;
	//void runLearningAgentForSingleGame();
	SnakePositionUpdatedSubject& positionUpdateSubject() noexcept;
	SnakeCollisionSubject& snakeCollisionSubject() noexcept;
	FoodEatenSubject& foodEatenSubject() noexcept;
private:
	const Point<std::size_t> getNextSnakePosition(const Point<std::size_t> currentPosition, const SnakeControl::Direction direction) const noexcept;
	const bool checkForCollision(const Point<std::size_t> target);
	void notifyPositionObservers() noexcept;
	void notifyCollisionObservers() noexcept;
	void notifyFoodEatenObservers() noexcept;

	SnakePositionUpdatedSubject positionUpdateSubject_;
	SnakeCollisionSubject snakeCollisionSubject_;
	FoodEatenSubject foodEatenSubject_;

	uint32_t lastUpdateTime_{ 0 };
	Point<std::size_t> headPosition_{ 0,0 };
	std::list<Point<std::size_t>> body_;
	SnakeControl::Direction direction_{ SnakeControl::Direction::right };

//	std::list<SnakeObserver *> observers_;
	const unsigned int updateRate{ 100 };
	Board& board_;

	
};


