#pragma once

#include <vector>

#include "State.hpp"

class GameStates {
public:
	template<class T, class... Args>
	void pushState(Args&&... args) {
		pushStateImpl(std::make_unique<T>(std::forward<Args>(args)...));
	}
	void popState() {
		popStateImpl();
	}

protected:
	void pushStateImpl(std::unique_ptr<State> state)
	{
		if (states_.empty() == false) {
			states_.back()->exit();
		}

		state->enter();

		states_.emplace_back(std::move(state));
	}

	void popStateImpl()
	{
		if (states_.empty() == false) {
			states_.back()->exit();
			states_.pop_back();
		}

		if (states_.empty() == false) {
			states_.back()->enter();
		}
	}

	std::vector<std::unique_ptr<State>> states_;
};