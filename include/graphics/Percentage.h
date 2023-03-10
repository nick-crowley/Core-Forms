#pragma once
#include "library/core.Forms.h"

namespace core::forms
{
	class Percentage
	{
	public:
		Percentage const
		static Min, Max;

	private:
		float   m_value;

	public:
		template <meta::RealNumber T>
		constexpr
		explicit  
		Percentage(T v) : m_value{static_cast<float>(v)}
		{
			if (*this < Min)
				this->m_value = Min;
			if (*this > Max)
				this->m_value = Max;
		}

		satisfies(Percentage,
			NotDefaultConstructible,
			IsCopyable,
			IsSortable
		);

	public:
		template <meta::RealNumber Result> 
		implicit operator Result() const {
			return static_cast<Result>(this->m_value);
		}

		template <meta::RealNumber Result> 
		Result
		operator*(Result const rhs) const
		{
			return static_cast<Result>(this->m_value * rhs / 100.0f);
		}

		bool
		operator==(Percentage const& r) const {
			return boost::math::relative_difference(this->m_value,r.m_value) < 0.01f;
		}

		bool
		operator!=(Percentage const& r) const {
			return boost::math::relative_difference(this->m_value,r.m_value) >= 0.01f;
		}
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
}