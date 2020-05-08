#include "Input.hpp"
#include <memory.h>

namespace keyboard {

	KeyboardState::KeyboardState()
	{
		currentState_ = SDL_GetKeyboardState(nullptr);
	}

	KeyboardState::~KeyboardState()
	{

	}

	ButtonState KeyboardState::getKeyState(SDL_Scancode keyCode) const
	{
		if (previousState_[keyCode] == 0) {
			if (currentState_[keyCode] == 0) {
				return ButtonState::none;
			}

			return ButtonState::pressed;
		}
		else {
			if (currentState_[keyCode] == 0) {
				return ButtonState::released;
			}
			return ButtonState::held;
		}
	}

	bool KeyboardState::getKeyValue(SDL_Scancode keyCode) const
	{
		if (currentState_[keyCode] == 1) {
			return true;
		}

		return false;
	}

}

Input::Input()// : keyboard_(std::make_unique<keyboard::KeyboardState>(keyboard::KeyboardState()))
{
	
}


Input::~Input()
{

}

void Input::prepareForUpdate()
{
	memcpy_s(previousState_, SDL_NUM_SCANCODES, currentState_, SDL_NUM_SCANCODES);
}

void Input::update()
{
	prepareForUpdate();
	currentState_ = SDL_GetKeyboardState(nullptr);
}

/*
const bool Input::getKeyValue(SDL_Scancode keyCode) const
{
	return keyboard_.getKeyValue(keyCode);
}

const keyboard::ButtonState Input::getKeyState(SDL_Scancode keyCode) const
{
	return keyboard_.getKeyState(keyCode);
}*/