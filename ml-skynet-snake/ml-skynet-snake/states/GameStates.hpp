#pragma once

#include <vector>
#include <mutex>

namespace gamestates {

	namespace handling {
		template<class TT> class StateStorage {
		public:
			template<class T, class... Args>
			void nextState(Args&&... args)
			{
				pushStateImpl(std::make_unique<T>(std::forward<Args>(args)...));
			}

			void changeState()
			{
				const std::lock_guard<std::mutex> guard(lock_);

				if (popState_) {
					popStateImpl();
					updateCurrentStateFromStack();
				}
				else  if (currentState_ != states_.back().get()) {
					if (currentState_) {
						currentState_->exit();
					}

					updateCurrentStateFromStack();
				}
			}

			void popState()
			{
				popState_ = true;
			}

			TT* currentState() { return currentState_; };
		private:
			template<class T> void pushStateImpl(std::unique_ptr<T> state)
			{
				const std::lock_guard<std::mutex> guard(lock_);
				states_.emplace_back(std::move(state));
			}

			void popStateImpl()
			{
				if (states_.empty() == false) {
					states_.back()->exit();
					states_.pop_back();
				}

				popState_ = false;
			}

			void updateCurrentStateFromStack()
			{
				currentState_ = states_.back().get();
				currentState_->enter();
			}

			bool popState_{ false };
			TT* currentState_{ nullptr };
			std::vector<std::unique_ptr<TT>> states_;
			std::mutex lock_;
		};
	}
}