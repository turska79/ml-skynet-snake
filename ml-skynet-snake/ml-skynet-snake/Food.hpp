#pragma once

#include "utils/Point.hpp"
#include <cstddef>

class Board;

class Food
{
public:
	void init(Board& board, const utils::Point<std::size_t> position);
	void updatePosition(Board& board, const utils::Point<std::size_t> position);
	const utils::Point<std::size_t> position() const noexcept;
private:
	utils::Point<std::size_t> position_{ 0, 0 };
};

