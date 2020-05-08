#pragma once

#include <SDL.h>
#include <memory>

class Input;

namespace keyboard {

	enum class ButtonState { none, pressed, released, held };

	class Keyboard {
		virtual bool getKeyValue(SDL_Scancode keyCode) const = 0;
		virtual ButtonState getKeyState(SDL_Scancode keyCode) const = 0;
	};
	
	class KeyboardState : public Keyboard
	{
	public:
		friend class Input;

		KeyboardState();
		virtual ~KeyboardState();

		bool getKeyValue(SDL_Scancode keyCode) const;
		ButtonState getKeyState(SDL_Scancode keyCode) const;

	protected:
		const uint8_t* currentState_{ nullptr };
		uint8_t previousState_[SDL_NUM_SCANCODES] = { 0 };
	};

}

class Input : public keyboard::KeyboardState
{
public:
	Input();
	virtual ~Input();
		
	void update();
	const keyboard::Keyboard* getKeyboard() { return dynamic_cast<keyboard::Keyboard*>(this); }

private:
	void prepareForUpdate();
};
