#pragma once

#include "State.hpp"
//#include "Snake.hpp"
//#include "Simulation.hpp"
#include "Food.hpp"

class Renderer;
class Keyboard;
class Game;
class Snake;
class Simulation;

class RunningState : public State
{
public:
	explicit RunningState(Game& game);
	virtual ~RunningState() = default;
	RunningState(const RunningState&) = default;
	RunningState& operator=(const RunningState&) = default;
	RunningState(RunningState&&) = default;
	RunningState& operator=(RunningState&&) = default;

	virtual void enter() override;
	virtual void update(Renderer& renderer) override;
	void exit() override;
	virtual void handleInput(const Keyboard& keyboard) override;

protected:
	void resetBoard() const;
	void initSnake();
	void initFood();
	void newRandomPositionForFood();
	void printCurrentScoreToScreen(Renderer& renderer);

	Snake& snake_;
	Simulation& simulation_;
	Food food_;
//	uint32_t updateDeltaTime_{ 0 };
};

