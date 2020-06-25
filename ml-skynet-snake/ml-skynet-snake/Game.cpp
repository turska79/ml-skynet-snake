#include "Game.hpp"
#include "Renderer.hpp"
#include "MainMenuState.hpp"
#include "Timer.hpp"
#include "FontCache.hpp"
#include <SDL.h>
#include <iostream>
#include <string>

FontCache fontCache;
constexpr unsigned int targetFramesPerSecond{ 60 };
constexpr unsigned int secondAsMilliseconds{ 1000 };
constexpr uint32_t targetFrameTime = secondAsMilliseconds / targetFramesPerSecond;
const SDL_Color black = { 0, 0, 0,255 };
constexpr unsigned int fontSize{ 20 };

Game::Game(Settings& settings) :
	settings_(settings),
	board_(settings),
	renderer_(settings.windowWidth_, settings.windowHeight_, settings.gridStartOffset_, settings.backGround_)
{

}
void Game::run()
{
	std::cout << " Game::run()" << std::endl;
	pushState<MainMenuState>(*this);
	runGameLoop();
	exit();
	std::cout << " Game::run() exit" << std::endl;
}

void Game::runGameLoop()
{
	float fps{ 0 };
	fpsTimer_.start();
	capFramesTimer_.start();

	while (running_) {
		gameLoop();

		if (running_ == false) {
			break;
		}
	}
}

void Game::gameLoop()
{
	const uint32_t deltaTime = fpsTimer_.deltaTime();
	float fps{ 1000.0f / deltaTime };
	TTF_Font* font = fontCache.getFont(20);

	handleEvents();
	handleInput();

	std::string fpsTtext{ "FPS: " };
	fpsTtext.append(std::to_string(static_cast<unsigned int>(std::round(fps))));

	std::cout << fpsTtext << std::endl;

	renderer_.renderBackground();
	renderer_.renderCells(board_.grid());
	renderer_.renderText(0, 0, fpsTtext, *font, black);

	currentState()->update(renderer_, deltaTime);

	renderer_.present();

	capFrameRate();
}

void Game::capFrameRate()
{
	const uint32_t frameTicks = capFramesTimer_.deltaTime();

	if (frameTicks < targetFrameTime) {
		SDL_Delay(targetFrameTime - frameTicks);
	}
}

void Game::exit() noexcept
{
	fontCache.clear();
	states_.clear();
}

Board& Game::board() noexcept
{
	return board_;
}

Renderer& Game::renderer() noexcept
{
	return renderer_;
}

void Game::handleEvents()
{
	SDL_Event event{ 0 };

	const auto eventsInQueue = SDL_PollEvent(nullptr);
	constexpr int queueEmpty{ 0 };

	if (eventsInQueue != queueEmpty) {
		SDL_PollEvent(&event);
		input_.update();

		if (event.type == SDL_QUIT || checkForQuit()) {
			running_ = false;
		}
	}
}

void Game::handleInput()
{
	const auto& keyboard = input_.getKeyboard();
	currentState()->handleInput(keyboard);
}

State* Game::currentState() const
{
	if (states_.empty() == false) {
		return states_.back().get();
	}

	return nullptr;
}

const bool Game::checkForQuit() const
{
	const auto& keyboard = input_.getKeyboard();
	bool running = false;

	if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_ESCAPE) == keyboard::ButtonState::pressed) {
		running = true;
	}

	return running;
}


