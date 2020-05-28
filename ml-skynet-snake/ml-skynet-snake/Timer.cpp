#include "Timer.hpp"
#include <SDL.h>

void Timer::start() noexcept
{
	started = true;

	paused = false;

	startTicks = SDL_GetTicks();
	pausedTicks = 0;
}

void Timer::stop() noexcept
{
	started = false;

	paused = false;

	startTicks = 0;
	pausedTicks = 0;
}

void Timer::pause() noexcept
{
	if (started && !paused) {
		paused = true;

		pausedTicks = SDL_GetTicks() - startTicks;
		startTicks = 0;
	}
}

void Timer::unpause() noexcept
{
	if (started && paused) {
		paused = false;

		startTicks = SDL_GetTicks() - pausedTicks;

		pausedTicks = 0;
	}
}

uint32_t Timer::getTicks() noexcept
{
	uint32_t time{ 0 };

	if (started) {
		if (paused) {
			time = pausedTicks;
		} else {
			time = SDL_GetTicks() - startTicks;
		}
	}

	return time;
}

bool Timer::isStarted() noexcept
{
	return started;
}

bool Timer::isPaused() noexcept
{
	return paused && started;
}
