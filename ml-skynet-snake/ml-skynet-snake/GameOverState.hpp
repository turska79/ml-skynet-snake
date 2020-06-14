#pragma once

#include "State.hpp"

class Game;

class GameOverState : public State
{
public:
	explicit GameOverState(Game& game) noexcept;
	virtual ~GameOverState() = default;
	GameOverState(const GameOverState&) = default;
	GameOverState& operator=(const GameOverState&) = default;
	GameOverState(GameOverState&&) = default;
	GameOverState& operator=(GameOverState&&) = default;

	void enter() override;
	void update(Renderer& renderer, uint32_t deltaTime) override;
	void exit() override;
	void handleInput(const Keyboard& keyboard) override;

private:

};

