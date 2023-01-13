#pragma once
#include "formsFramework.h"
#include "system/SharedHandle.h"
#include "graphics/Colours.h"
#include "graphics/ObjectFlags.h"

namespace core::forms
{
	class Pen
	{
		SharedHandle  Handle;

	public:
		Pen(Colour col, unsigned width, PenStyle s = PenStyle::Solid) {
			if (auto const pen = ::CreatePen(static_cast<int>(s), width, win::DWord{col}); !pen)
				win::throw_exception(::GetLastError());
			else
				this->Handle = make_handle(pen);
		}

		::HPEN
		handle() const
		{
			return get_handle<::HPEN>(this->Handle);
		}
	};
}