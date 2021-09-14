#pragma once

#include "utils/Point.hpp"
#include <cstddef>

class Board;

class Food
{
public:
	void updatePosition(Board& board, const utils::Point<std::size_t> position);
	const utils::Point<std::size_t> position() const noexcept;
private:
	utils::Point<std::size_t> position_{ 0, 0 };
};

