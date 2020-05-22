#pragma once

#include "Renderer.hpp"
#include "Board.hpp"
#include "Settings.hpp"
#include "Input.hpp"
#include "Simulation.hpp"
//#include "State.hpp"
#include "GameStates.h"
#include <vector>
#include <memory>

class State;

class Game : public GameStates
{
public:
	Game(Settings& settings);

	//void pushState(std::unique_ptr<State> state);
	//void popState();

	void run();
	void exit();
	
	Board& board();
private:
	void handleEvents();
	void handleInput();
	State* currentState() const;
	const bool checkForQuit() const;

	bool running_{ false };
	Renderer renderer_;
	Settings settings_;
	Board board_;
	//Snake snake_;
	Input input_;
	//Simulation simulation_;
	
	//std::vector<std::unique_ptr<State>> states_;
};

