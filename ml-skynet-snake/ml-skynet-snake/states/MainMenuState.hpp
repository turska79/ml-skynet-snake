#pragma once

#include "BaseState.hpp"

class Game;

namespace gamestates {

	namespace state {
		
		class MainMenuState : public BaseState
		{
		public:
			explicit MainMenuState(Game& game) noexcept;
			virtual ~MainMenuState() {};

			void enter() override;
			void update(Renderer& renderer) override;
			void exit() override;
			void handleInput(const Keyboard& keyboard) override;
		};

	}
}
