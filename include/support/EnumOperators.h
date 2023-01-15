#pragma once
#include "library/core.Forms.h"

namespace core::forms::detail {
	auto constexpr
	inline enum_bit_and(auto&& lhs, auto&& rhs) { 
		using lhs_t = std::remove_const_t<std::remove_reference_t<decltype(lhs)>>;
		using rhs_t = std::remove_const_t<std::remove_reference_t<decltype(rhs)>>;
		return lhs_t( std::underlying_type_t<lhs_t>(lhs) & std::underlying_type_t<rhs_t>(rhs) );
	};

	auto constexpr
	inline enum_bit_or(auto&& lhs, auto&& rhs) { 
		using lhs_t = std::remove_const_t<std::remove_reference_t<decltype(lhs)>>;
		using rhs_t = std::remove_const_t<std::remove_reference_t<decltype(rhs)>>;
		return lhs_t( std::underlying_type_t<lhs_t>(lhs) | std::underlying_type_t<rhs_t>(rhs) );
	};
	
	auto constexpr
	inline enum_bit_xor(auto&& lhs, auto&& rhs) { 
		using lhs_t = std::remove_const_t<std::remove_reference_t<decltype(lhs)>>;
		using rhs_t = std::remove_const_t<std::remove_reference_t<decltype(rhs)>>;
		return lhs_t( std::underlying_type_t<lhs_t>(lhs) ^ std::underlying_type_t<rhs_t>(rhs) );
	};
	
	auto constexpr
	inline enum_bit_not(auto&& lhs) { 
		using lhs_t = std::remove_const_t<std::remove_reference_t<decltype(lhs)>>;
		return lhs_t( ~std::underlying_type_t<lhs_t>(lhs) );
	};
}
