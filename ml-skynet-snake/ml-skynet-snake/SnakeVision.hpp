#pragma once

#include "vec2.hpp"
#include <array>

class Board;
class Simulation;
class Game;
class Renderer;

class SnakeVision
{
public:
	std::array<float, 24> lookInAllDirections(const Board& board, const Point<std::size_t>& fromPosition, const Simulation& simulation, Renderer& renderer);
	std::array<float, 3> lookInDirection(const Board& board, const Point<std::size_t>& fromPosition, const Simulation& simulation, Renderer& renderer, Vec2<int> direction);
};

