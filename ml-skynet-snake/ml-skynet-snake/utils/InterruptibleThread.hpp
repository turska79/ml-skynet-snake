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
		
		template<typename Lockable> void interruptibleWait(std::condition_variable_any& cv, Lockable& lk)
		{
			this_thread_interrupt_flag.wait(cv, lk);
		}

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
		template<typename F, typename T> interruptibleThread(F function, T instance)
		{
			std::promise<utils::InterruptFlag*> exitSignal;

			internalThread_ = new std::thread([instance, function, &exitSignal]() {
				exitSignal.set_value(&thread::utils::this_thread_interrupt_flag);

				try {
					(*instance.*function)();
				} catch (const thread::utils::ThreadInterrupted&) {
					std::cout << "interruptibleThread ThreadInterrupted exception" << std::endl;
				} catch (const std::exception& e) {
					std::cout << "interruptibleThread exception: " << e.what() << std::endl;
				}
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

	protected:
		std::thread* internalThread_{ nullptr };;
		utils::InterruptFlag* exitSignal_{ nullptr };
	};
}