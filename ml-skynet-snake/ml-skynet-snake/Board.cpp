#include "Board.hpp"
#include "Settings.hpp"
#include <algorithm>
#include <random>

Board::Board(const Settings& settings)
{
	createBoard(settings.gridWidth_, settings.gridHeight_);
}

Cell* Board::findCell(const Point<std::size_t>& coordinate)
{
	const auto& it = std::find_if(grid_.begin(), grid_.end(), [&coordinate](const std::unique_ptr<Cell>& cell) {
		if (coordinate.x_ == cell->x_ && coordinate.y_ == cell->y_)
			return true;
		else
			return false;
	});
	
	if (it != grid_.end())
		return it->get();

	return nullptr;
}

void Board::resetBoard()
{
	grid_.clear();
	createBoard(gridWidth_, gridHeight_);
}

Point<std::size_t> Board::findRandomEmptyCell()
{
	Point<std::size_t> emptyCell{ 0,0 };

	std::random_device rd;
	std::default_random_engine generator(rd());
	
	std::uniform_int_distribution<size_t> distributionByWidth(0, gridWidth_);
	std::uniform_int_distribution<size_t> distributionByHeight(0, gridHeight_);

	while (true) {
		size_t randomX = distributionByWidth(generator);
		size_t randomY = distributionByHeight(generator);
		emptyCell.x_ = randomX;
		emptyCell.y_ = randomY;

		Cell* cell = findCell(emptyCell);
		if (cell && cell->type_ == Cell::Type::empty) {
			break;
		}
	}

	return emptyCell;
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
			} else {
				grid_.emplace_back(std::make_unique<Cell>(Cell::Type::empty, row, column));
			}
		}
	}
}


