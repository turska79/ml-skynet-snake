#include "Game.hpp"
#include "Renderer.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <iostream>


class FpsCounter {
public:
	void start() {
		counter = 0;
		startTime = SDL_GetTicks();
	}
	
	void update() {
		uint32_t currentTime = SDL_GetTicks();
		uint32_t elapsed = currentTime - startTime;
		++counter;

		if (elapsed >= second) {
			text = "FPS: " + std::to_string(counter);
			start();
		}
	}

	uint32_t startTime{ 0 };
	uint32_t elapsedTime{ 0 };
	unsigned int counter{ 0 };
	const unsigned int second{ 1000 };
	std::string text{ "FPS: " };
};

Game::Game(Settings& settings) :
	settings_(settings),
	board_(settings),
	renderer_(settings.windowWidth_, settings.windowHeight_, settings.backGround_)
{

}


Game::~Game()
{

}



void Game::run()
{
	const unsigned int simulationFrequency{ 100 };
	const unsigned int framesPerSecond{ 60 };
	const unsigned int secondAsMilliseconds{ 1000 };
	bool running = true;
	FpsCounter fpsCounter;
	fpsCounter.start();

	TTF_Font* calibri = TTF_OpenFont("C:\\WINDOWS\\Fonts\\Calibrib.ttf", 20);
	SDL_Color black = { 0, 0, 0,255 };

	Point<std::size_t> initialPosition;
	initialPosition.x_ = 10;
	initialPosition.y_ = 10;

	snake_.init(initialPosition, board_);

	uint32_t previous = SDL_GetTicks();
	double lag{ 0 };

	while (running) {
		uint32_t current = SDL_GetTicks();
		uint32_t elapsed = current - previous;

		previous = current;
		lag += elapsed;

		SDL_Event event;
		
		//if (SDL_PollEvent(nullptr) == 1 && SDL_PollEvent(&event)) {
		auto eventsInQueue = SDL_PollEvent(nullptr);
		constexpr int queueEmpty{ 0 };

		if (eventsInQueue != queueEmpty) {
			SDL_PollEvent(&event);

			if (event.type == SDL_QUIT) {
				running = false;
				break;
			}
			input_.update();

			const auto& keyboard = input_.getKeyboard();

			if (keyboard.getKeyState(SDL_Scancode::SDL_SCANCODE_ESCAPE) == keyboard::ButtonState::pressed) {
				running = false;
			}

			snake_.processInput(keyboard);
		}

			
		while (lag >= simulationFrequency) {
			std::cout << "run simulation, lag: " << std::to_string(lag);
			//snake_.updatePosition(board_);
			const Point<std::size_t> target = simulation_.getNextSnakePosition(board_, snake_);
			simulation_.checkForCollision(board_, target);
			simulation_.updateSnakePosition(board_, snake_, target);
			lag -= simulationFrequency;
		}

		renderer_.renderBackground();
		renderer_.renderCells(board_.grid_);
		
		fpsCounter.update();
		renderer_.renderText(20, 20, fpsCounter.text, *calibri, black);
		renderer_.present();

		uint32_t deltatime = SDL_GetTicks() - previous;

		if (deltatime < (secondAsMilliseconds / framesPerSecond)) {
			SDL_Delay((secondAsMilliseconds / framesPerSecond) - deltatime);
		}
	}
}
