#include "MainMenuState.hpp"
#include "../Game.hpp"
#include "RunningState.hpp"
#include "RunningStateAI.hpp"
#include "../utils/FontCache.hpp"
#include "../utils/Utils.hpp"
#include <iostream>

extern FontUtils::FontCache fontCache;

gamestates::state::MainMenuState::MainMenuState(Game& game) noexcept : gamestates::state::BaseState(game)
{

}

void gamestates::state::MainMenuState::enter()
{
	std::cout << " MainMenuState::enter()" << std::endl;
}

void gamestates::state::MainMenuState::update(Renderer& renderer)
{
	constexpr unsigned int x{ 340 };
	constexpr unsigned int y{ 300 };
	std::string text{ "Press Space bar to start" };

	renderer.renderText(x, y, text, *fontCache.getFont(utils::commonConstants::fontSize::twenty), utils::commonConstants::color::black);
}

void gamestates::state::MainMenuState::exit()
{
	std::cout << " MainMenuState::exit()" << std::endl;
}

void gamestates::state::MainMenuState::handleInput(const Keyboard& keyboard)
{
	if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_SPACE) == keyboard::ButtonState::pressed) {
		game_.nextState<RunningState>(game_);
	} else if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_A) == keyboard::ButtonState::pressed) {
		game_.nextState<RunningStateAI>(game_);
	}
}