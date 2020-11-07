#pragma once

#include <vector>
#include <functional>
#include <memory>
#include <mutex>

namespace subjects {

	class BaseInvoke {
	public:
		virtual ~BaseInvoke() {};
		virtual void invoke() = 0;
	};

	template<class T>
	class MethodWrapper : public BaseInvoke {
	public:
		typedef void (T::*methodWrapperArgs0)();
		//using methodWrapperArgs0 = std::function<void(&T)>;
		T* instance_{ nullptr };
		methodWrapperArgs0 method_{ nullptr };

	public:
		MethodWrapper(T* instance, methodWrapperArgs0 method) : instance_(instance), method_(method) {
		}

		virtual void invoke() override {
			(instance_->*method_)();
		}
	};

	class SubjectBase {
	public:
		template<class T>
		void addObserver(T* instance, typename MethodWrapper<T>::methodWrapperArgs0 method) {
			const std::lock_guard<std::mutex> guard(lock_);
			observers_.emplace_back(std::make_unique<MethodWrapper<T>>(instance, method));
		}

		template<class T>
		void removeObserver(T* instance, typename MethodWrapper<T>::methodWrapperArgs0 method) {
			const std::lock_guard<std::mutex> guard(lock_);
			observers_.erase(std::remove_if(observers_.begin(), observers_.end(), [&instance, &method](const BaseInvokePtr& it)
				{
					MethodWrapper<T> inputWrapper(instance, method);
					MethodWrapper<T>* iterWrapper = dynamic_cast<MethodWrapper<T>*>(it.get());

					if (inputWrapper.instance_ == iterWrapper->instance_ && inputWrapper.method_ == iterWrapper->method_) {
						return true;
					}

					return false;
				}), observers_.end());
		}

		void invoke() {
			const std::lock_guard<std::mutex> guard(lock_);
			for (auto& it : observers_) {
				it->invoke();
			}
		}

	protected:
		using BaseInvokePtr = std::unique_ptr<BaseInvoke>;
		std::vector<BaseInvokePtr> observers_;

		std::mutex lock_;
	};

}

