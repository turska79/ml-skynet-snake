#include "Simulation.hpp"
#include "../utils/Utils.hpp"
#include <iostream>
#include <algorithm>
#include <string>
#pragma warning(push)  
#pragma warning(disable : 26819 26812)
#include <SDL_timer.h>
#pragma warning( pop )


Simulation::Simulation(Board& board, SnakeControl& snakeControl) : board_(board), snakeControl_(snakeControl)
{
}

void Simulation::start()
{
	std::cout << "Simulation::start()" << std::endl;
	const std::lock_guard<std::mutex> lock(threadHandlingMutex_);

	if (simulationThread_) {
		simulationThread_->interrupt();
	}

	simulationThread_ = std::make_unique<thread::interruptibleThread>(&Simulation::run, this);
}

void Simulation::stop()
{
	std::cout << "Simulation::stop()" << std::endl;
	const std::lock_guard<std::mutex> lock(threadHandlingMutex_);

	if (simulationThread_) {
		simulationThread_->interrupt();
	}

	updateRate_ = 0;
}

void Simulation::addObject(std::reference_wrapper<simulationObject> object)
{
	objects_.emplace_back(object);
}

void Simulation::detachObject(std::reference_wrapper<simulationObject> object)
{
	objects_.erase(std::find_if(objects_.cbegin(), objects_.cend(), [&](const std::reference_wrapper<simulationObject> &i)
	{
		return std::addressof(i.get()) == std::addressof(object.get());
	}));
}

void Simulation::run()
{
	runSimulationLoop();
}

void Simulation::runSimulationLoop()
{
	bool running{ true };

	lastSimulationUpdate_ = SDL_GetTicks();
	nextSimulationStep_ = SDL_GetTicks();

	while (running) {
		thread::utils::interruptionPoint();

		uint32_t now = SDL_GetTicks();

		if (nextSimulationStep_ <= now) {
			int computer_is_too_slow_limit{ utils::commonConstants::lowSpeedLimit };
			uint32_t deltaTime{ now - lastSimulationUpdate_ };
			//std::cout << "Simulation::runSimulationLoop() delta: " << std::to_string(deltaTime) << std::endl;
			unsigned int count{ 0 };

			while ((nextSimulationStep_ <= now) && (computer_is_too_slow_limit--)) {
				++count;
				//std::cout << "Simulation::runSimulationLoop() count: " << std::to_string(count) << std::endl;
				thread::utils::interruptionPoint();

				updateObjects(deltaTime);
				nextSimulationStep_ += utils::commonConstants::simulationRefreshRateTargetTimeStep;
			}

			lastSimulationUpdate_ = now;
			
			if (deltaTime > 0) {
				updateRate_ = 1000 / deltaTime;
			}
		}
		else {
			//std::cout << "Simulation::runSimulationLoop() sleep: " << std::to_string(nextSimulationStep_ - now)  << std::endl;
			SDL_Delay(nextSimulationStep_ - now);
		}
	}
}

void Simulation::updateObjects(const uint32_t deltaTime)
{
	for (auto& object : objects_) {
		thread::utils::interruptionPoint();

		object.get().update(deltaTime);
	}
}
bool Simulation::checkCellType(const utils::Point<std::size_t>& target, Cell::Type type) const
{
	const Cell* cell = board_.findCell(target);

	if (cell->type_ == type) {
		return true;
	}

	return false;
}

bool Simulation::checkForCollisionWithFood(const utils::Point<std::size_t>& target) const
{
	return checkCellType(target, Cell::Type::food);
}

uint32_t Simulation::updateRate()
{
	return updateRate_;
}
/*
const bool Simulation::checkForCollisionWithWall(const utils::Point<std::size_t>& target) const
{
	return checkCellType(target, Cell::Type::wall);
}*/

bool Simulation::checkForCollisionWithSnakeBody(const utils::Point<std::size_t>& target) const
{
	return checkCellType(target, Cell::Type::body);
}
