#pragma once

#include <cstddef>
#include "Point.hpp"

class SnakeMovement {
public:
	enum class Direction { up, down, left, right };

	virtual Point<std::size_t> getPosition() const noexcept = 0;
	virtual void updatePosition(const Point<std::size_t> newPosition) = 0;
	virtual void setDirection(const Direction direction) noexcept = 0;
	virtual const Direction getDirection() const noexcept = 0;

	virtual void grow(const unsigned int length) noexcept  = 0;
};
