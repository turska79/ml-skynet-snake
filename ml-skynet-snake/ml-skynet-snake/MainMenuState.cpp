#include "MainMenuState.hpp"
//#include "Input.hpp"
//#include "Renderer.hpp"
#include "Game.hpp"
#include <SDL_ttf.h>
#include "RunningState.hpp"

MainMenuState::MainMenuState(Game& game) : State(game)
{
}

void MainMenuState::enter()
{
	constexpr unsigned int fontSize{ 20 };

	FontPtr font(TTF_OpenFont("C:\\WINDOWS\\Fonts\\Calibrib.ttf", fontSize));
	calibri_ = std::move(font);
}

void MainMenuState::update(Renderer& renderer, uint32_t deltaTime)
{
	SDL_Color black = { 0, 0, 0, 255 };
	std::string text{ "Press Space bar to start" };

	renderer.renderText(20, 40, text, *calibri_, black);
}

void MainMenuState::exit()
{
}

void MainMenuState::handleInput(const Keyboard& keyboard)
{
	if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_SPACE) == keyboard::ButtonState::pressed) {
		game_.pushState<RunningState>(game_);
	}
}