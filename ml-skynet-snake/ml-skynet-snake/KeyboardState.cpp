#include "KeyboardState.hpp"
#include <iostream>

KeyboardState::KeyboardState()
{
	const uint8_t* current = SDL_GetKeyboardState(nullptr);
	std::copy(current, current + SDL_NUM_SCANCODES, std::begin(currentState_));
}

keyboard::ButtonState KeyboardState::getKeyState(SDL_Scancode keyCode) const noexcept
{
	if (previousState_.at(keyCode) == 0) {
		if (currentState_.at(keyCode) == 0) {
			return keyboard::ButtonState::none;
		}

		return keyboard::ButtonState::pressed;
	} else {
		if (currentState_.at(keyCode) == 0) {
			return keyboard::ButtonState::released;
		}
		return keyboard::ButtonState::held;
	}
}

bool KeyboardState::getKeyValue(SDL_Scancode keyCode) const noexcept
{
	if (currentState_.at(keyCode) == 1) {
		return true;
	}

	return false;
}