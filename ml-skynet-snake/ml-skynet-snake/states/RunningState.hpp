#pragma once

#include "State.hpp"
#include "../Food.hpp"

class Renderer;
class Keyboard;
class Game;
class SnakeControl;
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

	void snakeCollisionCallback();

	virtual void enter() override;
	virtual void update(Renderer& renderer) override;
	void exit() override;
	virtual void handleInput(const Keyboard& keyboard) override;

protected:
	void resetBoard() const;
	void initSnake();
	void newRandomPositionForFood();

	void registerCallbacks();
	void unregisterCallbacks();
	void registerCollisionCallback();
	void unregisterCollisionCallback();
	void registerFoodEatenCallback();
	void unregisterFoodEatenCallback();

	SnakeControl& snakeControl_;
	Simulation& simulation_;
	Food food_;
};

