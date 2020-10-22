#include "InterruptibleThread.hpp"
#include <iostream>

//thread_local thread::utils::InterruptFlag this_thread_interrupt_flag;

void thread::utils::interruptionPoint()
{
	//std::cout << "interruptionPoint::this_thread_interrupt_flag() 0x" << &this_thread_interrupt_flag << std::endl;
	if (this_thread_interrupt_flag.isSet())
	{
		throw ThreadInterrupted();
	}
}

void thread::utils::interruptibleWait(std::condition_variable& cv, std::unique_lock<std::mutex>& lk)
{
	interruptionPoint();
	this_thread_interrupt_flag.setConditionVariable(cv);
	InterruptFlag::clearCvOnDestruct guard;
	interruptionPoint();
	cv.wait_for(lk, std::chrono::milliseconds(1));
	interruptionPoint();
}

void thread::utils::InterruptFlag::set()
{
	flag_.store(true, std::memory_order_relaxed);
	//std::cout << "interruptionPoint::set() 0x" << this << std::endl;
	//flag_ = true;

	std::lock_guard<std::mutex> lk(setClearMutex_);

	if (threadConditionVariable_) {
		threadConditionVariable_->notify_all();
	} else if (threadConditionVariableAny_) {
		threadConditionVariableAny_->notify_all();
	}
}

const bool thread::utils::InterruptFlag::isSet() const
{
	//std::cout << "interruptionPoint::isSet() 0x" << this << std::endl;
	return flag_.load(std::memory_order_relaxed);
}

void thread::utils::InterruptFlag::setConditionVariable(std::condition_variable& cv)
{
	std::lock_guard<std::mutex> lk(setClearMutex_);
	threadConditionVariable_ = &cv;
}

void thread::utils::InterruptFlag::clearConditionVariable()
{
	std::lock_guard<std::mutex> lk(setClearMutex_);
	threadConditionVariable_ = 0;
}

thread::utils::InterruptFlag::clearCvOnDestruct::~clearCvOnDestruct()
{
	this_thread_interrupt_flag.clearConditionVariable();
}

void thread::interruptibleThread::interrupt()
{
	if (exitSignal_) {
		exitSignal_->set();
	}
}
