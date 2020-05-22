#include "GameOverState.hpp"
#include "Game.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

GameOverState::GameOverState(Game& game) : State(game)
{
}

void GameOverState::enter()
{
	std::cout << " GameOverState::enter()" << std::endl;
	constexpr unsigned int fontSize{ 20 };

	FontPtr font(TTF_OpenFont("C:\\WINDOWS\\Fonts\\Calibrib.ttf", fontSize));
	calibri_ = std::move(font);
}

void GameOverState::update(Renderer& renderer, uint32_t deltaTime)
{
	SDL_Color black = { 0, 0, 0, 255 };
	std::string text{ "Game over - Press Space bar to start" };

	renderer.renderText(20, 40, text, *calibri_, black);
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
