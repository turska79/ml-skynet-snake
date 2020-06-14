#pragma once

#include <cstddef>
#include <SDL.h>

struct Settings
{
	std::size_t windowWidth_{ 0 };
	std::size_t windowHeight_{ 0 };

	std::size_t gridStartOffset_{ 0 };
	std::size_t gridWidth_{ 0 };
	std::size_t gridHeight_{ 0 };

	SDL_Color backGround_{ 211, 211, 211, 255 };
};

