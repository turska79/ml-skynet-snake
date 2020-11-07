#pragma once

#include "BaseState.hpp"
#include "../Food.hpp"

class Renderer;
class Keyboard;
class Game;
class SnakeControl;
class Simulation;

namespace gamestates {

	namespace state {

		class RunningState : public BaseState
		{
		public:
			explicit RunningState(Game& game) noexcept;
			virtual ~RunningState() {};


			virtual void snakeCollisionCallback();

			virtual void enter() override;
			virtual void update(Renderer& renderer) override;
			virtual void exit() override;
			virtual void handleInput(const Keyboard& keyboard) override;

		protected:
			void resetBoard() const;
			void initSnake();
			void newRandomPositionForFood();

			void registerCallbacks();
			void unregisterCallbacks();
			void registerCollisionCallback();
			void unregisterCollisionCallback();
			void registerFoodEatenCallback();
			void unregisterFoodEatenCallback();

			SnakeControl& snakeControl_;
			Simulation& simulation_;
			Food food_;
		};

	}
}