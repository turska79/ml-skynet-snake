#pragma once


#include <memory>
#include "Keyboard.hpp"

class KeyboardState : public Keyboard
{
public:
	friend class Input;

	KeyboardState();

	bool getKeyValue(SDL_Scancode keyCode) const override;
	keyboard::ButtonState getKeyState(SDL_Scancode keyCode) const override;

protected:
	uint8_t currentState_[SDL_NUM_SCANCODES] = { 0 };
	uint8_t previousState_[SDL_NUM_SCANCODES] = { 0 };
};