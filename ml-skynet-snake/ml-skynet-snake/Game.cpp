#include "Game.hpp"
#include "Renderer.hpp"
#include "MainMenuState.hpp"
#include "Timer.hpp"
#include "FontCache.hpp"
#include <SDL.h>
#include <iostream>
#include <string>

FontCache fontCache;

Game::Game(Settings& settings) :
	settings_(settings),
	board_(settings),
	renderer_(settings.windowWidth_, settings.windowHeight_, settings.gridStartOffset_, settings.backGround_)
{

}
void Game::run()
{
	std::cout << " Game::run()" << std::endl;
	Timer fpsTimer;
	Timer capFramesTimer;
	
	const SDL_Color black = { 0, 0, 0,255 };
	uint32_t currentTime = SDL_GetTicks();
	constexpr unsigned int fontSize{ 20 };
	constexpr unsigned int targetFramesPerSecond{ 60 };
	constexpr unsigned int secondAsMilliseconds{ 1000 };

	TTF_Font* font = fontCache.getFont(20);

	pushState<MainMenuState>(*this);
	
	const uint32_t targetFrameTime = secondAsMilliseconds / targetFramesPerSecond;
	uint32_t countedFrames{ 0 };
	
	fpsTimer.start();

	while (running_) {
		capFramesTimer.start();

		const uint32_t newTime = SDL_GetTicks();
		const uint32_t deltaTime = newTime - currentTime;
		currentTime = newTime;

		handleEvents();

		if (running_ == false) {
			break;
		}
		
		handleInput();
		
		float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
		if (avgFPS > 2000000) {
			avgFPS = 0;
		}

		std::string fpsTtext{"FPS: "};
		fpsTtext.append(std::to_string(static_cast<unsigned int>(std::round(avgFPS))));

		renderer_.renderBackground();
		renderer_.renderCells(board_.grid());
		renderer_.renderText(0, 0, fpsTtext, *font, black);

		currentState()->update(renderer_, deltaTime);

		renderer_.present();
		
		++countedFrames;
		const uint32_t frameTicks = capFramesTimer.getTicks();
		
		if (frameTicks < targetFrameTime) {
			SDL_Delay(targetFrameTime - frameTicks);
		}
	}

	exit();
	std::cout << " Game::run() exit" << std::endl;
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
