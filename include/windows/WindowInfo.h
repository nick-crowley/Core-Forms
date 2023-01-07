#pragma once
#include "windows/ExWindowStyle.h"
#include "windows/WindowStyle.h"
#include "graphics/SizePoint.h"
#include "graphics/Rectangle.h"

enum class WindowCaptionState {Inactive = FALSE, Active = TRUE, Unknown = -1};
	
struct WindowInfo
{
	Size const               Border;
	Rect const               Client;
	Rect const               Window;
	WindowStyle const        Style;
	ExWindowStyle const      ExStyle;
	WindowCaptionState const State;

	explicit
	WindowInfo(::WINDOWINFO const& wi)
	  : Border{wi.cxWindowBorders, wi.cyWindowBorders},
		Client{wi.rcClient},
		Window{wi.rcWindow},
		Style{static_cast<WindowStyle>(wi.dwStyle)},
		ExStyle{static_cast<ExWindowStyle>(wi.dwExStyle)},
		State{static_cast<WindowCaptionState>(wi.dwWindowStatus)}
	{}
};
