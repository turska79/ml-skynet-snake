#include "Utils.hpp"
#include "Board.hpp"
#include <iostream>
#include <fstream>
#include <string>

Point<long> utils::math::directionVectorBetweenPoints(const Point<std::size_t> from, const Point<std::size_t> to) noexcept
{
	return Point<long>(to.x_ - from.x_, to.y_ - from.y_);
}

float utils::math::distanceBetweenPoints(const Point<std::size_t> from, const Point<std::size_t> to) noexcept
{
	Point<long> vector = directionVectorBetweenPoints(from, to);

	double distance = sqrt((vector.x_ * vector.x_) + (vector.y_ * vector.y_));
	return static_cast<float>(distance);
}

float utils::math::snakeAngleToFood(const Point<std::size_t> body, const Point<std::size_t> head, const Point<std::size_t> food) noexcept
{
	Point<long> foodDirectionVector = directionVectorBetweenPoints(food, head);
	Point<long> snakeDirectionVector = directionVectorBetweenPoints(head, body);

	double angle = atan2(snakeDirectionVector.x_ * foodDirectionVector.y_ - snakeDirectionVector.y_ * foodDirectionVector.x_, snakeDirectionVector.x_ * foodDirectionVector.x_ + snakeDirectionVector.y_ * foodDirectionVector.y_);

	return static_cast<float>(angle * (180.0 / pi));
}

void utils::training::generateData(Board& board, unsigned int games)
{
	std::ofstream fs;
	
	std::string filename = "training_data.csv";

	fs.open(filename, std::ios::out | std::ios::app);

	for (unsigned int game = 0; game < games; ++game) {
		Positions positions = setStartingPositions(board);

		for (unsigned int step = 0; step < 1; ++step) {
			const SnakePosition snake = positions.first;

			const Point<std::size_t> snakeHeadPosition(snake.first->x_, snake.first->y_);
			const Point<std::size_t> snakeBodyPosition(snake.second->x_, snake.second->y_);
			const Point<std::size_t> foodPosition(positions.second->x_, positions.second->y_);

			float distance = utils::math::distanceBetweenPoints(snakeHeadPosition, foodPosition);
			float angle = utils::math::snakeAngleToFood(snakeBodyPosition, snakeHeadPosition, foodPosition);
		}
	}
	fs.close();
}

std::pair<std::pair<Cell*, Cell*>, Cell*> utils::training::setStartingPositions(Board& board)
{
	board.resetBoard();

	Point<std::size_t> foodPosition = board.findRandomEmptyCell();
	Cell* foodCell = board.findCell(foodPosition);
	foodCell->type_ = Cell::Type::food;

	Point<std::size_t> snakeHeadPosition = board.findRandomEmptyCell();
	Cell* snakeHead = board.findCell(snakeHeadPosition);
	snakeHead->type_ = Cell::Type::head;

	Point<std::size_t> snakeBodyPosition(snakeHeadPosition);
	snakeBodyPosition.x_ -= 1;
	Cell* snakeBody = board.findCell(snakeBodyPosition);
	snakeHead->type_ = Cell::Type::head;

	return std::make_pair(std::make_pair(snakeHead, snakeBody), foodCell);
}

void utils::training::writeData(const bool leftBlocked, const bool frontBlocked, const bool rightBlocked, const float foodAngle, const float snakeDirection)
{
	std::ofstream fs;
	std::string filename = "training_data_input.csv";

	fs.open(filename, std::ios::out | std::ios::app);
	fs << leftBlocked << "," << frontBlocked << "," << rightBlocked << "," << foodAngle / 360 << "," << snakeDirection << std::endl;
	fs.close();
}

void utils::training::writeData(const bool left, const bool front, const bool right)
{
	std::ofstream fs;
	std::string filename = "training_data_output.csv";

	fs.open(filename, std::ios::out | std::ios::app);
	fs << left << "," << front << "," << right << std::endl;
	fs.close();
}
