#pragma once
#include "formsFramework.h"
#include "support/Delegate.h"

template <typename Delegate>
class ObservableEvent {
	using type = ObservableEvent<Delegate>;

private:
	std::vector<Delegate>	Observers;

public:
	ObservableEvent() = default;

public:
	template <typename... Arguments>
	void
	raise(Arguments&&... args) const {
		static_assert(std::is_invocable_v<Delegate,Arguments...>);

		for (auto& o : this->Observers) {
			(void)o.invoke(std::forward<Arguments>(args)...);
		}
	}

public:
	void subscribe(const Delegate& d) {
		this->Observers.push_back(d);
	}

	bool unsubscribe(const Delegate& d) {
		auto is_equal = [&d](auto& e){ return e.compare(d) == DelegateEquality::Equal; };
		return std::erase_if(this->Observers, is_equal) != 0;
	}

	type& operator+=(const Delegate& d) {
		this->subscribe(d);
		return *this;
	}
	
	type& operator-=(const Delegate& d) {
		this->unsubscribe(d);
		return *this;
	}
};

using NullaryEvent = ObservableEvent<NullaryDelegate>;
