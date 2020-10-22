#pragma once

#include <cmath>

namespace utils {
	template<class T> struct Point
	{
		Point() noexcept {};
		Point(T x, T y) noexcept : x_(x), y_(y) {};
		Point<T> normalise()
		{
			Point<T> normalised(*this);

			T len = std::sqrt(normalised.x_ * normalised.x_ + normalised.y_ * normalised.y_);

			if (len != 0.) {
				normalised.x_ /= len;
				normalised.y_ /= len;
			}
			return normalised;
		}

		T x_{ 0 };
		T y_{ 0 };
	};
}