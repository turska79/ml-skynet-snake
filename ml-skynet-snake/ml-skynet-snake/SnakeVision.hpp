#pragma once

#include "utils/Point.hpp"
#include "utils/vec2.hpp"
#include <array>
#include <list>

class Board;
class Renderer;

using FromPoint = const utils::Point<std::size_t>;
using ToPoint = const utils::Point<std::size_t>;
using VisionPoints = std::pair<FromPoint, ToPoint>;

class SnakeVision
{
public:
	std::array<float, 24> lookInAllDirections(Board& board, const utils::Point<std::size_t> fromPosition );
	std::list< VisionPoints> pointsForRendering(Board& board, const utils::Point<std::size_t> fromPosition);
protected:
	std::array<float, 3> lookInDirection(Board& board, const utils::Point<std::size_t> fromPosition, utils::Vec2<int> direction);
	VisionPoints pointsForRenderingDirection(Board& board, const utils::Point<std::size_t> fromPosition, const utils::Vec2<int> direction);
};

