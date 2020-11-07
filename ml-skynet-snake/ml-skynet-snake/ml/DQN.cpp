#include "DQN.hpp"

ml::DQN::DQN()
{
    network_ = std::make_unique<Network>();
    auto& network = *(network_.get()->getNetwork());
    model_ = std::make_unique<mlpack::rl::SimpleDQN<>>(network);
}

mlpack::rl::SimpleDQN<>& ml::DQN::model() const
{
    return *(model_.get());
}

mlpack::ann::FFN<mlpack::ann::MeanSquaredError<>, mlpack::ann::GaussianInitialization>& ml::DQN::network() const
{
    return *(network_.get()->getNetwork());
}
/*
Network& ml::DQN::getModel()
{
    return *(network_.get());
}*/
/*
Network& ml::DQN::getModel()
{
    return *(network_.get());
}
*/