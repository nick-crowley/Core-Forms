#pragma once
#include "library/core.Forms.h"
#include "system/SharedHandle.h"
#include "graphics/Colours.h"
#include "graphics/ObjectFlags.h"

namespace core::forms
{
	class Pen
	{
		SharedPen  Handle;

	public:
		Pen(Colour col, unsigned width, PenStyle s = PenStyle::Solid) 
		  : Handle{::CreatePen(static_cast<int>(s), width, win::DWord{col})}
		{
			if (!this->Handle)
				win::LastError{}.throwAlways();
		}

		::HPEN
		handle() const
		{
			return *this->Handle;
		}
	};
}