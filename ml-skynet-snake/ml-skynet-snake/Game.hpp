#pragma once

#include "Renderer.hpp"
#include "Board.hpp"
#include "Settings.hpp"
#include "Input.hpp"
#include "Simulation.hpp"
#include "states/GameStates.hpp"
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
	State* currentState() const;
private:
	void runGameLoop();
public:
	void gameLoop();
	void renderBoard();
private:
	void printCurrentScoreToScreen();
	void printFpsRateToScreen();
	void handleEvents();
	void handleInput();
	
	const bool checkForQuit() const;
	
	bool running_{ true };
	uint32_t nextGameStep{ 0 };
	uint32_t lastRender_{ 0 };

	Renderer renderer_;
	Settings settings_;
	Board board_;
	Input input_;
	Simulation simulation_;
	Snake snake_;
};

