#include "Food.hpp"
#include "Board.hpp"

void Food::init(Point<std::size_t> position, Board& board)
{
	position_ = position;

	Cell* cell = board.findCell(position_);
	cell->type_ = Cell::Type::food;
}

void Food::updatePosition(Board& board, const Point<std::size_t> position)
{
	Cell* cell = board.findCell(position_);
	cell->type_ = Cell::Type::empty;

	position_ = position;
	cell = board.findCell(position_);
	cell->type_ = Cell::Type::food;
}