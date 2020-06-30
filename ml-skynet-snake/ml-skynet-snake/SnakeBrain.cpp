#include "SnakeBrain.hpp"
#include "Board.hpp"
#include "Renderer.hpp"
#include "Simulation.hpp"
#include "SnakeMovement.hpp"
#include "Game.hpp"
#include "Snake.hpp"

//SnakeBrain::SnakeBrain(Board* board, SnakePosition* snakePosition) : board_(board), snakePosition_(snakePosition)
//{
	// Set up the feed forward neural network.
	/*mlpack::ann::FFN< mlpack::ann::MeanSquaredError<>, mlpack::ann::GaussianInitialization> model(mlpack::ann::MeanSquaredError<>(), mlpack::ann::GaussianInitialization(0, 0.001));    // Gaussian Initialization is how we initialize the weights in the neural network, with mean 0 and standard deviation 0.001
	model.Add< mlpack::ann::Linear<>>(24, 128);
	model.Add< mlpack::ann::ReLULayer<>>();
	model.Add< mlpack::ann::Linear<>>(128, 128);
	model.Add< mlpack::ann::ReLULayer<>>();
	model.Add< mlpack::ann::Linear<>>(128, 4);

	// Set up the policy and replay method.
	mlpack::rl::GreedyPolicy<SnakeBrain::LearningAgent> policy(1.0, 1000, 0.1, 0.99);
	mlpack::rl::RandomReplay<SnakeBrain::LearningAgent> replayMethod(10, 10000);

	mlpack::rl::TrainingConfig config;
	config.StepSize() = 1;
	config.Discount() = 0.9;
	config.TargetNetworkSyncInterval() = 100;
	config.ExplorationSteps() = 100;
	config.DoubleQLearning() = false;
	config.StepLimit() = 500;
	*/
	//std::array<float, 24> vision;
	// Set up DQN agent.
	//QLearning<CartPole, decltype(model), AdamUpdate, decltype(policy)>
	//	agent(std::move(config), std::move(model), std::move(policy),
	//		std::move(replayMethod));

	// mlpack::rl::QLearning<SnakeDirection, mlpack::ann::FFN<mlpack::ann::MeanSquaredError<>, mlpack::ann::GaussianInitialization>, ens::AdamUpdate, mlpack::rl::GreedyPolicy<SnakeDirection> >*
	//learningAgent_ = new mlpack::rl::QLearning<SnakeBrain::LearningAgent, decltype(model), ens::AdamUpdate, decltype(policy)> (std::move(config), std::move(model), std::move(policy), std::move(replayMethod));
//}

SnakeBrain::~SnakeBrain()
{
	//delete learningAgent_;
}

//void SnakeBrain::runAI()
//{
//	std::array<float, 24> vision = snakeVision_.lookInAllDirections();

	//SnakeBrain& env = learningAgent_->Environment();
	//std::copy(std::begin(vision), std::end(vision), std::begin(env.vision_));

	//auto value = learningAgent_->Episode();

	
//}

void SnakeBrain::setBoard(Board* board)
{
	board_ = board;
}

void SnakeBrain::setSnakeMovementInterface(SnakeMovement* snakeMovement)
{
	snakeMovement_ = snakeMovement;
}

void SnakeBrain::setRenderer(Renderer* renderer)
{
	renderer_ = renderer;
}

void SnakeBrain::setSimulation(Simulation* simulation)
{
	simulation_ = simulation;
}

void SnakeBrain::setGame(Game* game)
{
	game_ = game;
}

void SnakeBrain::setSnake(Snake* snake)
{
	snake_ = snake;
}

double SnakeBrain::Sample(const State& state, const Action& action, State& nextState)
{
	// Update the number of steps performed.
	stepsPerformed++;

	auto position = snakeMovement_->getPosition();
	SnakeMovement::Direction direction;

	if (action == up) {
		direction = SnakeMovement::Direction::up;
	}
	else if (action == down) {
		direction = SnakeMovement::Direction::down;
	}
	else if (action == right) {
		direction = SnakeMovement::Direction::right;
	}
	else if (action == left) {
		direction = SnakeMovement::Direction::left;
	}
	
	snake_->setDirection(direction);

	auto target = simulation_->getNextSnakePosition(position, direction);
	double& x = nextState.coordinateX();
	double& y = nextState.coordinateY();

	x = static_cast<double>(target.x_);
	y = static_cast<double>(target.y_);

	bool done = IsTerminal(nextState);
	bool food{ false };

	if (done == false) {
		auto vision = snakeVision_.lookInAllDirections(*board_, target, *simulation_, *renderer_);
		auto data = nextState.Data();
		std::copy(std::begin(vision), std::end(vision), std::begin(data));

		//food = simulation_->checkForFood(*board_, target);

		//simulation_->updateSnakePosition(*snake_, target);
	}

	//game_->gameLoop();

	// Do not reward agent if it failed.
	
	if (done && maxSteps != 0 && stepsPerformed >= maxSteps) {
		return doneReward;
	} else if (done) {
		return -10;
	} else if (food) {
		if (stepsPerformed > 100) {
			stepsPerformed -= 100;
		} else {
			stepsPerformed = 0;
		}

		return foodReward;
	}
	
	return 0;
}

double SnakeBrain::Sample(const State& state, const Action& action)
{
	State nextState;
	return Sample(state, action, nextState);
}

SnakeBrain::State SnakeBrain::InitialSample()
{
	stepsPerformed = 0;

	auto vision = snakeVision_.lookInAllDirections(*board_, snakeMovement_->getPosition(), *simulation_, *renderer_);
	auto position = snakeMovement_->getPosition();

	arma::colvec data(26);
	std::copy(std::begin(vision), std::end(vision), std::begin(data));
	data[24] = position.x_;
	data[25] = position.y_;

	return State(data);
}

bool SnakeBrain::IsTerminal(const State& state) const
{
	if (maxSteps != 0 && stepsPerformed >= maxSteps) {
		std::cout << "Episode terminated due to the maximum number of steps being taken.";
		return true;
	}

	Point<std::size_t> target;
	target.x_ = static_cast<std::size_t>(state.coordinateX());
	target.y_ = static_cast<std::size_t>(state.coordinateY());
	
	const bool collision = false;// simulation_->checkForCollisionWithWall(*board_, target);
	return collision;
}

size_t SnakeBrain::StepsPerformed() const
{
	return stepsPerformed;
}

size_t SnakeBrain::MaxSteps() const
{
	return maxSteps;
}

size_t& SnakeBrain::MaxSteps()
{
	return maxSteps;
}
