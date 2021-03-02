#include "FFNNetwork.hpp"

ml::Network::Network()
{
	network_ = std::make_unique<FFNNetwork>(Mean(), Gaussian(0, 0.001));
	network_->Add<mlpack::ann::Linear<>>(28, 64);
	network_->Add<mlpack::ann::ReLULayer<>>();
	network_->Add<mlpack::ann::Linear<>>(64, 64);
	network_->Add<mlpack::ann::Softmax<>>();
	network_->Add<mlpack::ann::Linear<>>(64, 4);
}
mlpack::ann::FFN<mlpack::ann::MeanSquaredError<>, mlpack::ann::GaussianInitialization>* ml::Network::getNetwork()
{
	return network_.get();
}

ml::PolicyNetwork::PolicyNetwork()
{
	/*mlpack::ann::FFN< mlpack::ann::EmptyLoss<>, mlpack::ann::GaussianInitialization> policyNetwork(mlpack::ann::EmptyLoss<>(), mlpack::ann::GaussianInitialization(0, 0.01));
	policyNetwork.Add(new  mlpack::ann::Linear<>(28, 128));
	policyNetwork.Add(new  mlpack::ann::ReLULayer<>());
	policyNetwork.Add(new  mlpack::ann::Linear<>(128, 128));
	policyNetwork.Add(new  mlpack::ann::ReLULayer<>());
	policyNetwork.Add(new  mlpack::ann::Linear<>(128, 4));
	policyNetwork.Add(new  mlpack::ann::TanHLayer<>());
	policyNetwork.ResetParameters();
	*/
	policyNetwork_ = std::make_unique<ml::FeedForwardNetwork>(mlpack::ann::EmptyLoss<>(), mlpack::ann::GaussianInitialization(0, 0.01));

	policyNetwork_->Add(new  mlpack::ann::Linear<>(ml::input_parameters, 128));
	policyNetwork_->Add(new  mlpack::ann::ReLULayer<>());
	policyNetwork_->Add(new  mlpack::ann::Linear<>(128, 128));
	policyNetwork_->Add(new  mlpack::ann::ReLULayer<>());
	policyNetwork_->Add(new  mlpack::ann::Linear<>(128, ml::actions));
	policyNetwork_->Add(new  mlpack::ann::TanHLayer<>());
	policyNetwork_->ResetParameters();

	qnetwork_ = std::make_unique<ml::FeedForwardNetwork>(mlpack::ann::EmptyLoss<>(), mlpack::ann::GaussianInitialization(0, 0.01));
	qnetwork_->Add(new mlpack::ann::Linear<>(ml::input_parameters + ml::actions, 128));
	qnetwork_->Add(new mlpack::ann::ReLULayer<>());
	qnetwork_->Add(new mlpack::ann::Linear<>(128, 128));
	qnetwork_->Add(new mlpack::ann::ReLULayer<>());
	qnetwork_->Add(new mlpack::ann::Linear<>(128, 1));
	qnetwork_->ResetParameters();
}

ml::FeedForwardNetwork& ml::PolicyNetwork::getPolicyNetwork()
{
	return *(policyNetwork_.get());
}

ml::FeedForwardNetwork& ml::PolicyNetwork::getQNetwork()
{
	return *(qnetwork_.get());
}
