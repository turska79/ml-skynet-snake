#pragma once

#include <cstddef>
#include <list>
#include <memory>
#include "utils/Point.hpp"

struct Settings;

struct Cell : public utils::Point<std::size_t>
{
	enum class Type { wall, head, body, food, empty } type_{ Type::empty };
	
	Cell(Type type, std::size_t x, std::size_t y) noexcept : type_(type) { x_ = x; y_ = y; };
};

class Board
{
public:
	explicit Board(const Settings& settings);
	
	Cell* findCell(const utils::Point<std::size_t> coordinate);
	Cell* findFood();
	void resetBoard();
	utils::Point<std::size_t> findRandomEmptyCell();
	std::list<std::unique_ptr<Cell>>& grid() noexcept;
	const bool isFood(const utils::Point<std::size_t> target);
	const bool isWall(const utils::Point<std::size_t> target);
	const bool isSnakeBody(const utils::Point<std::size_t> target);
private:
	void createBoard(std::size_t gridWidth_, std::size_t gridHeight);
	std::size_t gridWidth_{ 0 };
	std::size_t gridHeight_{ 0 };
	std::list<std::unique_ptr<Cell>> grid_;
};

