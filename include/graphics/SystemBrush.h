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
		
		SystemBrush const
		static Highlight;
		
		SystemBrush const
		static HighlightText;

	protected:
		implicit
		SystemBrush(SystemColour col) 
		  : base{{::GetSysColorBrush(win::DWord{col}),weakref}}
		{
		}

	public:
		std::type_identity_t<SystemBrush const&>
		static get(SystemColour col) {
			switch (col) {
			default: ThrowInvalidArg(obj, "Unsupported SsytemColour");
			case SystemColour::ActiveCaption:   return SystemBrush::ActiveCaption;
			case SystemColour::InactiveCaption: return SystemBrush::InactiveCaption;
			case SystemColour::CaptionText:     return SystemBrush::CaptionText;
			case SystemColour::Window:          return SystemBrush::Window;
			case SystemColour::WindowText:      return SystemBrush::WindowText;
			case SystemColour::Dialog:          return SystemBrush::Dialog;
			case SystemColour::Highlight:       return SystemBrush::Highlight;
			case SystemColour::HighlightText:   return SystemBrush::HighlightText;
			}
		}
	};
}