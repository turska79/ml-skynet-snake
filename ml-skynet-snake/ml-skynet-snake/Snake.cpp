#include "Snake.hpp"
#include "Board.hpp"

Snake::Snake()
{

}


Snake::~Snake()
{
}

void Snake::init(Point<std::size_t> position, Board& board)
{
	headPosition_ = position;

	Cell* cell = board.findCell(headPosition_);
	cell->type_ = Cell::Type::head;
}

void Snake::updatePosition(Board& board)
{
	Cell* cell = board.findCell(headPosition_);
	cell->type_ = Cell::Type::head;
}
