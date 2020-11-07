#pragma once

#include "BaseState.hpp"

class Game;

namespace gamestates {

	namespace state {
		class GameOverState : public BaseState
		{
		public:
			explicit GameOverState(Game& game) noexcept;
			virtual ~GameOverState() {};

			void enter() override;
			void update(Renderer& renderer) override;
			void exit() override;
			void handleInput(const Keyboard& keyboard) override;
		};
	}
}

