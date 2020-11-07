#pragma once

#include <vector>
#include <mutex>

namespace gamestates {

	namespace handling {
		template<class TT> class StateStorage {
		public:
			template<class T, class... Args>
			void nextState(Args&&... args) {
				const std::lock_guard<std::mutex> guard(lock_);
				pushStateImpl(std::make_unique<T>(std::forward<Args>(args)...));
			}

			void changeState() {
				const std::lock_guard<std::mutex> guard(lock_);

				if (previousState_) {
					previousState_->exit();
					previousState_ = nullptr;
				}

				if (nextState_) {
					nextState_->enter();
					nextState_ = nullptr;
				}
			}

			void popState() {
				const std::lock_guard<std::mutex> guard(lock_);
				popStateImpl();
			}

		protected:
			template<class T> void pushStateImpl(std::unique_ptr<T> state)
			{
				previousState_ =  nullptr ;
				nextState_ = nullptr;

				if (states_.empty() == false) {
					previousState_ = states_.back().get();
				}

				states_.emplace_back(std::move(state));
				nextState_ = states_.back().get();
			}

			void popStateImpl()
			{
				nextState_ = nullptr;

				if (states_.empty() == false) {
					states_.back()->exit();
					states_.pop_back();
				}

				if (states_.empty() == false) {
					nextState_ = states_.back().get();
				}
			}

			TT* previousState_{ nullptr };
			TT* nextState_{ nullptr };
			std::vector<std::unique_ptr<TT>> states_;
			std::mutex lock_;
		};
	}
}