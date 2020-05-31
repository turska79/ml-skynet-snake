#pragma once

#include <memory>
#include <array>
#include "Keyboard.hpp"

class KeyboardState : public Keyboard
{
public:
	friend class Input;

	KeyboardState();

	virtual ~KeyboardState() = default;
	KeyboardState(const KeyboardState&) = default;
	KeyboardState& operator=(const KeyboardState&) = default;
	KeyboardState(KeyboardState&&) = default;
	KeyboardState& operator=(KeyboardState&&) = default;

	bool getKeyValue(SDL_Scancode keyCode) const noexcept override;
	keyboard::ButtonState getKeyState(SDL_Scancode keyCode) const noexcept override;

protected:
	std::array<uint8_t, SDL_NUM_SCANCODES> currentState_{ 0 };
	std::array<uint8_t, SDL_NUM_SCANCODES> previousState_{ 0 };
};