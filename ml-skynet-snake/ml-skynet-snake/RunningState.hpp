#pragma once

#include "State.hpp"
#include "Snake.hpp"
#include "Simulation.hpp"

class Renderer;
class Keyboard;
class Game;

class RunningState : public State
{
public:
	RunningState(Game& game);
	virtual ~RunningState();

	virtual void enter() override;
	virtual void update(Renderer& renderer, uint32_t deltaTime) override;
	virtual void exit() override;
	virtual void handleInput(const Keyboard& keyboard) override;
private:
	void resetBoard() const;
	void initSnake();
	Snake snake_;
	Simulation simulation_;
	
	uint32_t updateDeltaTime_{ 0 };
};

