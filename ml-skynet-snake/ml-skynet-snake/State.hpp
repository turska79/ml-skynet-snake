#pragma once

class IState
{
public:
	virtual void run() = 0;
	virtual void enter() = 0;
	virtual void exit() = 0;
};