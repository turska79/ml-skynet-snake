#include "KeyboardState.hpp"


KeyboardState::KeyboardState()
{
	const uint8_t* current = SDL_GetKeyboardState(nullptr);
	memcpy_s(currentState_, SDL_NUM_SCANCODES, current, SDL_NUM_SCANCODES);
}

keyboard::ButtonState KeyboardState::getKeyState(SDL_Scancode keyCode) const
{
	if (previousState_[keyCode] == 0) {
		if (currentState_[keyCode] == 0) {
			return keyboard::ButtonState::none;
		}

		return keyboard::ButtonState::pressed;
	} else {
		if (currentState_[keyCode] == 0) {
			return keyboard::ButtonState::released;
		}
		return keyboard::ButtonState::held;
	}
}

bool KeyboardState::getKeyValue(SDL_Scancode keyCode) const
{
	if (currentState_[keyCode] == 1) {
		return true;
	}

	return false;
}