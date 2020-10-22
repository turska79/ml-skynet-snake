#pragma once

#include "utils/vec2.hpp"
#include <array>
#include <list>

class Board;
class Simulation;
class Renderer;

using FromPoint = const utils::Point<std::size_t>;
using ToPoint = const utils::Point<std::size_t>;
using VisionPoints = std::pair<FromPoint, ToPoint>;

class SnakeVision
{
public:
	std::array<float, 24> lookInAllDirections(const Board& board, const utils::Point<std::size_t>& fromPosition, const Simulation& simulation, Renderer& renderer);
	std::array<float, 3> lookInDirection(const Board& board, const utils::Point<std::size_t>& fromPosition, const Simulation& simulation, Renderer& renderer, utils::Vec2<int> direction);
	
	std::list< VisionPoints> pointsForRendering(const Board& board, const utils::Point<std::size_t>& fromPosition, const Simulation& simulation);
	VisionPoints pointsForRenderingDirection(const Board& board, const utils::Point<std::size_t>& fromPosition, const Simulation& simulation, const utils::Vec2<int> direction);
};

