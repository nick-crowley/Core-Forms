#pragma once
#include "formsFramework.h"
#include "graphics/Colours.h"
#include "system/SharedHandle.h"

class Brush
{
	SharedHandle  Handle;

public:
	Brush(Colour col) {
		if (auto const brush = ::CreateSolidBrush(win::DWord{col}); !brush)
			win::throw_exception(::GetLastError());
		else
			this->Handle = make_handle(brush);
	}

	::HBRUSH
	handle() const
	{
		return get_handle<::HBRUSH>(this->Handle);
	}
};
