#pragma once

#include <utility>
#include <cmath>
#include <cstdint>
#include <SDL_pixels.h>
#include "Point.hpp"

class Board;
struct Cell;

namespace utils {
	namespace commonConstants {
		constexpr unsigned int secondAsMilliseconds{ 1000 };
		constexpr unsigned int targetFramesPerSecond{ 60 };
		constexpr uint32_t refreshRateTargetTimeStep{ secondAsMilliseconds / targetFramesPerSecond };
		constexpr uint32_t simulationRefreshRateTargetTimeStep{ secondAsMilliseconds / 30 };
		constexpr unsigned int lowSpeedLimit{ 10 };

		namespace fontSize {
			constexpr unsigned int twenty{ 20 };
		}

		namespace color {
			constexpr SDL_Color black = { 0, 0, 0,255 };
		}
	}
	
	namespace math {
		const double pi{ std::atan(1.0) * 4 };

		Point<long> directionVectorBetweenPoints(const Point<std::size_t> from, const Point<std::size_t> to) noexcept;
		float distanceBetweenPoints(const Point<std::size_t> from, const Point<std::size_t> to) noexcept;
		float snakeAngleToFood(const Point<std::size_t> head, const Point<std::size_t> body, const Point<std::size_t> food) noexcept;
	}
}
