#include "Game.hpp"
#include "Renderer.hpp"
#include "states/MainMenuState.hpp"
#include "FontCache.hpp"
#include "utils/Utils.hpp"
#include <SDL.h>
#include <iostream>
#include <string>

FontCache fontCache;

Game::Game(Settings& settings) :
	settings_(settings),
	board_(settings),
	renderer_(settings.windowWidth_, settings.windowHeight_, settings.gridStartOffset_, settings.backGround_),
	simulation_(board_, snake_),
	snake_(board_)
{
	simulation_.addObject(snake_);
}
void Game::run()
{
	pushState<MainMenuState>(*this);
	runGameLoop();
	exit();
}

void Game::runGameLoop()
{
	nextGameStep = SDL_GetTicks();

	while (running_) {
		uint32_t now = SDL_GetTicks();

		if (nextGameStep <= now) {
			unsigned int computer_is_too_slow_limit = utils::commonConstants::lowSpeedLimit;

			while ((nextGameStep <= now) && (computer_is_too_slow_limit--)) {
				gameLoop();
				nextGameStep += utils::commonConstants::refreshRateTargetTimeStep;
			}
		} else {
			SDL_Delay(nextGameStep - now);
		}
	}
}

void Game::gameLoop()
{
	handleEvents();
	handleInput();

	renderBoard();

	currentState()->update(renderer_);
	
	printCurrentScoreToScreen();
	printFpsRateToScreen();
	renderer_.present();
}

void Game::renderBoard()
{
	renderer_.renderBackground();
	renderer_.renderCells(board_.grid());
}

void Game::printCurrentScoreToScreen()
{
	std::string score = "Score: ";
	score.append(std::to_string(snake_.length()));
	constexpr unsigned int x{ 0 };
	constexpr unsigned int y{ 40 };
	renderer_.renderText(x, y, score, *fontCache.getFont(utils::commonConstants::fontSize::twenty), utils::commonConstants::color::black);
}

void Game::printFpsRateToScreen()
{
	uint32_t now{ SDL_GetTicks() };

	uint32_t delta = now - lastRender_;

	float fps = { 1000.0f / delta };
	TTF_Font* font = fontCache.getFont(20);

	std::string fpsTtext{ "FPS: " };
	fpsTtext.append(std::to_string(static_cast<unsigned int>(std::round(fps))));
	
	fpsTtext.append(" / Simulation rate: ");
	const uint32_t simulationRate = simulation_.updateRate();
	fpsTtext.append(std::to_string(static_cast<unsigned int>(simulationRate)));
	renderer_.renderText(0, 0, fpsTtext, *font, utils::commonConstants::color::black);
	
	lastRender_ = now;
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

Simulation& Game::simulation() noexcept
{
	return simulation_;
}

Snake& Game::snake() noexcept
{
	return snake_;
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
			simulation_.stop();
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


