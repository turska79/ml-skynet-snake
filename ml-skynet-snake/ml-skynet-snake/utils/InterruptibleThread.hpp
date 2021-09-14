#pragma once

#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <iostream>

#include "ThreadInterrupted.hpp"

namespace thread {
	namespace utils {

		void interruptionPoint();
		void interruptibleWait(std::condition_variable& cv, std::unique_lock<std::mutex>& lk);

		class InterruptFlag
		{
		public:
			void set();
			const bool isSet() const;

			template<typename Lockable> void wait(std::condition_variable_any& cv, Lockable& lk);
			void setConditionVariable(std::condition_variable& cv);
			void clearConditionVariable();

			struct clearCvOnDestruct
			{
				~clearCvOnDestruct();
			};

		protected:
			std::atomic<bool> flag_{ false };
			std::condition_variable* threadConditionVariable_{ nullptr };
			std::condition_variable_any* threadConditionVariableAny_{ nullptr };
			std::mutex setClearMutex_;
		};

		inline thread_local InterruptFlag this_thread_interrupt_flag;
		/*
		template<typename Lockable> void interruptibleWait(std::condition_variable_any& cv, Lockable& lk)
		{
			this_thread_interrupt_flag.wait(cv, lk);
		}
		*/
		template<typename T> void interruptibleWait(std::future<T>& uf, std::unique_lock<std::mutex>& lk)
		{
			while (!this_thread_interrupt_flag.isSet())
			{
				if (uf.wait_for(lk, std::chrono::milliseconds(1)) == std::future_status::ready) {
					break;
				}
			}

			interruptionPoint();
		}

		template<typename Predicate> void interruptibleWait(std::condition_variable& cv, std::unique_lock<std::mutex>& lk, Predicate pred)
		{
			interruptionPoint();
			this_thread_interrupt_flag.setConditionVariable(cv);
			InterruptFlag::clearCvOnDestruct guard;

			while (!this_thread_interrupt_flag.isSet() && !pred())
			{
				cv.wait_for(lk, std::chrono::milliseconds(1));
			}

			interruptionPoint();
		}

		template<typename Predicate> void blockingWait(std::condition_variable& cv, std::unique_lock<std::mutex>& lk, Predicate pred)
		{
			while (!pred())
			{
				cv.wait_for(lk, std::chrono::milliseconds(1));
			}
		}

		template<typename Lockable> inline void InterruptFlag::wait(std::condition_variable_any& cv, Lockable& lk)
		{
			struct customLock
			{
				InterruptFlag* self_{ nullptr };
				Lockable& lk_;

				customLock(InterruptFlag* self, std::condition_variable_any& cond, Lockable& lk) : self_(self), lk_(lk)
				{
					self->set_clear_mutex.lock();
					self->thread_cond_any = &cond;
				}

				void unlock()
				{
					lk.unlock();
					self->set_clear_mutex.unlock();
				}

				void lock()
				{
					std::lock(self->set_clear_mutex, lk);
				}

				~customLock()
				{
					self->thread_cond_any = 0;
					self->set_clear_mutex.unlock();
				}
			};

			customLock cl(this, cv, lk);
			interruptionPoint();
			cv.wait(cl);
			interruptionPoint();
		}
	}
	
	class interruptibleThread
	{
	public:
		template<typename F, typename T> interruptibleThread(T instance, F function, std::string name)
		{
			name_ = name;
			std::promise<utils::InterruptFlag*> exitSignal;
			auto& localRunning{ running_ };
			auto& localName{ name_ };

			internalThread_ = new std::thread([instance, function, &exitSignal, &localRunning, &localName]() {
				exitSignal.set_value(&thread::utils::this_thread_interrupt_flag);

				try {
					std::cout << "interruptibleThread " << localName << " enter" << std::endl;
					localRunning = true;
					(*instance.*function)();
				} catch (const thread::utils::ThreadInterrupted&) {
					std::cout << "interruptibleThread " << localName << " ThreadInterrupted exception" << std::endl;
				} catch (const std::exception& e) {
					std::cout << "interruptibleThread " << localName << " exception: " << e.what() << std::endl;
				} catch (...) {
					std::cout << "interruptibleThread " << localName << " unhandled exception: " << std::endl;
				}
				std::cout << "interruptibleThread " << localName << " exit" << std::endl;
				localRunning = false;
			});

			exitSignal_ = exitSignal.get_future().get();
		}
		~interruptibleThread() {
			if (internalThread_) {
				internalThread_->join();
			}

			delete internalThread_;
		}
		void interrupt();
		void waitUntilInterrupted() {
			std::cout << "interruptibleThread " << name_ << " waitUntilInterrupted enter" << std::endl;
			
			if (!running_) {
				return;
			}
			
			std::mutex mutex;
			std::condition_variable cv;

			try {
				std::unique_lock<std::mutex> lock(mutex);
				auto function = std::bind(&interruptibleThread::isRunning, this);

				interrupt();
				thread::utils::blockingWait<decltype(function)>(cv, lock, function);
			}
			catch (...) {
			}
			std::cout << "interruptibleThread " << name_ << " waitUntilInterrupted exit" << std::endl;
		}
		
		bool isRunning() { return running_; };
	protected:
		
		std::thread* internalThread_{ nullptr };
		utils::InterruptFlag* exitSignal_{ nullptr };
		std::atomic<bool> running_{ false };
		std::string name_;
	};
}