#pragma once

#pragma warning(push)  
#pragma warning( disable : 26819 26495 4244 26451 6011 26439 26812 4458 4267 4801 4081)
#include <mlpack/methods/ann/ffn.hpp>
#include <mlpack/methods/ann/loss_functions/mean_squared_error.hpp>

#include <mlpack/methods/ann/loss_functions/empty_loss.hpp>
#include <mlpack/methods/ann/init_rules/gaussian_init.hpp>
#pragma warning (pop)

#include <memory>

namespace ml {

	// Set up the actor and critic networks.
	/*
	FFN<EmptyLoss<>, GaussianInitialization> policyNetwork(EmptyLoss<>(), GaussianInitialization(0, 0.01));
	policyNetwork.Add(new Linear<>(ContinuousActionEnv::State::dimension, 128));
	policyNetwork.Add(new ReLULayer<>());
	policyNetwork.Add(new Linear<>(128, 128));
	policyNetwork.Add(new ReLULayer<>());
	policyNetwork.Add(new Linear<>(128, ContinuousActionEnv::Action::size));
	policyNetwork.Add(new TanHLayer<>());
	policyNetwork.ResetParameters();
	*/
	
	constexpr unsigned int input_parameters{ 28 };
	constexpr unsigned int actions{ 4 };
	
	using Mean = mlpack::ann::MeanSquaredError<>;
	using Gaussian = mlpack::ann::GaussianInitialization;
	using FFNNetwork = mlpack::ann::FFN<Mean, Gaussian>;

	class Network {
	public:
		Network();
		mlpack::ann::FFN<mlpack::ann::MeanSquaredError<>, mlpack::ann::GaussianInitialization>* getNetwork();
	protected:
		std::unique_ptr<mlpack::ann::FFN<mlpack::ann::MeanSquaredError<>, mlpack::ann::GaussianInitialization>> network_;
	};

	using FeedForwardNetwork = mlpack::ann::FFN<mlpack::ann::EmptyLoss<>, mlpack::ann::GaussianInitialization>;

	class PolicyNetwork {
	public:
		PolicyNetwork();
		FeedForwardNetwork& getPolicyNetwork();
		FeedForwardNetwork& getQNetwork();

	private:
		std::unique_ptr<FeedForwardNetwork> policyNetwork_;
		std::unique_ptr<FeedForwardNetwork> qnetwork_;
	};
}

