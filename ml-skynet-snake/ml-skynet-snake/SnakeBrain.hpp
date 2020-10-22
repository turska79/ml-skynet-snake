#pragma once

#include "SnakeVision.hpp"
//#include "Observer.hpp"
#include <vector>
#include <array>
#include <mutex>
#include <condition_variable>

#pragma warning (push, 0)
#include <mlpack/prereqs.hpp>
#pragma warning (pop)

class Board;
class SnakeControl;
class Renderer;
class Simulation;
class Game;
class Snake;

class SnakeBrain //: public SnakeObserver
{
public:

	class State
	{
	public:
		State() : data(dimension)
		{
		}

		State(const arma::colvec& data) : data(data)
		{
		}

		arma::colvec& Data()
		{ 
			return data;
		}
			
		std::array<float, 24> vision() const
		{
			std::array<float, 24> vision{ 0 };
			auto iter = std::begin(data);
			std::advance(iter, 24);
			std::copy(std::begin(data), iter, std::begin(vision));
			return vision;
		}
		/*
		std::size_t coordinateX() const
		{
			return position.x_;
		}

		std::size_t& coordinateX()
		{
			return position.x_;
		}

		std::size_t coordinateY() const
		{
			return position.y_;
		}
	
		std::size_t& coordinateY()
		{
			return position.y_;
		}
		*/
		const arma::colvec& Encode() const
		{ 
			return data;
		}

		static constexpr size_t dimension{ 24 };

	private:
		arma::colvec data;
		utils::Point<std::size_t> position;
	};

	enum Action
	{
		up,
		down,
		right,
		left,
		// Track the size of the action space.
		size
	};

	SnakeBrain();
	double Sample(const State& state, const Action& action, State& nextState);
	double Sample(const State& state, const Action& action);

	State InitialSample();

	bool IsTerminal(const State& state) const;

	size_t StepsPerformed() const;
	size_t MaxSteps() const;
	size_t& MaxSteps();

	void ProceedToNextMove();
	void setBoard(Board* board);
	void setSnakeMovementInterface(SnakeControl* SnakeControl);
	void setRenderer(Renderer* renderer);
	void setSimulation(Simulation* simulation);
	void setGame(Game* game);
	void setSnake(Snake* snake);
	SnakeVision& snakeVision();

	//virtual void positionUpdated() override;
	//virtual void collision() override;
private:
		
	double doneReward{ -30 };
	const double foodReward{ 100 };
	size_t maxSteps{ 500 };
	size_t stepsPerformed{ 0 };

	Board* board_{ nullptr };
	SnakeControl* snakeControl_{ nullptr };
	Renderer* renderer_{ nullptr };
	Simulation* simulation_{ nullptr };
	SnakeVision snakeVision_;
	Game* game_{ nullptr };
	Snake* snake_{ nullptr };

	std::mutex* moveMutex_;
	std::condition_variable* moveCv_;
};

