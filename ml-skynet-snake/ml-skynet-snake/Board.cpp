#include "Board.hpp"
#include "Settings.hpp"
#include <algorithm>
#include <random>
#include <execution>
#include <iostream>
#include <assert.h>

namespace walls {
	static constexpr std::size_t topWall{ 0 };
	static constexpr std::size_t leftWall{ 0 };
	static std::size_t rightWall{ 0 };
	static std::size_t bottomWall{ 0 };
}

Board::Board(const Settings& settings)
{
	createBoard(settings.gridWidth_, settings.gridHeight_);
}

Cell* Board::findCell(const utils::Point<std::size_t>& toFind)
{
	auto const& it = std::find_if(grid_.begin(), grid_.end(), [&toFind](std::unique_ptr<Cell>& cell) noexcept {
		if (toFind.x_ == cell->x_ && toFind.y_ == cell->y_)
			return true;
		else
			return false;
	});

	if (it != grid_.end())
		return it->get();

	return nullptr;
}

Cell* Board::findFood()
{
	auto const& it = std::find_if(grid_.begin(), grid_.end(), [](std::unique_ptr<Cell>& cell) noexcept {
		if (cell->type_ == Cell::Type::food)
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

utils::Point<std::size_t> Board::findRandomEmptyCell()
{
	utils::Point<std::size_t> emptyCell{ 0,0 };

	std::random_device rd;
	std::default_random_engine generator(rd());
	
	const std::uniform_int_distribution<size_t> distributionByWidth(0, gridWidth_);
	const std::uniform_int_distribution<size_t> distributionByHeight(0, gridHeight_);

	while (true) {
		const size_t randomX = distributionByWidth(generator);
		const size_t randomY = distributionByHeight(generator);
		emptyCell.x_ = randomX;
		emptyCell.y_ = randomY;

		const Cell* cell = findCell(emptyCell);

		if (cell && cell->type_ == Cell::Type::empty) {
			break;
		}
	}

	return emptyCell;
}

std::list<std::unique_ptr<Cell>>& Board::grid() noexcept
{
	return grid_;
}

const bool Board::isFood(const utils::Point<std::size_t>& target)
{
	const Cell* cell = findCell(target);

	if (cell->type_ == Cell::Type::food) {
		return true;
	}

	return false;
}

const bool Board::isWall(const utils::Point<std::size_t>& target)
{
	if (target.x_ == walls::leftWall || target.x_ == walls::rightWall) {
		return true;
	}

	if (target.y_ == walls::topWall || target.y_ == walls::bottomWall) {
		return true;
	}

	return false;
}

const bool Board::isSnakeBody(const utils::Point<std::size_t>& target)
{
	const Cell* cell = findCell(target);

	if (cell->type_ == Cell::Type::body) {
		return true;
	}

	return false;
}

void Board::createBoard(std::size_t gridWidth, std::size_t gridHeight)
{
	/*
	x x x x x
	x o o o x
	x o o o x
	x o o o x
	x x x x x
	*/
	
	gridWidth_ = gridWidth;
	gridHeight_ = gridHeight;

	//std::size_t rightWall = gridWidth_ - 1;
	//std::size_t bottomWall = gridHeight_ - 1;
	walls::rightWall = gridWidth_ - 1;
	walls::bottomWall = gridHeight_ - 1;

	for (std::size_t y = 0; y < gridHeight_; ++y) {
		for (std::size_t x = 0; x < gridWidth_; ++x) {
			if (y == walls::topWall) {
				grid_.emplace_back(std::make_unique<Cell>(Cell::Type::wall, x, y));
			} else if (x == walls::leftWall) {
				grid_.emplace_back(std::make_unique<Cell>(Cell::Type::wall, x, y));
			} else if (x == walls::rightWall) {
				grid_.emplace_back(std::make_unique<Cell>(Cell::Type::wall, x, y));
			} else if (y == walls::bottomWall) {
				grid_.emplace_back(std::make_unique<Cell>(Cell::Type::wall, x, y));
			} else {
				Cell* cell = findCell({ x, y });
				if (cell != nullptr) {
					std::cout << "Board::createBoard() duplicate cell" << std::endl;
					assert(cell != nullptr);
				}
				grid_.emplace_back(std::make_unique<Cell>(Cell::Type::empty, x, y));
			}
		}
	}
}


