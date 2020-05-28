#pragma once

#include <cstdint>

class Timer
{
public:
	void start() noexcept;
	void stop() noexcept;
	void pause() noexcept;
	void unpause() noexcept;

	uint32_t getTicks() noexcept;

	bool isStarted() noexcept;
	bool isPaused() noexcept;

private:
	uint32_t startTicks{ 0 };
	uint32_t pausedTicks{ 0 };

	bool paused{ false };
	bool started{ false };
};

