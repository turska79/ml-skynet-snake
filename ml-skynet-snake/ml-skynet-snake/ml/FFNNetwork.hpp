#pragma once

#pragma warning(push)  
#pragma warning( disable : 26819 26495 4244 26451 6011 26439 26812 4458 4267 4801 4081)
#include <mlpack/methods/ann/ffn.hpp>
#include <mlpack/methods/ann/loss_functions/mean_squared_error.hpp>
#pragma warning (pop)

#include <memory>

namespace ml {

	using Mean = mlpack::ann::MeanSquaredError<>;
	using Gaussian = mlpack::ann::GaussianInitialization;
	using FFNNetwork = mlpack::ann::FFN<Mean, Gaussian>;

	class Network
	{
	public:
		Network();
		mlpack::ann::FFN<mlpack::ann::MeanSquaredError<>, mlpack::ann::GaussianInitialization>* getNetwork();
	protected:
		std::unique_ptr<mlpack::ann::FFN<mlpack::ann::MeanSquaredError<>, mlpack::ann::GaussianInitialization>> network_;
	};

}

