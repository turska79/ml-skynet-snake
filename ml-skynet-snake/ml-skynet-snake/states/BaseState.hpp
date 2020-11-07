#pragma once

class Keyboard;
class Renderer;
class Game;

namespace gamestates {

	namespace state {

		class BaseState
		{
		public:
			BaseState(Game& game) noexcept : game_(game) { };
			virtual ~BaseState() {};

			virtual void enter() = 0;
			virtual void update(Renderer& renderer) = 0;
			virtual void exit() = 0;
			virtual void handleInput(const Keyboard& keyboard) = 0;

		protected:
			Game& game_;
		};
	}
}