#pragma once

#include <utility>
#include <cmath>
#include <cstdint>

#pragma warning(push)  
#pragma warning(disable : 26819 26812)
#include <SDL_pixels.h>
#pragma warning( pop )

namespace utils {
	namespace commonConstants {
		constexpr unsigned int secondAsMilliseconds{ 1000 };
		constexpr unsigned int targetFramesPerSecond{ 60 };
		constexpr uint32_t refreshRateTargetTimeStep{ secondAsMilliseconds / targetFramesPerSecond };
		constexpr uint32_t simulationRefreshRateTargetTimeStep{ secondAsMilliseconds / 30 };
		constexpr unsigned int processOneStepPerUpdate{ 1 };
		constexpr unsigned int lowSpeedLimit{ processOneStepPerUpdate };

		namespace fontSize {
			constexpr unsigned int twenty{ 20 };
		}

		namespace color {
			constexpr SDL_Color black = { 0, 0, 0,255 };
		}
	}
	
	namespace math {
		const double pi{ std::atan(1.0) * 4 };
	}
}
