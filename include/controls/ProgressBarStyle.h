#pragma once

#include "windows/WindowStyle.h"

enum class ProgressBarStyle {
	Smooth           = PBS_SMOOTH              , // 0x01
	Vertical         = PBS_VERTICAL            , // 0x04
	Marquee          = PBS_MARQUEE             , // 0x08
	SmoothReverse    = PBS_SMOOTHREVERSE       , // 0x10
	
	None			  = 0
};

ProgressBarStyle constexpr inline operator|(ProgressBarStyle a,ProgressBarStyle b) { return detail::enum_bit_or(a,b);  }
ProgressBarStyle constexpr inline operator&(ProgressBarStyle a,ProgressBarStyle b) { return detail::enum_bit_and(a,b); }
ProgressBarStyle constexpr inline operator^(ProgressBarStyle a,ProgressBarStyle b) { return detail::enum_bit_xor(a,b); }

WindowStyle constexpr inline operator|(WindowStyle a,ProgressBarStyle b) { return detail::enum_bit_or(a,b);  }
WindowStyle constexpr inline operator&(WindowStyle a,ProgressBarStyle b) { return detail::enum_bit_and(a,b); }
WindowStyle constexpr inline operator^(WindowStyle a,ProgressBarStyle b) { return detail::enum_bit_xor(a,b); }

ProgressBarStyle constexpr inline operator|(ProgressBarStyle a,WindowStyle b) { return detail::enum_bit_or(a,b);  }
ProgressBarStyle constexpr inline operator&(ProgressBarStyle a,WindowStyle b) { return detail::enum_bit_and(a,b); }
ProgressBarStyle constexpr inline operator^(ProgressBarStyle a,WindowStyle b) { return detail::enum_bit_xor(a,b); }
