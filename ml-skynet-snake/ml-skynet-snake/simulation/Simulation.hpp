#pragma once

#include <cstddef>
#include <mutex>
#include <atomic>
#include <list>
#include <memory>
#include <functional>
#include "utils/Point.hpp"
#include "Board.hpp"
#include "SnakeControl.hpp"
#include "SimulationObject.hpp"
#include "InterruptibleThread.hpp"
/*
namespace thread {
	class interruptibleThread;
}
*/
class Simulation
{
public:
	Simulation(Board& board, SnakeControl& snakeControl);
	void start();
	void stop();
	void addObject(std::reference_wrapper<simulationObject> object);
	void detachObject(std::reference_wrapper<simulationObject> object);
	
	bool checkForCollisionWithSnakeBody(const utils::Point<std::size_t>& target) const;
	bool checkForCollisionWithFood(const utils::Point<std::size_t>& target) const;

	uint32_t updateRate();
private:
	void run();
	void runSimulationLoop();
	void updateObjects(const uint32_t deltaTime);
	bool checkCellType(const utils::Point<std::size_t>& target, Cell::Type type) const;
	
	std::unique_ptr<thread::interruptibleThread> simulationThread_;
	std::mutex threadHandlingMutex_;
	
	uint32_t nextSimulationStep_{ 0 };
	uint32_t lastSimulationUpdate_{ 0 };
	uint32_t updateRate_{ 0 };

	std::list<std::reference_wrapper<simulationObject>> objects_;

	Board& board_;
	SnakeControl& snakeControl_;
};

