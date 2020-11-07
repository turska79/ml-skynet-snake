#include "Food.hpp"
#include "Board.hpp"
#include <string>
#include <iostream>

void Food::updatePosition(Board& board, const utils::Point<std::size_t> position)
{
	Cell* cell = board.findFood();
	if (cell != nullptr) {
		cell->type_ == Cell::Type::empty;
	}

	position_ = position;
	cell = board.findCell(position_);
	cell->type_ = Cell::Type::food;

	std::cout << "Food::updatePosition() x: " << std::to_string(static_cast<int>(position.x_)) << " y: " << std::to_string(static_cast<int>(position.x_)) << std::endl;
}

const utils::Point<std::size_t> Food::position() const noexcept
{
	return position_;
}
