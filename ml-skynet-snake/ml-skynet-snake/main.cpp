#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Game.hpp"

constexpr unsigned int msBetweenUpdates { 17 };

int main(int, char**) {
	constexpr unsigned int success {0};
	constexpr std::size_t windowWidth {860};
	constexpr std::size_t windowHeight {640};

	constexpr std::size_t gridStartOffset{ 220 };
	constexpr std::size_t gridWidth{ 32 };
	constexpr std::size_t gridHeight{ 32 };

	Settings settings;
	settings.windowWidth_ = windowWidth;
	settings.windowHeight_ = windowHeight;
	settings.gridStartOffset_ = gridStartOffset;
	settings.gridWidth_ = gridWidth;
	settings.gridHeight_ = gridHeight;
	settings.backGround_ = {211, 211, 211, 255};
	
	if (SDL_Init(SDL_INIT_VIDEO) != success) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	TTF_Init();

	Game game(settings);
	game.run();
	
	TTF_Quit();
	SDL_Quit();

	return 0;
}

