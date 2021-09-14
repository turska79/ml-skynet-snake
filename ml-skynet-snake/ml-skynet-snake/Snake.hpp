#pragma once

#include <cstddef>
#include <list>
#include "utils/Point.hpp"
#include "SnakeControl.hpp"
#include "Simulation/SimulationObject.hpp"
#include "subjects/SnakePositionUpdatedSubject.hpp"
#include "subjects/SnakeCollisionSubject.hpp"
#include "subjects/FoodEatenSubject.hpp"

class Board;

class Snake : public SnakeControl, public simulationObject
{
public:
	Snake(Board& board);
	
	void init(const utils::Point<std::size_t> position, const SnakeControl::Direction direction) override;
	void updatePosition(const utils::Point<std::size_t> newPosition) override;
	void setDirection(const SnakeControl::Direction direction) noexcept override;
	const SnakeControl::Direction getDirection() const noexcept override;
	const utils::Point<std::size_t> getPosition() const noexcept override;
	const utils::Point<std::size_t> getNextPosition() const noexcept override;
	void grow(const unsigned int length) noexcept override;

	const unsigned int length() const noexcept;

//	virtual void attach(SnakeObserver *observer) override;
//	virtual void detach(SnakeObserver *observer) override;
//	virtual void notify() override;

	virtual void update(const uint32_t delta) noexcept override;
	//SnakeBrain& brain() noexcept;
	//void runLearningAgentForSingleGame();
	subjects::SnakePositionUpdatedSubject& positionUpdateSubject() noexcept;
	subjects::SnakeCollisionSubject& snakeCollisionSubject() noexcept;
	subjects::FoodEatenSubject& foodEatenSubject() noexcept;
private:
	const bool checkForCollision(const utils::Point<std::size_t> target);
	void notifyPositionObservers() noexcept;
	void notifyCollisionObservers() noexcept;
	void notifyFoodEatenObservers() noexcept;

	subjects::SnakePositionUpdatedSubject positionUpdateSubject_;
	subjects::SnakeCollisionSubject snakeCollisionSubject_;
	subjects::FoodEatenSubject foodEatenSubject_;

	uint32_t lastUpdateTime_{ 0 };
	utils::Point<std::size_t> headPosition_{ 0,0 };
	std::list<utils::Point<std::size_t>> body_;
	SnakeControl::Direction direction_{ SnakeControl::Direction::right };

//	std::list<SnakeObserver *> observers_;
	const unsigned int updateRate{ 100 };
	Board& board_;

	
};


