#include "Timer.hpp"
#include <SDL.h>
/*/
const uint32_t newTime = SDL_GetTicks();
const uint32_t deltaTime = newTime - currentTime;
fps = 1000.0f / deltaTime;
currentTime = newTime;
*/
void Timer::start() noexcept
{
	started = true;
	paused = false;

	startTicks_ = SDL_GetTicks();
	currentTime_ = startTicks_;
	pausedTicks_ = 0;
}

void Timer::stop() noexcept
{
	started = false;
	paused = false;

	startTicks_ = 0;
	pausedTicks_ = 0;
}

void Timer::pause() noexcept
{
	if (started && !paused) {
		paused = true;

		pausedTicks_ = SDL_GetTicks() - startTicks_;
		startTicks_ = 0;
	}
}

void Timer::unpause() noexcept
{
	if (started && paused) {
		paused = false;

		startTicks_ = SDL_GetTicks() - pausedTicks_;

		pausedTicks_ = 0;
	}
}

uint32_t Timer::getTicksSinceStart() noexcept
{
	uint32_t time{ 0 };

	if (started) {
		if (paused) {
			time = pausedTicks_;
		} else {
			time = SDL_GetTicks() - startTicks_;
		}
	}

	return time;
}

uint32_t Timer::deltaTime() noexcept
{
	const uint32_t newTime = SDL_GetTicks();
	const uint32_t deltaTime = newTime - currentTime_;
	currentTime_ = newTime;
	return deltaTime;
}

bool Timer::isStarted() noexcept
{
	return started;
}

bool Timer::isPaused() noexcept
{
	return paused && started;
}
