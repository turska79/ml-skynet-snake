#include "Game.hpp"
#include "Renderer.hpp"
#include "MainMenuState.hpp"
#include "Timer.hpp"
#include "FontCache.hpp"
#include <SDL.h>
#include <iostream>
#include <string>

#include <chrono>
/*
void Spin(float milliseconds)
{
	milliseconds /= 1000.0f;
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	double ms = 0;
	while (ms < milliseconds)
	{
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
		ms = time_span.count();
	}
}*/

FontCache fontCache;
constexpr unsigned int targetFramesPerSecond{ 60 };
constexpr unsigned int secondAsMilliseconds{ 1000 };
constexpr uint32_t targetFrameTime = secondAsMilliseconds / targetFramesPerSecond;
const SDL_Color black = { 0, 0, 0,255 };
constexpr unsigned int fontSize{ 20 };

Game::Game(Settings& settings) :
	settings_(settings),
	board_(settings),
	renderer_(settings.windowWidth_, settings.windowHeight_, settings.gridStartOffset_, settings.backGround_),
	simulation_(board_),
	snake_(board_)
{
	JobSystem::Initialize();
}
void Game::run()
{
	std::cout << " Game::run()" << std::endl;

	//JobSystem::Execute([] { Spin(0); });

	pushState<MainMenuState>(*this);
	runGameLoop();
	exit();
	std::cout << " Game::run() exit" << std::endl;
}

void Game::runGameLoop()
{
	fpsTimer_.start();
	
	while (running_) {
		capFramesTimer_.start();
		gameLoop();

		if (running_ == false) {
			break;
		}
	}
}

void Game::gameLoop()
{
	handleEvents();
	handleInput();

	//simulation_.update((SnakeMovement&)snake_);
	JobSystem::Execute([this] { simulation_.update((SnakeMovement&)snake_); });
	JobSystem::Execute([this] { renderBoard(); });

	//renderBoard();
	JobSystem::Wait();
	currentState()->update(renderer_);

	renderer_.present();

	capFrameRate();
}

void Game::renderBoard()
{
	const uint32_t deltaTime = fpsTimer_.deltaTime();
	float fps{ 1000.0f / deltaTime };
	TTF_Font* font = fontCache.getFont(20);

	std::string fpsTtext{ "FPS: " };
	fpsTtext.append(std::to_string(static_cast<unsigned int>(std::round(fps))));

	renderer_.renderBackground();
	renderer_.renderCells(board_.grid());
	renderer_.renderText(0, 0, fpsTtext, *font, black);
}

void Game::capFrameRate()
{
	const uint32_t frameTicks = capFramesTimer_.getTicksSinceStart();

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


