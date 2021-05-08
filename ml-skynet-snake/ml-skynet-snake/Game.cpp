#include "Game.hpp"
#include "Renderer.hpp"
#include "states/MainMenuState.hpp"
#include "utils/FontCache.hpp"
#include "utils/Utils.hpp"

#pragma warning(push)  
#pragma warning(disable : 26819 26812)
#include <SDL.h>
#pragma warning( pop )

#include <iostream>
#include <string>

FontUtils::FontCache fontCache;

Game::Game(Settings& settings) :
	settings_(settings),
	renderer_(settings.windowWidth_, settings.windowHeight_, settings.gridStartOffset_, settings.backGround_),
	board_(settings),
	simulation_(board_, snake_),
	snake_(board_)
{
	simulation_.addObject(snake_);
}
void Game::run()
{
	nextState<gamestates::state::MainMenuState>(*this);
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
	changeState();

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
	constexpr unsigned int snakeInitialLentgh{ 2 };
	std::string score = "Score: ";

	const unsigned int len = snake_.length() > 0 ? snake_.length() - snakeInitialLentgh : 0;

	score.append(std::to_string(len));
	constexpr unsigned int x{ 0 };
	constexpr unsigned int y{ 40 };
	renderer_.renderText(x, y, score, *fontCache.getFont(utils::commonConstants::fontSize::twenty), utils::commonConstants::color::black);
}

void Game::printFpsRateToScreen()
{
	uint32_t now{ SDL_GetTicks() };
	TTF_Font* font = fontCache.getFont(20);

	uint32_t delta = now - lastRender_;

	float fps = { 1000.0f / delta };

	std::string fpsTtext{ "FPS: " };
	fpsTtext.append(std::to_string(static_cast<unsigned int>(std::round(fps))));
	std::string simulationFpsText("Simulation rate: ");

	const uint32_t simulationRate = simulation_.updateRate();
	simulationFpsText.append(std::to_string(static_cast<unsigned int>(simulationRate)));
	renderer_.renderText(0, 0, fpsTtext, *font, utils::commonConstants::color::black);
	renderer_.renderText(0, 20, simulationFpsText, *font, utils::commonConstants::color::black);
	
	lastRender_ = now;
}

void Game::exit() noexcept
{
	fontCache.clear();
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

bool Game::checkForQuit() const
{
	const auto& keyboard = input_.getKeyboard();
	bool quit{ false };

	if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_ESCAPE) == keyboard::ButtonState::pressed) {
		quit = true;
	}

	return quit;
}


