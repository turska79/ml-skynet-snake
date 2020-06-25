#pragma once

#include "RunningState.hpp"
//#include "SnakeVision.hpp"
//#include "SnakeBrain.hpp"
/*
#include <mlpack/core.hpp>
#include <mlpack/methods/ann/layer/layer.hpp>
#include <mlpack/methods/ann/loss_functions/mean_squared_error.hpp>
#include <mlpack/methods/ann/ffn.hpp>
*/


class RunningStateAI : public RunningState
{
public:
	explicit RunningStateAI(Game& game);
	virtual ~RunningStateAI() = default;
	RunningStateAI(const RunningStateAI&) = default;
	RunningStateAI& operator=(const RunningStateAI&) = default;
	RunningStateAI(RunningStateAI&&) = default;
	RunningStateAI& operator=(RunningStateAI&&) = default;

	void enter() override;
	void update(Renderer& renderer, uint32_t deltaTime) override;
	void handleInput(const Keyboard& keyboard) override;
private:
	bool running_{ false };
	//SnakeVision snakeVision_;
	//SnakeBrain snakeBrain_;
	//mlpack::ann::FFN<mlpack::ann::MeanSquaredError<>, mlpack::ann::RandomInitialization> model;
};

