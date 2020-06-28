#pragma once

#include <functional>

// A Dispatched job will receive this as function argument:
struct JobDispatchArgs
{
	uint32_t jobIndex;
	uint32_t groupIndex;
};

namespace JobSystem
{
	// Create the internal resources such as worker threads, etc. Call it once when initializing the application.
	void Initialize();

	// Add a job to execute asynchronously. Any idle thread will execute this job.
	void Execute(const std::function<void()>& job);

	// Divide a job onto multiple jobs and execute in parallel.
	//	jobCount	: how many jobs to generate for this task.
	//	groupSize	: how many jobs to execute per thread. Jobs inside a group execute serially. It might be worth to increase for small jobs
	//	func		: receives a JobDispatchArgs as parameter
	void Dispatch(uint32_t jobCount, uint32_t groupSize, const std::function<void(JobDispatchArgs)>& job);

	// Check if any threads are working currently or not
	bool IsBusy();

	// Wait until all threads become idle
	void Wait();
}

/*
#include <thread>
#include <queue>
#include <chrono>
#include <mutex>
#include <atomic>

//using namespace std::chrono;
//using namespace std;


class Task {
public:
	virtual void run() = 0;
};

class MyTask : Task {
public:
	virtual void run() override {
		printf("mytask \n");
	};
};

template<typename T, typename = std::enable_if<std::is_base_of<Task, T>::value>>
class SchedulerItem {
public:
	T task;
	std::chrono::time_point<std::chrono::steady_clock> startTime;
	int delay;
	SchedulerItem(T t, std::chrono::time_point<std::chrono::steady_clock> s, int d) : task(t), startTime(s), delay(d) {}
};

template<typename T, typename = std::enable_if<std::is_base_of<Task, T>::value>>
class Scheduler {
public:
	queue<SchedulerItem<T>> pool;
	mutex mtx;
	atomic<bool> running;
	Scheduler() : running(false) {}

	void add(T task, double delayMsToRun) {
		lock_guard<mutex> lock(mtx);
		pool.push(SchedulerItem<T>(task, std::chrono::high_resolution_clock::now(), delayMsToRun));
		if (running == false)
			runNext();
	}
	void runNext(void) {
		running = true;
		auto th = [this]() {
			mtx.lock();
			auto item = pool.front();
			pool.pop();
			mtx.unlock();
			auto remaining = (item.startTime + milliseconds(item.delay)) - std::chrono::high_resolution_clock::now();
			
			if (remaining.count() > 0)
				this_thread::sleep_for(remaining);
			
			item.task.run();
			
			if (pool.size() > 0)
				runNext();
			else
				running = false;
		};

		thread t(th);
		t.detach();
	}
};*/
