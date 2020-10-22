#include "Simulation.hpp"
#include "InterruptibleThread.hpp"
#include "Utils.hpp"
#include <iostream>
#include <algorithm>
#include <SDL_timer.h>

constexpr unsigned int secondAsMilliseconds{ 1000 };
constexpr uint32_t targetUpdateRate = secondAsMilliseconds / 30;

Simulation::Simulation(Board& board, SnakeControl& snakeControl) : board_(board), snakeControl_(snakeControl)
{
}

void Simulation::start()
{
	std::cout << "Simulation::start()" << std::endl;
	const std::lock_guard<std::mutex> lock(threadHandlingMutex_);

	if (simulationThread_) {
		simulationThread_->interrupt();
		delete simulationThread_;
	}

	simulationThread_ = new thread::interruptibleThread(&Simulation::run, this);
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

	while (running) {
		thread::utils::interruptionPoint();

		uint32_t now = SDL_GetTicks();

		if (nextSimulationStep_ <= now) {
			int computer_is_too_slow_limit = utils::commonConstants::lowSpeedLimit;

			while ((nextSimulationStep_ <= now) && (computer_is_too_slow_limit--)) {
				thread::utils::interruptionPoint();

				updateObjects(now - lastSimulationUpdate_);
				nextSimulationStep_ += utils::commonConstants::simulationRefreshRateTargetTimeStep;
			}

			uint32_t deltaTime = now - lastSimulationUpdate_;

			lastSimulationUpdate_ = now;
			
			if (deltaTime > 0) {
				updateRate_ = 1000 / deltaTime;
			}
		}
		else {
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
const bool Simulation::checkCellType(const Point<std::size_t>& target, Cell::Type type) const
{
	const Cell* cell = board_.findCell(target);

	if (cell->type_ == type) {
		return true;
	}

	return false;
}

const bool Simulation::checkForCollisionWithFood(const Point<std::size_t>& target) const
{
	return checkCellType(target, Cell::Type::food);
}

const uint32_t Simulation::updateRate()
{
	return updateRate_;
}

const bool Simulation::checkForCollisionWithWall(const Point<std::size_t>& target) const
{
	return checkCellType(target, Cell::Type::wall);
}

const bool Simulation::checkForCollisionWithSnakeBody(const Point<std::size_t>& target) const
{
	return checkCellType(target, Cell::Type::body);
}
