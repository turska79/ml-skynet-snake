#pragma once

#include <SDL.h>

namespace keyboard {

	enum class ButtonState { none, pressed, released, held };
}

class Keyboard
{
public:
	virtual bool getKeyValue(SDL_Scancode keyCode) const = 0;
	virtual keyboard::ButtonState getKeyState(SDL_Scancode keyCode) const = 0;
};
