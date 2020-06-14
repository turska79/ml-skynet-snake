#pragma once

#include "State.hpp"
#include "Snake.hpp"
#include "Simulation.hpp"
#include "Food.hpp"

class Renderer;
class Keyboard;
class Game;

class RunningState : public State
{
public:
	explicit RunningState(Game& game);
	virtual ~RunningState() = default;
	RunningState(const RunningState&) = default;
	RunningState& operator=(const RunningState&) = default;
	RunningState(RunningState&&) = default;
	RunningState& operator=(RunningState&&) = default;

	void enter() override;
	void update(Renderer& renderer, uint32_t deltaTime) override;
	void exit() override;
	void handleInput(const Keyboard& keyboard) override;

private:
	void resetBoard() const;
	void initSnake();
	void initFood();
	void newRandomPositionForFood();

	Snake snake_;
	Simulation simulation_;
	Food food_;
	uint32_t updateDeltaTime_{ 0 };
};

