#include "GameOverState.hpp"
#include "Game.hpp"
#include "FontCache.hpp"
#include <SDL.h>
#include <iostream>

extern FontCache fontCache;
constexpr unsigned int fontSize{ 20 };

GameOverState::GameOverState(Game& game) noexcept : State(game)
{

}

void GameOverState::enter()
{
	std::cout << " GameOverState::enter()" << std::endl;
}

void GameOverState::update(Renderer& renderer)
{
	SDL_Color black = { 0, 0, 0, 255 };
	constexpr unsigned int x{ 340 };
	constexpr unsigned int y{ 300 };

	std::string text{ "Game over - Press Space bar to start" };

	renderer.renderText(x, y, text, *fontCache.getFont(fontSize), black);
}

void GameOverState::exit()
{
	std::cout << " GameOverState::exit()" << std::endl;
}

void GameOverState::handleInput(const Keyboard& keyboard)
{
	if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_SPACE) == keyboard::ButtonState::pressed) {
		game_.popState();
	}
}
