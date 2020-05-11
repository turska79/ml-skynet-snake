#include "Input.hpp"
#include <memory.h>
#include <iostream>
#include <string>
//namespace keyboard {

	KeyboardState::KeyboardState()
	{
		const uint8_t* current = SDL_GetKeyboardState(nullptr);
		memcpy_s(currentState_, SDL_NUM_SCANCODES, current, SDL_NUM_SCANCODES);
	}

	KeyboardState::~KeyboardState()
	{

	}

	keyboard::ButtonState KeyboardState::getKeyState(SDL_Scancode keyCode) const
	{
		if (previousState_[keyCode] == 0) {
			if (currentState_[keyCode] == 0) {
				return keyboard::ButtonState::none;
			}

			return keyboard::ButtonState::pressed;
		}
		else {
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

//}

Input::Input()
{
	
}


Input::~Input()
{

}

void Input::prepareForUpdate()
{
	memcpy_s(keyboard_.previousState_, SDL_NUM_SCANCODES, keyboard_.currentState_, SDL_NUM_SCANCODES);
}

void Input::update()
{
	prepareForUpdate();

	const uint8_t* current = SDL_GetKeyboardState(nullptr);

	memcpy_s(keyboard_.currentState_, SDL_NUM_SCANCODES, current, SDL_NUM_SCANCODES);
	/*
	for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
		if (keyboard_.currentState_[i] != 0)
			std::cout << "key pressed:" << std::to_string(keyboard_.currentState_[i]) << std::endl;
	}*/
}

const Keyboard& Input::getKeyboard()
{ 
	return dynamic_cast<Keyboard&>(keyboard_);
}
