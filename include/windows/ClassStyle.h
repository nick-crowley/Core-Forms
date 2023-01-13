#pragma once
#include "formsFramework.h"
#include "support/EnumOperators.h"

namespace core::forms
{
	enum class ClassStyle : uint32_t {
		Vredraw				= CS_VREDRAW				, // 0x0001
		Hredraw				= CS_HREDRAW				, // 0x0002
		Dblclks				= CS_DBLCLKS				, // 0x0008
		Owndc				= CS_OWNDC					, // 0x0020
		Classdc				= CS_CLASSDC				, // 0x0040
		Parentdc			= CS_PARENTDC				, // 0x0080
		Noclose				= CS_NOCLOSE				, // 0x0200
		Savebits			= CS_SAVEBITS				, // 0x0800
		Bytealignclient		= CS_BYTEALIGNCLIENT		, // 0x1000
		Bytealignwindow		= CS_BYTEALIGNWINDOW		, // 0x2000
		Globalclass			= CS_GLOBALCLASS			, // 0x4000
		Ime					= CS_IME					, // 0x00010000
		Dropshadow			= CS_DROPSHADOW				, // 0x00020000
	
		None				= 0,
	};

	ClassStyle constexpr inline operator|(ClassStyle a,ClassStyle b) { return detail::enum_bit_or(a,b);  }
	ClassStyle constexpr inline operator&(ClassStyle a,ClassStyle b) { return detail::enum_bit_and(a,b); }
	ClassStyle constexpr inline operator^(ClassStyle a,ClassStyle b) { return detail::enum_bit_xor(a,b); }
}	// namespace core::forms