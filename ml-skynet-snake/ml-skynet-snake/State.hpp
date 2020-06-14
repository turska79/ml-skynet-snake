#pragma once

#include <cstdint>
//#include <SDL_ttf.h>
#include <memory>

class Keyboard;
class Renderer;
class Game;

//using FontPtr = std::unique_ptr<TTF_Font, std::integral_constant<decltype(&TTF_CloseFont), &TTF_CloseFont>>;

class State
{
public:
	State(Game& game) noexcept : game_(game) { };

	virtual void enter() = 0;
	virtual void update(Renderer& renderer, uint32_t deltaTime) = 0;
	virtual void exit() = 0;
	virtual void handleInput(const Keyboard& keyboard) = 0;

protected:
	Game& game_;
};