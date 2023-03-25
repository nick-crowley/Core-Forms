#pragma once
#include "windows/WindowStyle.h"

namespace core::forms
{
	enum class ProgressBarStyle {
		Smooth           = PBS_SMOOTH              , // 0x01
		Vertical         = PBS_VERTICAL            , // 0x04
		Marquee          = PBS_MARQUEE             , // 0x08
		SmoothReverse    = PBS_SMOOTHREVERSE       , // 0x10
	
		None			  = 0
	};
}	// namespace core::forms

namespace core::meta 
{
	metadata bool Settings<bitwise_enum, core::forms::ProgressBarStyle> = true;
	metadata bool Settings<compatible_enum, core::forms::ProgressBarStyle, core::forms::WindowStyle> = true;
	metadata bool Settings<compatible_enum, core::forms::WindowStyle, core::forms::ProgressBarStyle> = true;
}
