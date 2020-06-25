#include "MainMenuState.hpp"
#include "Game.hpp"
#include "RunningState.hpp"
#include "RunningStateAI.h"
#include "FontCache.hpp"
#include <iostream>

extern FontCache fontCache;
constexpr unsigned int fontSize{ 20 };

MainMenuState::MainMenuState(Game& game) noexcept : State(game)
{

}

void MainMenuState::enter()
{
	std::cout << " MainMenuState::enter()" << std::endl;
}

void MainMenuState::update(Renderer& renderer, uint32_t deltaTime)
{
	SDL_Color black = { 0, 0, 0, 255 };
	constexpr unsigned int x{ 340 };
	constexpr unsigned int y{ 300 };
	std::string text{ "Press Space bar to start" };

	renderer.renderText(x, y, text, *fontCache.getFont(fontSize), black);
}

void MainMenuState::exit()
{
	std::cout << " MainMenuState::exit()" << std::endl;
}

void MainMenuState::handleInput(const Keyboard& keyboard)
{
	if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_SPACE) == keyboard::ButtonState::pressed) {
		game_.pushState<RunningState>(game_);
	} else if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_A) == keyboard::ButtonState::pressed) {
		game_.pushState<RunningStateAI>(game_);
	}
		
}