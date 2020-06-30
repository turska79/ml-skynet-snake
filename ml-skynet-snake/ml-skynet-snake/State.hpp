#pragma once

#include <cstdint>
#include <memory>

class Keyboard;
class Renderer;
class Game;

class State
{
public:
	State(Game& game) noexcept : game_(game) { };

	virtual void enter() = 0;
	virtual void update(Renderer& renderer) = 0;
	virtual void exit() = 0;
	virtual void handleInput(const Keyboard& keyboard) = 0;

protected:
	Game& game_;
};