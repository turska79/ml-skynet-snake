#pragma once

#include <cstdint>

class Timer
{
public:
	void start() noexcept;
	void stop() noexcept;
	void pause() noexcept;
	void unpause() noexcept;

	uint32_t getTicksSinceStart() noexcept;
	uint32_t deltaTime() noexcept;
	bool isStarted() noexcept;
	bool isPaused() noexcept;

private:
	uint32_t startTicks_{ 0 };
	uint32_t pausedTicks_{ 0 };
	uint32_t currentTime_{ 0 };

	bool paused{ false };
	bool started{ false };
};

