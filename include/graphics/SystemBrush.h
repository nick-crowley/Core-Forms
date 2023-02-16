#pragma once
#include "library/core.Forms.h"
#include "graphics/Brush.h"

namespace core::forms
{
	class FormsExport SystemBrush : public Brush
	{
		using base = Brush;

	public:
		SystemBrush const
		static ActiveCaption;
		
		SystemBrush const
		static InactiveCaption;

		SystemBrush const
		static CaptionText;

		SystemBrush const
		static Window;
		
		SystemBrush const
		static WindowText;

		SystemBrush const
		static Dialog;

	protected:
		implicit
		SystemBrush(SystemColour col) 
		  : base{{::GetSysColorBrush(win::DWord{col}),weakref}}
		{
		}
	};
}