#include "Input.hpp"
#include <memory.h>

#pragma warning(push)  
#pragma warning(disable : 26819 26812)
#include <SDL_keyboard.h>
#pragma warning( pop )

void Input::prepareForUpdate() noexcept
{
	keyboard_.previousState_ = keyboard_.currentState_;
}

void Input::update()
{
	prepareForUpdate();

	const uint8_t* current = SDL_GetKeyboardState(nullptr);
	std::copy(current, current + SDL_NUM_SCANCODES, std::begin(keyboard_.currentState_));
}

const Keyboard& Input::getKeyboard() const noexcept
{ 
	return dynamic_cast<const Keyboard&>(keyboard_);
}
