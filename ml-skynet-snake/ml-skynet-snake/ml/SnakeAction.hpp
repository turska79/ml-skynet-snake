#pragma once

#include "../SnakeVision.hpp"
#include "../utils/Point.hpp"
#include "State.hpp"
#include <vector>
//#include <condition_variable>

class Board;
class SnakeControl;

namespace ml {
	class Environment : public State
	{
	public:
		double reward_;
		bool terminal_;
	};

	
	class ContinuousActionEnvironment : public State
	{
	public:

		class Action
		{
		public:
			enum actions
			{
				up,
				down,
				right,
				left,
			};
			
			Action() : action(ContinuousActionEnvironment::Action::size) { /* Nothing to do here */ }
			// To store the action.
			//actions action;
			std::vector<double> action;

			// Track the size of the action space.
			static constexpr size_t size = 4;
		};

		ContinuousActionEnvironment();

		double Sample(const State& state, const Action& action, State& nextState);
		double Sample(const State& state, const Action& action);

		State InitialSample();

		bool IsTerminal(const State& state) const;

		size_t StepsPerformed() const;
		size_t MaxSteps() const;
		size_t& MaxSteps();

		void setBoard(Board* board);
		void setSnakeControl(SnakeControl* SnakeControl);
		SnakeVision& snakeVision();

		//typedef std::function<void()> Predicate;

		//bool isReadyForNextStep();
		//bool notReadyForNextStep();
		//void proceedToNextStep();
	private:

		const int terminalReward{ -100 };
		const int maxStepsReward{ -30 };
		const int foodFoundReward{ 100 };
		const int emptyStepReward{ 0 };

		size_t maxSteps{ 500 };
		size_t stepsPerformed{ 0 };

		bool readyForNextStep_{ false };
		//std::condition_variable cv_;
		//std::unique_ptr<std::condition_variable> cv_;
		//std::unique_ptr<std::mutex> mutex_;

		Board* board_{ nullptr };
		SnakeControl* snakeControl_{ nullptr };
		SnakeVision snakeVision_;
	};
}