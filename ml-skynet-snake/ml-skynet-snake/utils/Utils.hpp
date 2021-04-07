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
		constexpr uint32_t simulationRefreshRateTargetTimeStep{ secondAsMilliseconds / 60 };
		constexpr unsigned int processOneStepPerUpdate{ 1 };
		constexpr unsigned int lowSpeedLimit{ processOneStepPerUpdate };

		namespace fontSize {
			constexpr unsigned int twenty{ 20 };
		}

		namespace color {
			constexpr SDL_Color black = { 0, 0, 0,255 };
		}

		namespace ml {
			namespace inputParameters {
				constexpr unsigned int count{ 30 };

				constexpr unsigned int snake_head_x_in_data{ 24 };
				constexpr unsigned int snake_head_y_in_data{ 25 };

				constexpr unsigned int direction_vector_x{ 26 };
				constexpr unsigned int direction_vector_y{ 27 };

				constexpr unsigned int food_x{ 27 };
				constexpr unsigned int food_y{ 28 };
			}
			constexpr unsigned int left_or_forward_or_right{ 3 };
			constexpr unsigned int number_of_possible_actions{ left_or_forward_or_right };
			constexpr unsigned int max_steps{ 500 };
		}
	}
	
	namespace math {
		const double pi{ std::atan(1.0) * 4 };
	}
}
