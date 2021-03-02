#pragma once

#include "Renderer.hpp"
#include "Board.hpp"
#include "Settings.hpp"
#include "Input.hpp"
#include "simulation/Simulation.hpp"
#include "states/BaseState.hpp"
#include "states/GameStates.hpp"
#include "Snake.hpp"
#include <vector>
#include <memory>

class GameState;

class Game : public gamestates::handling::StateStorage<gamestates::state::BaseState>
{
public:
	explicit Game(Settings& settings);

	void run();
	void exit() noexcept;
	
	Board& board() noexcept;
	Renderer& renderer() noexcept;
	Simulation& simulation() noexcept;
	Snake& snake() noexcept;
	gamestates::state::BaseState* currentState();
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
	
	bool checkForQuit() const;
	
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

