#pragma once

#include "Renderer.hpp"
#include "Board.hpp"
#include "Settings.hpp"
#include "Input.hpp"
#include "Simulation.hpp"
#include "GameStates.h"
#include "Timer.hpp"
#include "Snake.hpp"
#include <vector>
#include <memory>

class State;

class Game : public GameStates
{
public:
	explicit Game(Settings& settings);

	void run();
	void exit() noexcept;
	
	Board& board() noexcept;
	Renderer& renderer() noexcept;
	Simulation& simulation() noexcept;
	Snake& snake() noexcept;
private:
	void runGameLoop();
public:
	void gameLoop();
	void renderBoard();
private:
	void capFrameRate();
	void printCurrentScoreToScreen();
	void handleEvents();
	void handleInput();
	State* currentState() const;
	const bool checkForQuit() const;
	bool running_{ true };
	Timer fpsTimer_;
	Timer capFramesTimer_;

	Renderer renderer_;
	Settings settings_;
	Board board_;
	Input input_;
	Simulation simulation_;
	Snake snake_;
};

