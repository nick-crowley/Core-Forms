#pragma once
#include "library/core.Forms.h"
#include "graphics/Colours.h"
#include "system/SharedHandle.h"

namespace core::forms
{
	class FormsExport Brush
	{
		SharedBrush  Handle;

	public:
		explicit
		Brush(SharedBrush h) : Handle{std::move(ThrowIfEmpty(h))}
		{
		}

		Brush(Colour col) : Handle{::CreateSolidBrush(win::DWord{col})}
		{
			if (!this->Handle)
				win::LastError{}.throwAlways();
		}

		SharedBrush
		handle() const
		{
			return this->Handle;
		}
	};
}