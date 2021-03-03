#pragma once

#include <memory>
#include "RunningState.hpp"

namespace ml {
	class LearningAgent;
}

namespace thread {
	class interruptibleThread;
}

namespace gamestates {

	namespace state {
		
		class RunningStateAI : public RunningState
		{
		public:
			explicit RunningStateAI(Game& game) noexcept;
			virtual ~RunningStateAI();

			void enter() override;
			void exit() override;
			void update(Renderer& renderer) override;
			void handleInput(const Keyboard& keyboard) override;

		protected:
			void runLearningAgent();
			void printStepsToScreen(Renderer& renderer);
			void printGameCountToScreen(Renderer& renderer);
			void snakePositionUpdated() override;

		private:
			thread::interruptibleThread* ai_{ nullptr };
			std::size_t gameCount_{ 0 };

			std::unique_ptr<ml::LearningAgent> learningAgent_;
			bool running_{ false };
		};

	}
}