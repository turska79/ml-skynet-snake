#pragma once

#include <utility>
#include <cmath>
#include "Point.hpp"

class Board;
struct Cell;

namespace utils {
	namespace commonConstants {
		constexpr unsigned int secondAsMilliseconds{ 1000 };
		constexpr unsigned int fontSize{ 20 };
	}
	
	namespace math {
		const double pi = std::atan(1.0) * 4;

		Point<long> directionVectorBetweenPoints(const Point<std::size_t> from, const Point<std::size_t> to) noexcept;
		float distanceBetweenPoints(const Point<std::size_t> from, const Point<std::size_t> to) noexcept;
		float snakeAngleToFood(const Point<std::size_t> head, const Point<std::size_t> body, const Point<std::size_t> food) noexcept;
	}

	/*namespace training {
		void generateData(Board& board, unsigned int games);
		using SnakePosition = std::pair<Cell*, Cell*>;
		using Positions = std::pair<SnakePosition, Cell*>;

		Positions setStartingPositions(Board& board);

		void writeData(const bool leftBlocked, const bool frontBlocked, const bool rightBlocked, const float foodAngle, const float snakeDirection);
		void writeData(const bool left, const bool front, const bool right);
	}*/
}
