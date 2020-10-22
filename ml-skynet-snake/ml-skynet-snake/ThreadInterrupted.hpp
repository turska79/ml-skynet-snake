#pragma once

#include <exception>

class ThreadInterrupted : public std::exception {
public:
};