#pragma once

#include "Renderer.hpp"
#include "Board.hpp"
#include "Settings.hpp"
#include "Snake.hpp"
#include "Input.hpp"

class Game
{
public:
	Game(Settings& settings);
	virtual ~Game();

	void run();

private:
	Renderer renderer_;
	Settings settings_;
	Board board_;
	Snake snake_;
	Input input_;
};

