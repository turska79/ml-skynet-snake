#include "Input.hpp"
#include <memory.h>
#include <iostream>
#include <string>


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

const Keyboard& Input::getKeyboard() const
{ 
	return dynamic_cast<const Keyboard&>(keyboard_);
}
