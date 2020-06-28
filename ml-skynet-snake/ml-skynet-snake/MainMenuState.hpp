#pragma once

#include "State.hpp"

class Game;

class MainMenuState : public State
{
public:
	explicit MainMenuState(Game& game) noexcept;
	virtual ~MainMenuState() = default;
	MainMenuState(const MainMenuState&) = default;
	MainMenuState& operator=(const MainMenuState&) = default;
	MainMenuState(MainMenuState&&) = default;
	MainMenuState& operator=(MainMenuState&&) = default;

	void enter() override;
	void update(Renderer& renderer) override;
	void exit() override;
	void handleInput(const Keyboard& keyboard) override;

private:
	
};

