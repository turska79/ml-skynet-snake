/*
Copyright (c) 2020 Chan Jer Shyan
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <cmath>
#include "Utils.hpp"


template <class T> class Vec2 {
public:
	T x{ 0 }, y{ 0 };

	Vec2() : x(0), y(0) {}
	Vec2(T x, T y) : x(x), y(y) {}
	Vec2(const Vec2& v) : x(v.x), y(v.y) {}

	Vec2& operator=(const Vec2& v) {
		x = v.x;
		y = v.y;
		return *this;
	}

	Vec2 operator+(Vec2& v) {
		return Vec2(x + v.x, y + v.y);
	}
	Vec2 operator-(Vec2& v) {
		return Vec2(x - v.x, y - v.y);
	}

	Vec2& operator+=(const Vec2& v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	Vec2& operator-=(const Vec2& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vec2 operator+(double s) {
		return Vec2(x + s, y + s);
	}
	Vec2 operator-(double s) {
		return Vec2(x - s, y - s);
	}
	Vec2 operator*(double s) {
		return Vec2(x * s, y * s);
	}
	Vec2 operator/(double s) {
		return Vec2(x / s, y / s);
	}

	Vec2& operator+=(const double s) {
		x += s;
		y += s;
		return *this;
	}
	Vec2& operator-=(const double s) {
		x -= s;
		y -= s;
		return *this;
	}
	Vec2& operator*=(const double s) {
		x *= s;
		y *= s;
		return *this;
	}
	Vec2& operator/=(double s) {
		x /= s;
		y /= s;
		return *this;
	}

	void set(T x, T y) {
		this->x = x;
		this->y = y;
	}

	void rotate(double deg) {
		double theta = deg / 180.0 * utils::math::pi;
		double c = cos(theta);
		double s = sin(theta);
		double tx = x * c - y * s;
		double ty = x * s + y * c;
		x = tx;
		y = ty;
	}

	Vec2& normalize() {
		if (length() == 0) return *this;
		*this *= (1.0 / length());
		return *this;
	}

	float dist(Vec2 v) const {
		Vec2 d(v.x - x, v.y - y);
		return d.length();
	}
	float length() const {
		return std::sqrt(x * x + y * y);
	}
	void truncate(double length) {
		double angle = atan2f(y, x);
		x = length * cos(angle);
		y = length * sin(angle);
	}

	Vec2 ortho() const {
		return Vec2(y, -x);
	}

	static float dot(Vec2 v1, Vec2 v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}
	static float cross(Vec2 v1, Vec2 v2) {
		return (v1.x * v2.y) - (v1.y * v2.x);
	}

};

typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;
