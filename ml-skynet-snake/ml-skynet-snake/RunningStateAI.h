#pragma once

#include "RunningState.hpp"

class RunningStateAI : public RunningState
{
public:
	explicit RunningStateAI(Game& game);
	virtual ~RunningStateAI() = default;
	RunningStateAI(const RunningStateAI&) = default;
	RunningStateAI& operator=(const RunningStateAI&) = default;
	RunningStateAI(RunningStateAI&&) = default;
	RunningStateAI& operator=(RunningStateAI&&) = default;

	void enter() override;
	void update(Renderer& renderer) override;
	void handleInput(const Keyboard& keyboard) override;
private:
	bool running_{ false };
};

