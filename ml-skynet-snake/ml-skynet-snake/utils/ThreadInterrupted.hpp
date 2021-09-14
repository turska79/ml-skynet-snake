#pragma once

#include <exception>

namespace thread {
	namespace utils {
		class ThreadInterrupted : public std::exception {
		public:
		};
	}
}