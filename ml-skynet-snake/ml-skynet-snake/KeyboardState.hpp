#pragma once


#include <memory>
#include "Keyboard.hpp"

class KeyboardState : public Keyboard
{
public:
	friend class Input;

	KeyboardState();

	bool getKeyValue(SDL_Scancode keyCode) const;
	keyboard::ButtonState getKeyState(SDL_Scancode keyCode) const;

protected:
	//const uint8_t* currentState_{ nullptr };
	uint8_t currentState_[SDL_NUM_SCANCODES] = { 0 };
	uint8_t previousState_[SDL_NUM_SCANCODES] = { 0 };
};