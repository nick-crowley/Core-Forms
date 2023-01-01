#pragma once
#include "formsFramework.h"

template <typename T>
concept RealNumber = std::is_arithmetic_v<T>
                 && !std::_Is_any_of_v<T,bool,char,wchar_t,char8_t,char16_t,char32_t>;

class Percentage
{
public:
	Percentage const
	static Min, Max;

private:
	float   m_value;

public:
	template <RealNumber T>
	constexpr
	explicit  
	Percentage(T v) : m_value{static_cast<float>(v)}
	{
		if (*this < Min)
			this->m_value = Min;
		if (*this > Max)
			this->m_value = Max;
	}

	template <RealNumber Result> 
	/*implicit*/
	operator Result() const {
		return static_cast<Result>(this->m_value);
	}

	template <RealNumber Result> 
	Result
	operator*(Result const rhs) const
	{
		return static_cast<Result>(this->m_value * rhs / 100.0f);
	}

	auto operator<=>(Percentage const&) const = default;
};

Percentage const
inline Percentage::Min{0.0f};

Percentage const
inline Percentage::Max{100.0f};

namespace literals
{
	Percentage constexpr
	operator""_px(unsigned long long n) {
		return Percentage{n};
	}

	Percentage constexpr
	operator""_px(long double n) {
		return Percentage{n};
	}
}
