#pragma once

#pragma warning(push)  
#pragma warning( disable : 26819 26495 4244 26451 6011 26439 26812 4458 4267 4801 4081)
#include <mlpack/methods/reinforcement_learning/q_networks/simple_dqn.hpp>
#pragma warning (pop)

#include <memory>
#include "FFNNetwork.hpp"


namespace ml {
	class DQN
	{
	public:
		DQN();
		
		mlpack::rl::SimpleDQN<>& model() const;
		mlpack::ann::FFN<mlpack::ann::MeanSquaredError<>, mlpack::ann::GaussianInitialization>& network() const;

	protected:
		std::unique_ptr<Network> network_;
		std::unique_ptr<mlpack::rl::SimpleDQN<> > model_;
	};

}