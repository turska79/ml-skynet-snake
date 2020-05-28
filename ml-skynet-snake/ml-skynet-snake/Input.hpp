#pragma once

#include "KeyboardState.hpp"

class Input 
{
public:
	void update() noexcept;
	const Keyboard& getKeyboard() const noexcept;

private:
	void prepareForUpdate() noexcept;
	KeyboardState keyboard_;
};
