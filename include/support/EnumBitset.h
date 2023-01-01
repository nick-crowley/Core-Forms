#pragma once
#include "formsFramework.h"

template <typename T>
concept Enumeration = std::is_enum_v<T>;

template <Enumeration E>
class EnumBitset
{
	template <Enumeration>
	friend class EnumBitset;

	using type = EnumBitset<E>;
	using reference = type&;
	using value_type = E;
	using underlying_t = std::underlying_type_t<value_type>;

private:
	value_type constinit
	inline static Zero = static_cast<value_type>(0);

private:
	value_type  Value = Zero;

public:
	constexpr
	EnumBitset() noexcept = default;

	constexpr 
	/*implicit*/
	EnumBitset(value_type const& v) noexcept
	  : Value{v}
	{}
	
	template <Enumeration E2>
	constexpr 
	explicit
	EnumBitset(EnumBitset<E2> const& v) noexcept
	  : Value{static_cast<value_type>(v.Value)}
	{}

public:
	constexpr
	EnumBitset(type const&) noexcept = default;

	reference constexpr
	operator=(type const&) noexcept = default;

public:
	template <Enumeration E2>
	EnumBitset<E2> constexpr
	cast() const noexcept {
		return static_cast<E2>(this->Value);
	}
	
	template <Enumeration E2>
	bool constexpr
	test(E2 bits) const noexcept {
		return (this->Value & bits) != Zero;
	}

	underlying_t constexpr
	value() const noexcept {
		return static_cast<underlying_t>(this->Value);
	}

	constexpr 
	/*implicit*/ operator
	value_type() const noexcept {
		return this->Value;
	}
	
	type constexpr 
	operator|(type const& r) const noexcept {
		return static_cast<value_type>(
			static_cast<underlying_t>(this->Value) | static_cast<underlying_t>(r.Value)
		);
	}

	type constexpr 
	operator&(type const& r) const noexcept {
		return static_cast<value_type>(
			static_cast<underlying_t>(this->Value) & static_cast<underlying_t>(r.Value)
		);
	}
	
	type constexpr 
	operator^(type const& r) const noexcept {
		return static_cast<value_type>(
			static_cast<underlying_t>(this->Value) ^ static_cast<underlying_t>(r.Value)
		);
	}
	
	type constexpr 
	operator~() const noexcept {
		return static_cast<value_type>(
			~static_cast<underlying_t>(this->Value)
		);
	}

	bool constexpr
	operator==(type const&) const noexcept = default;
	
	auto constexpr
	operator<=>(type const&) const noexcept = delete;

public:
	reference constexpr 
	operator|=(type const& r) noexcept {
		this->Value = static_cast<value_type>(
			static_cast<underlying_t>(this->Value) | static_cast<underlying_t>(r.Value)
		);
		return *this;
	}

	type constexpr 
	operator&=(type const& r) noexcept {
		this->Value = static_cast<value_type>(
			static_cast<underlying_t>(this->Value) & static_cast<underlying_t>(r.Value)
		);
		return *this;
	}
	
	type constexpr 
	operator^=(type const& r) noexcept {
		this->Value = static_cast<value_type>(
			static_cast<underlying_t>(this->Value) ^ static_cast<underlying_t>(r.Value)
		);
		return *this;
	}
};

template <Enumeration E>
EnumBitset<E> constexpr
inline operator|(EnumBitset<E> const& lhs, E rhs) noexcept {
	return lhs | EnumBitset<E>{rhs};
}

template <Enumeration E>
EnumBitset<E> constexpr
inline operator|(E lhs, EnumBitset<E> const& rhs) noexcept {
	return EnumBitset<E>{lhs} | rhs;
}

template <Enumeration E>
EnumBitset<E> constexpr
inline operator&(EnumBitset<E> const& lhs, E rhs) noexcept {
	return lhs & EnumBitset<E>{rhs};
}

template <Enumeration E>
EnumBitset<E> constexpr
inline operator&(E lhs, EnumBitset<E> const& rhs) noexcept {
	return EnumBitset<E>{lhs} & rhs;
}

template <Enumeration E>
EnumBitset<E> constexpr
inline operator^(EnumBitset<E> const& lhs, E rhs) noexcept {
	return lhs ^ EnumBitset<E>{rhs};
}

template <Enumeration E>
EnumBitset<E> constexpr
inline operator^(E lhs, EnumBitset<E> const& rhs) noexcept {
	return EnumBitset<E>{lhs} ^ rhs;
}

template <Enumeration E>
bool constexpr
inline operator==(E lhs, EnumBitset<E> const& rhs) noexcept {
	return lhs == static_cast<E>(rhs);
}

template <Enumeration E>
bool constexpr
inline operator==(EnumBitset<E> const& lhs, E rhs) noexcept {
	return static_cast<E>(lhs) == rhs;
}

template <Enumeration E>
bool constexpr
inline operator!=(E lhs, EnumBitset<E> const& rhs) noexcept {
	return lhs != static_cast<E>(rhs);
}

template <Enumeration E>
bool constexpr
inline operator!=(EnumBitset<E> const& lhs, E rhs) noexcept {
	return static_cast<E>(lhs) != rhs;
}
