#pragma once

#include <SDL.h>
#include <memory>

class Input;

namespace keyboard {

	enum class ButtonState { none, pressed, released, held };
}

	class Keyboard
	{
	public:
		virtual ~Keyboard() {};
		virtual bool getKeyValue(SDL_Scancode keyCode) const = 0;
		virtual keyboard::ButtonState getKeyState(SDL_Scancode keyCode) const = 0;
	};
	
	class KeyboardState : public Keyboard
	{
	public:
		friend class Input;

		KeyboardState();
		virtual ~KeyboardState();

		bool getKeyValue(SDL_Scancode keyCode) const;
		keyboard::ButtonState getKeyState(SDL_Scancode keyCode) const;

	protected:
		//const uint8_t* currentState_{ nullptr };
		uint8_t currentState_[SDL_NUM_SCANCODES] = { 0 };
		uint8_t previousState_[SDL_NUM_SCANCODES] = { 0 };
	};



class Input 
{
public:
	Input();
	virtual ~Input();
		
	void update();
	const Keyboard& getKeyboard();

private:
	void prepareForUpdate();
	KeyboardState keyboard_;
};
