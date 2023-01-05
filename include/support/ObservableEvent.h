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

		for (auto& fx : this->Observers) {
			fx.invoke(std::forward<Arguments>(args)...);
		}
	}

public:
	void subscribe(const Delegate& d) {
		this->Observers.push_back(d);
	}

	bool unsubscribe(const Delegate& d) {
		return std::erase(this->Observers, d) != 0;
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
