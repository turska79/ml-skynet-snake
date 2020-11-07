#pragma once

#pragma warning(push)  
#pragma warning(disable : 26819 26812)
#include <SDL_scancode.h>
#pragma warning( pop )

namespace keyboard {

	enum class ButtonState { none, pressed, released, held };
}

class Keyboard
{
public:
	virtual bool getKeyValue(SDL_Scancode keyCode) const = 0;
	virtual keyboard::ButtonState getKeyState(SDL_Scancode keyCode) const = 0;
};
