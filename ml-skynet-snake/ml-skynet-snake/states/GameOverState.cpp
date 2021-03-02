#include "GameOverState.hpp"
#include "../Game.hpp"
#include "../utils/FontCache.hpp"
#include "../utils/Utils.hpp"
#include <iostream>

extern FontUtils::FontCache fontCache;

gamestates::state::GameOverState::GameOverState(Game& game) noexcept : gamestates::state::BaseState(game)
{

}

void gamestates::state::GameOverState::enter()
{
	std::cout << " GameOverState::enter()" << std::endl;
}

void gamestates::state::GameOverState::update(Renderer& renderer)
{
	//SDL_Color black = { 0, 0, 0, 255 };
	constexpr unsigned int x{ 340 };
	constexpr unsigned int y{ 300 };

	std::string text{ "Game over - Press Space bar to start" };

	renderer.renderText(x, y, text, *fontCache.getFont(utils::commonConstants::fontSize::twenty), utils::commonConstants::color::black);
}

void gamestates::state::GameOverState::exit()
{
	std::cout << " GameOverState::exit()" << std::endl;
}

void gamestates::state::GameOverState::handleInput(const Keyboard& keyboard)
{
	if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_SPACE) == keyboard::ButtonState::pressed) {
		game_.popState();
	}
}
