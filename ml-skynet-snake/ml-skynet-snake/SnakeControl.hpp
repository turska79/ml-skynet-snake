#pragma once

#include <cstddef>
#include "Point.hpp"
//#include "Subject.hpp"

class SnakeControl {
public:
	enum class Direction { up, down, left, right };
	virtual void init(const Point<std::size_t> position, const Direction direction) = 0;

	virtual Point<std::size_t> getPosition() const noexcept = 0;
	virtual void updatePosition(const Point<std::size_t> newPosition) = 0;
	virtual void setDirection(const Direction direction) noexcept = 0;
	virtual const Direction getDirection() const noexcept = 0;

	virtual void grow(const unsigned int length) noexcept  = 0;

//virtual void attach(SnakeObserver* observer) = 0;
//	virtual void detach(SnakeObserver* observer) = 0;
//	virtual void notify() = 0;
};