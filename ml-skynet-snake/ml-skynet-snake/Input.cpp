#include "Input.hpp"
#include <memory.h>
#include <iostream>
#include <string>

void Input::prepareForUpdate() noexcept
{
	memcpy_s(keyboard_.previousState_, SDL_NUM_SCANCODES, keyboard_.currentState_, SDL_NUM_SCANCODES);
}

void Input::update() noexcept
{
	prepareForUpdate();

	const uint8_t* current = SDL_GetKeyboardState(nullptr);

	memcpy_s(keyboard_.currentState_, SDL_NUM_SCANCODES, current, SDL_NUM_SCANCODES);
}

const Keyboard& Input::getKeyboard() const noexcept
{ 
	return dynamic_cast<const Keyboard&>(keyboard_);
}
