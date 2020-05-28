#pragma once

#include "Renderer.hpp"
#include "Board.hpp"
#include "Settings.hpp"
#include "Input.hpp"
#include "Simulation.hpp"
#include "GameStates.h"
#include <vector>
#include <memory>

class State;

class Game : public GameStates
{
public:
	explicit Game(Settings& settings);

	void run();
	void exit();
	
	Board& board();
private:
	void handleEvents();
	void handleInput();
	State* currentState() const;
	const bool checkForQuit() const;

	bool running_{ true };
	Renderer renderer_;
	Settings settings_;
	Board board_;
	Input input_;
};

