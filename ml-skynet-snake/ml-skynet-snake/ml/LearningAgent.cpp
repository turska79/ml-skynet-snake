
#include "LearningAgent.hpp"
#include "../SnakeControl.hpp"
#include "../Board.hpp"
#include <mutex>
#include <condition_variable>
#include <memory>
#include "../utils/InterruptibleThread.hpp"
#include "../utils/Utils.hpp"
#include "../Game.hpp"
#include "../states/GameOverState.hpp"
#pragma warning(push)  
#pragma warning(disable : 26819 26812)
#include <SDL.h>
#pragma warning( pop )

#pragma warning(push)  
#pragma warning(disable : 26819 26495 4244 26451 6011 26439 26812 4458 4267 4801 4081)
//#include <mlpack/methods/reinforcement_learning/policy/greedy_policy.hpp>
//#include <mlpack/methods/reinforcement_learning/training_config.hpp>
//#include <mlpack/methods/reinforcement_learning/sac.hpp>
//#include <mlpack/methods/ann/loss_functions/empty_loss.hpp>
//#include <mlpack/methods/ann/ffn.hpp>
//#include <mlpack/core/math/random.hpp>
#pragma warning (pop)

namespace ml {
	//constexpr double initialEpsilon{ 1.0 };
	//constexpr size_t annealInterval{ 1000 };
	//constexpr double minEpsilon{ 0.1 };

	constexpr size_t batchSize{ 32 };
	constexpr size_t capacity{ 10000 };
	constexpr double alpha{ 0.6 };
}


ml::LearningAgent::LearningAgent(SnakeControl& snakeControl, Board& board, Game& game) : snakeControl_(snakeControl), board_(board), game_(game)
{
	replayMethod_ = std::make_unique<ml::ReplayMethod>(ml::batchSize, ml::capacity, ml::alpha);
	auto& replayMethod{ *(replayMethod_.get()) };

	policyNetwork_ = std::make_unique<ml::FeedForwardNetwork>(mlpack::ann::EmptyLoss<>(), mlpack::ann::GaussianInitialization(0, 1.0));

	policyNetwork_->Add(new  mlpack::ann::Linear<>(utils::commonConstants::ml::inputParameters::count, 128));
	policyNetwork_->Add(new  mlpack::ann::ReLULayer<>());
	policyNetwork_->Add(new  mlpack::ann::Linear<>(128, 128));
	policyNetwork_->Add(new  mlpack::ann::ReLULayer<>());
	policyNetwork_->Add(new  mlpack::ann::Linear<>(128, utils::commonConstants::ml::number_of_possible_actions));
	policyNetwork_->Add(new  mlpack::ann::TanHLayer<>());
	policyNetwork_->ResetParameters();
	FeedForwardNetwork& policyNetwork = *(policyNetwork_.get());

	qnetwork_ = std::make_unique<ml::FeedForwardNetwork>(mlpack::ann::EmptyLoss<>(), mlpack::ann::GaussianInitialization(0, 1.0));
	qnetwork_->Add(new mlpack::ann::Linear<>(utils::commonConstants::ml::inputParameters::count + utils::commonConstants::ml::number_of_possible_actions, 128));
	qnetwork_->Add(new mlpack::ann::ReLULayer<>());
	qnetwork_->Add(new mlpack::ann::Linear<>(128, 128));
	qnetwork_->Add(new mlpack::ann::ReLULayer<>());
	qnetwork_->Add(new mlpack::ann::Linear<>(128, 1));
	qnetwork_->ResetParameters();
	FeedForwardNetwork& qNetwork = *(qnetwork_.get());
		
	trainingConfig_ = std::make_unique<Config>();
	mlpack::rl::TrainingConfig& config{ *(trainingConfig_.get()) };

	config.ExplorationSteps() = 5;
	config.TargetNetworkSyncInterval() = 1;
	config.UpdateInterval() = 1;
	//config.StepLimit() = 500;
	//config.DoubleQLearning() = true;

	//ContinuousActionEnvironment& environment{ (ContinuousActionEnvironment&)*this };
	ContinuousActionEnvironment environment(std::addressof(snakeControl_), std::addressof(snakeVision_), std::addressof(board_), std::addressof(envState_));

	agent_ = std::make_unique<SoftActionCritic>(config, qNetwork, policyNetwork, replayMethod, ens::AdamUpdate(), ens::AdamUpdate(), environment);
}

ml::LearningAgent::~LearningAgent()
{
}

// Function to calculate distance 
/*
float distance(int x1, int y1, int x2, int y2)
{
	// Calculating distance 
	return sqrt(pow(x2 - x1, 2) +
		pow(y2 - y1, 2) * 1.0);
}
*/
/*
void ml::LearningAgent::runLearningAgent()
{
	
	//runEpisode();
	//return;

	
	std::cout << "LearningAgent::runLearningAgent() enter" << std::endl;
	//agent_->Episode();
	episodeRunning_ = true;

	step_ = 0;
	double totalReturn{ 0.0 };
	double reward{ 0 };
	double episodeReturn{ 0.0 };

	updateEnvironment();
	
	while (episodeRunning_) {
		std::cout << "LearningAgent::runLearningAgent() thinking about next move" << std::endl;
		uint32_t start = SDL_GetTicks();
		processNextStep_ = false;
		state_ = LearningAgentState::Processing;

		Cell* cell = board_.findFood();
		auto currentPosition{ snakeControl_.getPosition() };
		
		distanceToFood_ = distance(currentPosition.x_, currentPosition.y_, cell->x_, cell->y_);
		std::cout << "LearningAgent::runLearningAgent() distance to food: " << std::to_string(distanceToFood_) << std::endl;
		
		agent_->State().Data() = environment_.Encode();
		agent_->SelectAction();
		arma::mat action { agent_->Action().action };

		unsigned int maxIndex{ 0 };
		double max{ 0 };
		
		auto cols{ action.n_rows };
		
		for (unsigned int i = 0; i < cols; ++i) {
			if (action[i] > max) {
				max = action[i];
				maxIndex = i;
			}
			//std::cout << "LearningAgent::runLearningAgent() action: " << std::to_string(action[i]) << std::endl;
		}

		SnakeControl::Direction newDirection{ decodeDirection(maxIndex) };
		
		switch (newDirection) {
		case SnakeControl::Direction::up:
			std::cout << "LearningAgent::runLearningAgent() action: up" << std::endl;
			break;
		case SnakeControl::Direction::right:
			std::cout << "LearningAgent::runLearningAgent() action: right" << std::endl;
			break;
		case SnakeControl::Direction::down:
			std::cout << "LearningAgent::runLearningAgent() action: down" << std::endl;
			break;
		case SnakeControl::Direction::left:
			std::cout << "LearningAgent::runLearningAgent() action: left" << std::endl;
			break;
		}

		snakeControl_.setDirection(newDirection);

		auto nextPosition = snakeControl_.getNextPosition();

		if (board_.isFood(nextPosition)) {
			reward = 500;
		} else if (board_.isWall(nextPosition) || step_ >= maxSteps()) {// || board_.isSnakeBody(nextPosition)) {
			reward = -500;
			episodeRunning_ = false;
		} else {
			double newDistanceToFood = distance(nextPosition.x_, nextPosition.y_, cell->x_, cell->y_);

			if (newDistanceToFood > distanceToFood_) {
				reward = -10;
			} else {
				reward = 10;
			}
		}

		
		updateEnvironment();

		Environment::State nextState;
		nextState.Data() = environment_.Encode();

		replayMethod_->Store(agent_->State(), agent_->Action(), reward, nextState, episodeRunning_, 0.99);
		episodeReturn += reward;
		std::cout << "LearningAgent::runLearningAgent() episode reward: " << std::to_string(episodeReturn) << std::endl;
		agent_->TotalSteps()++;
		step_++;

		if (agent_->TotalSteps() < trainingConfig_->ExplorationSteps()) {
			std::cout << "LearningAgent::runLearningAgent() more exploration steps" << std::endl;
		} else {
			for (size_t i = 0; i < trainingConfig_->UpdateInterval(); i++) {
				agent_->Update();
			}
		}

		state_ = LearningAgentState::Idle;

		uint32_t end = SDL_GetTicks();
		std::cout << "LearningAgent::runLearningAgent() processing time: " << std::to_string(end-start) << std::endl;
		
		try {
			std::cout << "LearningAgent::runLearningAgent() waiting permission to process next" << std::endl;
			start = SDL_GetTicks();

			std::condition_variable cv;
			std::mutex mutex;
			std::unique_lock<std::mutex> lock(mutex);
			//auto function = std::bind(&LearningAgent::isProcessNextStepSet, this);

			//thread::utils::interruptibleWait<decltype(function)>(cv, lock, function);

			end = SDL_GetTicks();
			std::cout << "LearningAgent::runLearningAgent() waiting time: " << std::to_string(end - start) << std::endl;
		}
		catch (const std::exception&) {
			episodeRunning_ = false;
			std::cout << "LearningAgent::runLearningAgent() exception from interruptibleWait" << std::endl;
		}
	}

	episodeRunning_ = false;
	std::cout << "LearningAgent::runLearningAgent() exit" << std::endl;
	
}*/
/*
bool ml::LearningAgent::isIdle()
{
	if (state_ == LearningAgentState::Idle) {
		return true;
	}

	return false;
}
*/
/*
void ml::LearningAgent::processNextStep()
{
	state_ = LearningAgentState::Process;
}

bool ml::LearningAgent::processNextStep()
{
	if (state_ == LearningAgentState::Process) {
		return true;
	}

	return false;
}
*/
/*
bool ml::LearningAgent::isProcessing()
{
	if (state_ == LearningAgentState::Processing) {
		return true;
	}

	return false;
}
*/
/*
void ml::LearningAgent::processNextStep()
{
	try {
		std::cout << "LearningAgent::runLearningAgent() waiting permission to set procecssing on" << std::endl;

		std::mutex mutex;
		std::condition_variable cv;
		std::unique_lock<std::mutex> lock(mutex);
		auto function = std::bind(&LearningAgent::isIdle, this);

		thread::utils::interruptibleWait<decltype(function)>(cv, lock, function);
		processNextStep_ = true;
	}
	catch (const std::exception&) {
		std::cout << "LearningAgent::runLearningAgent() exception from processNextStep" << std::endl;
	}
}
*/
/*
bool ml::LearningAgent::isProcessNextStepSet()
{
	return processNextStep_;
}
*/
/*
void ml::LearningAgent::proceedToNextStep()
{
	if (episodeRunning_ == false) {
		return;
	}
	std::cout << "ml::LearningAgent::proceedToNextStep()" << std::endl;
	readyForNextStep_ = true;
}
*/

std::list<VisionPoints> ml::LearningAgent::currentVision()
{
	return snakeVision_.pointsForRendering(board_, snakeControl_.getPosition());
}

size_t ml::LearningAgent::stepsPerformed() const
{
	return step_;
}

size_t ml::LearningAgent::maxSteps() const
{
	return utils::commonConstants::ml::max_steps;
}

size_t ml::LearningAgent::totalSteps() const
{
	return agent_->TotalSteps();
}

void ml::LearningAgent::run()
{
	agentThread_ = std::make_unique<thread::interruptibleThread>(this, &LearningAgent::runEpisode, "ml agent");
}

void ml::LearningAgent::runEpisode()
{
	std::cout << "LearningAgent::runEpisode() enter" << std::endl;
	double reward = agent_->Episode();
	
	auto& agentSate{ envState_.environmentState() };
	agentSate = ml::EnvironmentState::State::Stopped;

	//Simulation& simulation = game_.simulation();
	//simulation.stop();
	//game_.nextState<gamestates::state::GameOverState>(game_);
	std::cout << "LearningAgent::runEpisode() exit" << std::endl;

	//return reward;
	/*
	mlpack::rl::TrainingConfig& config{ *(trainingConfig_.get()) };
	auto& replayMethod{ *(replayMethod_.get()) };
	
	//State& state{ agent_->State() };
	//state.Data() = InitialSample().Encode();
	//agent_->State().Data() = InitialSample().Encode();
	//Data() = InitialSample().Encode();
	State& state = envState();
	state.Data() = InitialSample().Encode();

	step_ = 0;
	double totalReturn{ 0.0 };
	auto& totalSteps{ agent_->TotalSteps() };

	while (!IsTerminal(state)) {
		uint32_t start = SDL_GetTicks();

		state_ = LearningAgentState::Processing;

		std::cout << "LearningAgent::runEpisode() thinking about next move" << std::endl;

		if (config.StepLimit() && step_ >= config.StepLimit()) {
			break;
		}

		agent_->State().Data() = state.Encode();

		agent_->SelectAction();
		const ContinuousActionEnvironment::Action& action = agent_->Action();
	
			
		ml::ContinuousActionEnvironment::State nextState;
		double reward = Sample(state, action, nextState);

		totalReturn += reward;
		step_++;
		totalSteps++;

		//replayMethod_->Store(agent_->State(), agent_->Action(), reward, nextState, episodeRunning_, 0.99);
		replayMethod.Store(state, action, reward, nextState, IsTerminal(nextState), config.Discount());

		state.Data() = nextState.Encode();
		//agent_->State().Data() = nextState.Encode();

		if (agent_->Deterministic() ||*//* totalSteps*//* step_ < config.ExplorationSteps()) {
			//continue;
		} else {

			for (size_t i = 0; i < config.UpdateInterval(); i++) {
				agent_->Update();
			}
		}
		
		state_ = LearningAgentState::Idle;

		uint32_t end = SDL_GetTicks();
		std::cout << "LearningAgent::runEpisode() total reward: " << std::to_string((long)totalReturn) << std::endl;
		std::cout << "LearningAgent::runEpisode() processing time: " << std::to_string(end - start) << std::endl;

		try {
			start = SDL_GetTicks();

			std::condition_variable cv;
			std::mutex mutex;
			std::unique_lock<std::mutex> lock(mutex);
			//auto function = std::bind(&LearningAgent::processNextStep, this);
			//wait([&]() {return check(a, b, c); });
			auto function = [&]() -> bool { return learningAgentState() == LearningAgentState::Process; };

			thread::utils::interruptibleWait<decltype(function)>(cv, lock, function);

			end = SDL_GetTicks();
			std::cout << "LearningAgent::runEpisode() waiting time: " << std::to_string(end - start) << std::endl;
		} catch (const std::exception&) {
			std::cout << "LearningAgent::runEpisode() exception from interruptibleWait" << std::endl;
			break;
		}
	}
	*/
	//std::cout << "LearningAgent::runEpisode() exit" << std::endl;
	//return totalReturn;
}
/*
ml::State ml::LearningAgent::InitialSample()
{
	std::cout << "LearningAgent::InitialSample()" << std::endl;
	auto position = snakeControl_.getPosition();
	State state{ sample(position) };

	return state;
}

bool ml::LearningAgent::IsTerminal(const ml::ContinuousActionEnvironment::State& state)
{
	bool terminal{ false };
	const auto position{ state.position() };
	
	terminal = board_.isWall(position);

	return terminal;
}

double ml::LearningAgent::Sample(const ml::ContinuousActionEnvironment::State& currentState, const ml::ContinuousActionEnvironment::Action action, ml::ContinuousActionEnvironment::State& nextState)
{
	std::cout << "LearningAgent::Sample()" << std::endl;
	double reward{ 0.0 };

	Cell* cell = board_.findFood();
	auto currentPosition{ currentState.position() };

	std::cout << "LearningAgent::Sample() current position x: " << std::to_string(currentPosition.x_) << " y: " << std::to_string(currentPosition.y_) << std::endl;

	auto distanceToFood_ = distance(currentPosition.x_, currentPosition.y_, cell->x_, cell->y_);

	auto snakeDirection{ actionToDirection(action.action) };
	
	snakeControl_.setDirection(snakeDirection);

	learningAgentState() = LearningAgentState::Idle;

	try {
		std::condition_variable cv;
		std::mutex mutex;
		std::unique_lock<std::mutex> lock(mutex);
		//auto function = std::bind(&LearningAgent::processNextStep, this);
		//wait([&]() {return check(a, b, c); });
		auto function = [&]() -> bool { return learningAgentState() == LearningAgentState::Process; };

		thread::utils::interruptibleWait<decltype(function)>(cv, lock, function);

		learningAgentState() = LearningAgentState::Processing;

	}
	catch (const std::exception&) {
		std::cout << "LearningAgent::runEpisode() exception from interruptibleWait" << std::endl;
	}

	//auto nextPosition{ snakeControl_.getNextPosition() };
	
	nextState.Data() = sample(currentState.position()).Encode();
	auto nextPosition{ nextState.position() };

	if (currentState.foodPosition() == nextPosition) {
		reward = 500;
	}
	else if (board_.isWall(nextPosition) || step_ >= maxSteps()) {// || board_.isSnakeBody(nextPosition)) {
		reward = -500;
		//episodeRunning_ = false;
	}
	else {
		double newDistanceToFood = distance(nextPosition.x_, nextPosition.y_, cell->x_, cell->y_);

		if (newDistanceToFood > distanceToFood_) {
			reward = -50;
		}
		else {
			reward = 50;
		}
	}

	return reward;
}

SnakeControl::Direction ml::LearningAgent::actionToDirection(const std::vector<double> action) const
{
	unsigned int rawDirection{ 0 };
	double max{ 0 };

	auto size{ action.size() };

	for (unsigned int i = 0; i < size; ++i) {
		if (action[i] > max) {
			max = action[i];
			rawDirection = i;
		}
		//std::cout << "LearningAgent::runLearningAgent() action: " << std::to_string(action[i]) << std::endl;
	}

	SnakeControl::Direction newDirection{ decodeDirection(rawDirection) };

	return newDirection;
}*/

void ml::LearningAgent::advanceEnvironment()
{
	try {
		std::cout << "LearningAgent::advanceEnvironment() " << std::endl;
		std::mutex mutex;
		std::condition_variable cv;
		std::unique_lock<std::mutex> lock(mutex);
		//auto function = std::bind(&LearningAgent::isIdle, this);
		auto function = [&]() -> bool { return envState_.environmentState() == ml::EnvironmentState::State::Idle; };

		std::cout << "LearningAgent::advanceEnvironment() waiting for environment to finish" << std::endl;
		thread::utils::interruptibleWait<decltype(function)>(cv, lock, function);

		auto& agentSate{ envState_.environmentState() };
		std::cout << "LearningAgent::advanceEnvironment() set process for environment" << std::endl;
		agentSate = ml::EnvironmentState::State::Process;
	}
	catch (const std::exception&) {
		std::cout << "LearningAgent::advanceEnvironment() exception" << std::endl;
	}
}
void ml::LearningAgent::waitUntilStopped()
{
	std::cout << "LearningAgent::waitUntilStopped() enter" << std::endl;

	envState_.environmentState() = ml::EnvironmentState::State::Stop;

	std::mutex mutex;
	std::condition_variable cv;
	std::unique_lock<std::mutex> lock(mutex);
	//auto function = std::bind(&LearningAgent::isIdle, this);
	auto function = [&]() -> bool { 
		return envState_.environmentState() == ml::EnvironmentState::State::Stopped;
	};

	thread::utils::interruptibleWait<decltype(function)>(cv, lock, function);
	std::cout << "LearningAgent::waitUntilStopped() exit" << std::endl;
}
/*
void ml::LearningAgent::updateEnvironment()
{
	
	auto position = snakeControl_.getPosition();
	auto vision = snakeVision_.lookInAllDirections(board_, position);

	arma::colvec& data = environment_.Data();

	std::copy(std::begin(vision), std::end(vision), std::begin(data));

	data[24] = position.x_;
	data[25] = position.y_;

	auto direction = snakeControl_.getDirection();

	if (direction == SnakeControl::Direction::left) {
		data[26] = -1;
	} else if (direction == SnakeControl::Direction::right) {
		data[26] = 1;
	} else {
		data[26] = 0;
	}

	if (direction == SnakeControl::Direction::up) {
		data[27] = -1;
	} else if (direction == SnakeControl::Direction::down) {
		data[27] = 1;
	} else {
		data[27] = 0;
	}

	Cell* cell = board_.findFood();

	data[28] = cell->x_;
	data[29] = cell->y_;
	
}

ml::ContinuousActionEnvironment::State ml::LearningAgent::sample(utils::Point<std::size_t> fromPoint)
{
	ml::ContinuousActionEnvironment::State state;

	auto vision = snakeVision_.lookInAllDirections(board_, fromPoint);

	arma::colvec& data = state.Data();

	std::copy(std::begin(vision), std::end(vision), std::begin(data));

	state.x() = fromPoint.x_;
	state.y() = fromPoint.y_;

	auto direction = snakeControl_.getDirection();

	if (direction == SnakeControl::Direction::left) {
		state.directionVectorX() = -1;
	} else if (direction == SnakeControl::Direction::right) {
		state.directionVectorX() = 1;
	} else {
		state.directionVectorX() = 0;
	}

	if (direction == SnakeControl::Direction::up) {
		state.directionVectorY() = -1;
	} else if (direction == SnakeControl::Direction::down) {
		state.directionVectorY() = 1;
	} else {
		state.directionVectorY() = 1;
	}

	Cell* cell = board_.findFood();

	state.foodPositionX() = cell->x_;
	state.foodPositionY() = cell->y_;

	return state;
}

bool ml::LearningAgent::isReadyForNextStep()
{
	return readyForNextStep_;
}*/
/*
SnakeControl::Direction ml::LearningAgent::decodeDirection(const unsigned int rawDirection) const
{
	const auto currentDirection{ snakeControl_.getDirection() };

	constexpr unsigned int left{ 0 };
	constexpr unsigned int forward{ 1 };
	constexpr unsigned int right{ 2 };

	if (rawDirection == forward) {
		return currentDirection;
	}

	SnakeControl::Direction newDirection{ SnakeControl::Direction::right };
		
	if (currentDirection == SnakeControl::Direction::up) {
		if (rawDirection == left) {
			newDirection = SnakeControl::Direction::left;
		} else if (rawDirection == right) {
			newDirection = SnakeControl::Direction::right;
		}
	} else if (currentDirection == SnakeControl::Direction::right) {
		if (rawDirection == left) {
			newDirection = SnakeControl::Direction::up;
		} else if (rawDirection == right) {
			newDirection = SnakeControl::Direction::down;
		}
	} else if (currentDirection == SnakeControl::Direction::down) {
		if (rawDirection == left) {
			newDirection = SnakeControl::Direction::left;
		}
		else if (rawDirection == right) {
			newDirection = SnakeControl::Direction::right;
		}
	} else if (currentDirection == SnakeControl::Direction::left) {
		if (rawDirection == left) {
			newDirection = SnakeControl::Direction::down;
		}
		else if (rawDirection == right) {
			newDirection = SnakeControl::Direction::up;
		}
	}

	return newDirection;
}
*/