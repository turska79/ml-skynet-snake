#pragma once

#include "State.hpp"
#include "../utils/Utils.hpp"
#include <vector>


namespace ml {

	class Environment : public State
	{
	public:
		//double reward_{ 0 };
		//bool terminal_{ false };
	};

	class ContinuousActionEnvironment : public State
	{
	public:

		class Action
		{
		public:
			enum actions
			{
				left,
				forward,
				right
			};
			
			Action() : action(utils::commonConstants::ml::number_of_possible_actions) { /* Nothing to do here */ }
			// To store the action.
			std::vector<double> action;
		};
	};
}