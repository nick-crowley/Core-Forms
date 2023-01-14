#pragma once
#include "formsFramework.h"

namespace core::forms
{
	template <meta::Enumeration E>
	class EnumBitset
	{
		template <meta::Enumeration>
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
		implicit
		EnumBitset(value_type const& v) noexcept
		  : Value{v}
		{}
	
		template <meta::Enumeration E2>
		constexpr 
		explicit
		EnumBitset(EnumBitset<E2> const& v) noexcept
		  : Value{static_cast<value_type>(v.Value)}
		{}

		satisfies(EnumBitset,
			constexpr IsRegular noexcept,
			NotSortable
		);

	public:
		template <meta::Enumeration E2>
		EnumBitset<E2> constexpr
		cast() const noexcept {
			return static_cast<E2>(this->Value);
		}
	
		template <meta::Enumeration E2>
		bool constexpr
		test(E2 bits) const noexcept {
			return (this->Value & bits) != Zero;
		}

		underlying_t constexpr
		value() const noexcept {
			return static_cast<underlying_t>(this->Value);
		}

		constexpr 
		implicit operator
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

	template <meta::Enumeration E>
	EnumBitset<E> constexpr
	inline operator|(EnumBitset<E> const& lhs, E rhs) noexcept {
		return lhs | EnumBitset<E>{rhs};
	}

	template <meta::Enumeration E>
	EnumBitset<E> constexpr
	inline operator|(E lhs, EnumBitset<E> const& rhs) noexcept {
		return EnumBitset<E>{lhs} | rhs;
	}

	template <meta::Enumeration E>
	EnumBitset<E> constexpr
	inline operator&(EnumBitset<E> const& lhs, E rhs) noexcept {
		return lhs & EnumBitset<E>{rhs};
	}

	template <meta::Enumeration E>
	EnumBitset<E> constexpr
	inline operator&(E lhs, EnumBitset<E> const& rhs) noexcept {
		return EnumBitset<E>{lhs} & rhs;
	}

	template <meta::Enumeration E>
	EnumBitset<E> constexpr
	inline operator^(EnumBitset<E> const& lhs, E rhs) noexcept {
		return lhs ^ EnumBitset<E>{rhs};
	}

	template <meta::Enumeration E>
	EnumBitset<E> constexpr
	inline operator^(E lhs, EnumBitset<E> const& rhs) noexcept {
		return EnumBitset<E>{lhs} ^ rhs;
	}

	template <meta::Enumeration E>
	bool constexpr
	inline operator==(E lhs, EnumBitset<E> const& rhs) noexcept {
		return lhs == static_cast<E>(rhs);
	}

	template <meta::Enumeration E>
	bool constexpr
	inline operator==(EnumBitset<E> const& lhs, E rhs) noexcept {
		return static_cast<E>(lhs) == rhs;
	}

	template <meta::Enumeration E>
	bool constexpr
	inline operator!=(E lhs, EnumBitset<E> const& rhs) noexcept {
		return lhs != static_cast<E>(rhs);
	}

	template <meta::Enumeration E>
	bool constexpr
	inline operator!=(EnumBitset<E> const& lhs, E rhs) noexcept {
		return static_cast<E>(lhs) != rhs;
	}
}