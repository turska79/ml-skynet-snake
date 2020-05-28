#pragma once

#include "State.hpp"

class Game;

class MainMenuState : public State
{
public:
	explicit MainMenuState(Game& game) noexcept;
	virtual ~MainMenuState() {};
	void enter() override;
	void update(Renderer& renderer, uint32_t deltaTime) override;
	void exit() override;
	void handleInput(const Keyboard& keyboard) override;

private:
	FontPtr calibri_;
};

