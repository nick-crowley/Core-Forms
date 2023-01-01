#pragma once

#include "windows/WindowStyle.h"

enum class EditStyle : uint32_t {
	Left			= ES_LEFT             , // 0x0000L
	Center			= ES_CENTER           , // 0x0001L
	Right			= ES_RIGHT            , // 0x0002L
	MultiLine		= ES_MULTILINE        , // 0x0004L
	UpperCase		= ES_UPPERCASE        , // 0x0008L
	LowerCase		= ES_LOWERCASE        , // 0x0010L
	Password		= ES_PASSWORD         , // 0x0020L
	AutoVScroll		= ES_AUTOVSCROLL      , // 0x0040L
	AutoHScroll		= ES_AUTOHSCROLL      , // 0x0080L
	NoHideSel		= ES_NOHIDESEL        , // 0x0100L
	OemConvert		= ES_OEMCONVERT       , // 0x0400L
	ReadOnly		= ES_READONLY         , // 0x0800L
	WantReturn		= ES_WANTRETURN       , // 0x1000L
	Number			= ES_NUMBER           , // 0x2000L
	
	None			= Left,
};

EditStyle constexpr inline operator|(EditStyle a,EditStyle b) { return detail::enum_bit_or(a,b);  }
EditStyle constexpr inline operator&(EditStyle a,EditStyle b) { return detail::enum_bit_and(a,b); }
EditStyle constexpr inline operator^(EditStyle a,EditStyle b) { return detail::enum_bit_xor(a,b); }

WindowStyle constexpr inline operator|(WindowStyle a,EditStyle b) { return detail::enum_bit_or(a,b);  }
WindowStyle constexpr inline operator&(WindowStyle a,EditStyle b) { return detail::enum_bit_and(a,b); }
WindowStyle constexpr inline operator^(WindowStyle a,EditStyle b) { return detail::enum_bit_xor(a,b); }

EditStyle constexpr inline operator|(EditStyle a,WindowStyle b) { return detail::enum_bit_or(a,b);  }
EditStyle constexpr inline operator&(EditStyle a,WindowStyle b) { return detail::enum_bit_and(a,b); }
EditStyle constexpr inline operator^(EditStyle a,WindowStyle b) { return detail::enum_bit_xor(a,b); }

