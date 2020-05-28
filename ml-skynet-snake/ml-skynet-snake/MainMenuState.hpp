#pragma once

#include "State.hpp"

class Game;

class MainMenuState : public State
{
public:
	explicit MainMenuState(Game& game);

	virtual void enter() override;
	virtual void update(Renderer& renderer, uint32_t deltaTime) override;
	virtual void exit() override;
	virtual void handleInput(const Keyboard& keyboard) override;

private:
	FontPtr calibri_;
};

