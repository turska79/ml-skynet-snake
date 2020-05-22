#pragma once

#include "KeyboardState.hpp"

class Input 
{
public:
	Input();
	virtual ~Input();
		
	void update();
	const Keyboard& getKeyboard() const;

private:
	void prepareForUpdate();
	KeyboardState keyboard_;
};
