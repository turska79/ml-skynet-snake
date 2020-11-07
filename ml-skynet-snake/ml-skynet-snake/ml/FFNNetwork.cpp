#include "FFNNetwork.hpp"

ml::Network::Network()
{
	network_ = std::make_unique<FFNNetwork>(Mean(), Gaussian(0, 0.001));
	network_->Add< mlpack::ann::Linear<>>(28, 64);
	network_->Add< mlpack::ann::ReLULayer<>>();
	network_->Add< mlpack::ann::Linear<>>(64, 64);
	network_->Add< mlpack::ann::Softmax<>>();
	network_->Add< mlpack::ann::Linear<>>(64, 4);
}
mlpack::ann::FFN<mlpack::ann::MeanSquaredError<>, mlpack::ann::GaussianInitialization>* ml::Network::getNetwork()
{
	return network_.get();
}
