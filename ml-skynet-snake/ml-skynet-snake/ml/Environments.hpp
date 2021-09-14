#pragma once

#include "State.hpp"
#include "../utils/Utils.hpp"
#include "../Snakevision.hpp"
#include <vector>
#include <atomic>

class SnakeControl;
class Board;


namespace ml {

	class EnvironmentState {
	public:
		enum class State { Idle, Process, Processing, Stop, Stopped };

		auto environmentState() const& -> const State& { return state_; }
		auto environmentState() & -> std::atomic<State>& { return state_; }
		auto environmentState() && -> State&& { return std::move(state_); }
	protected:
		std::atomic<State> state_{ State::Idle };
	};

	class ContinuousActionEnvironment : public State
	{
	public:
		
		ContinuousActionEnvironment();
		ContinuousActionEnvironment(SnakeControl* snakeControl, SnakeVision* snakeVision, Board* board, EnvironmentState* envState);
		
		class Action
		{
		public:
			enum class actions
			{
				left,
				forward,
				right
			};

			Action() : action(size) { };
			
			std::vector<double> action;
			static const size_t size = utils::commonConstants::ml::number_of_possible_actions;
		};


		virtual ml::ContinuousActionEnvironment::State InitialSample();
		ml::ContinuousActionEnvironment::State snapshot(utils::Point<std::size_t> fromPoint);
		virtual double Sample(const ml::ContinuousActionEnvironment::State& currentState, const ml::ContinuousActionEnvironment::Action action, ml::ContinuousActionEnvironment::State& nextState);
		virtual bool IsTerminal(const ml::ContinuousActionEnvironment::State& state);
		
	private:
		SnakeControl* snakeControl_{ nullptr };
		SnakeVision* snakeVision_{ nullptr };
		Board* board_{ nullptr };
		EnvironmentState* envState_{ nullptr };
		
	};
}
