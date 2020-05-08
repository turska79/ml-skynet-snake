#include "Board.hpp"
#include "Settings.hpp"
#include <algorithm>

Board::Board(Settings& settings)
{
	createBoard(settings.gridWidth_, settings.gridHeight_);
}


Board::~Board()
{
}

Cell* Board::findCell(const  Point<std::size_t>& coordinate)
{
	const auto& it = std::find_if(grid_.begin(), grid_.end(), [&coordinate](std::unique_ptr<Cell>& cell) {
		if (coordinate.x_ == cell->x_ && coordinate.y_ == cell->y_)
			return true;
		else
			return false;
	});
	
	if (it != grid_.end())
		return it->get();

	return nullptr;
}

void Board::createBoard(std::size_t gridWidth, std::size_t gridHeight)
{
	/*
	x x x x x
	x o o o x
	x o o o x
	x x x x x
	*/
	
	gridWidth_ = gridWidth;
	gridHeight_ = gridHeight;

	constexpr std::size_t topWall{ 0 };
	constexpr std::size_t leftWall{ 0 };
	const std::size_t rightWall = gridWidth_ - 1 ;
	const std::size_t bottomWall = gridHeight_ - 1 ;

	// y
	for (std::size_t row = 0; row < gridHeight_; ++row) {
		// x
		for (std::size_t column = 0; column < gridWidth_; ++column) {
			if (row == topWall) {
				grid_.emplace_back(std::make_unique<Cell>(Cell::Type::wall, row, column));
			} else if (column == leftWall) {
				grid_.emplace_back(std::make_unique<Cell>(Cell::Type::wall, row, column));
			} else if (column == rightWall) {
				grid_.emplace_back(std::make_unique<Cell>(Cell::Type::wall, row, column));
			} else if (row == bottomWall) {
				grid_.emplace_back(std::make_unique<Cell>(Cell::Type::wall, row, column));
			}
			else {
				grid_.emplace_back(std::make_unique<Cell>(Cell::Type::empty, row, column));
			}
		}
	}
}


