#pragma once

#include <cstdint>

class Timer
{
public:
	void start();
	void stop();
	void pause();
	void unpause();

	uint32_t getTicks();

	bool isStarted();
	bool isPaused();

private:
	uint32_t startTicks{ 0 };
	uint32_t pausedTicks{ 0 };

	bool paused{ false };
	bool started{ false };
};

