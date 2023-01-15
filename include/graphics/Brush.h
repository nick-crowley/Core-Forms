#pragma once
#include "library/core.Forms.h"
#include "graphics/Colours.h"
#include "system/SharedHandle.h"

namespace core::forms
{
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
}