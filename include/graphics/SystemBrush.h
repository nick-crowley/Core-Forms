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
		static InactiveCaptionText;

		SystemBrush const
		static GradientActiveCaption;

		SystemBrush const
		static GradientInactiveCaption;

		
		SystemBrush const
		static Menu;
		
		SystemBrush const
		static MenuText;
		
		SystemBrush const
		static MenuHighlight;
		
		SystemBrush const
		static MenuBar;
		

		SystemBrush const
		static AppWorkspace;

		SystemBrush const
		static Desktop;

		SystemBrush const
		static Dialog;
		
		SystemBrush const
		static Window;
		
		SystemBrush const
		static WindowFrame;

		SystemBrush const
		static WindowText;
		
		
		SystemBrush const
		static ActiveBorder;
		
		SystemBrush const
		static InactiveBorder;


		SystemBrush const
		static Highlight;
		
		SystemBrush const
		static HighlightText;
		
		SystemBrush const
		static GrayText;

		
		SystemBrush const
		static ButtonFace;

		SystemBrush const
		static ButtonHighlight;

		SystemBrush const
		static ButtonShadow;
		
		SystemBrush const
		static ButtonText;
		
		SystemBrush const
		static ButtonDkShadow;
		
		SystemBrush const
		static ButtonLight;
		

		SystemBrush const
		static InfoText;
		
		SystemBrush const
		static Info;
		

		SystemBrush const
		static Hotlight;
		
		SystemBrush const
		static ScrollBar;
		
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
			default: ThrowInvalidArg(col, "Unsupported SystemColour");
			case SystemColour::ActiveCaption:           return SystemBrush::ActiveCaption;
			case SystemColour::InactiveCaption:         return SystemBrush::InactiveCaption;
			case SystemColour::CaptionText:             return SystemBrush::CaptionText;
			case SystemColour::InactiveCaptionText:     return SystemBrush::InactiveCaptionText;
			case SystemColour::GradientActiveCaption:   return SystemBrush::GradientActiveCaption;
			case SystemColour::GradientInactiveCaption: return SystemBrush::GradientInactiveCaption;
			
			case SystemColour::Menu:                    return SystemBrush::Menu;
			case SystemColour::MenuText:                return SystemBrush::MenuText;
			case SystemColour::MenuHighlight:           return SystemBrush::MenuHighlight;
			case SystemColour::MenuBar:                 return SystemBrush::MenuBar;

			case SystemColour::AppWorkspace:            return SystemBrush::AppWorkspace;
			case SystemColour::Desktop:                 return SystemBrush::Desktop;
			case SystemColour::Dialog:                  return SystemBrush::Dialog;
			case SystemColour::Window:                  return SystemBrush::Window;
			case SystemColour::WindowFrame:             return SystemBrush::WindowFrame;
			case SystemColour::WindowText:              return SystemBrush::WindowText;
			
			case SystemColour::ActiveBorder:            return SystemBrush::ActiveBorder;
			case SystemColour::InactiveBorder:          return SystemBrush::InactiveBorder;

			case SystemColour::Highlight:               return SystemBrush::Highlight;
			case SystemColour::HighlightText:           return SystemBrush::HighlightText;
			case SystemColour::GrayText:                return SystemBrush::GrayText;
		
			case SystemColour::ButtonHighlight:         return SystemBrush::ButtonHighlight;
			case SystemColour::ButtonShadow:            return SystemBrush::ButtonShadow;
			case SystemColour::ButtonText:              return SystemBrush::ButtonText;
			case SystemColour::ButtonDkShadow:          return SystemBrush::ButtonDkShadow;
			case SystemColour::ButtonLight:             return SystemBrush::ButtonLight;
			
			case SystemColour::InfoText:                return SystemBrush::InfoText;
			case SystemColour::Info:                    return SystemBrush::Info;

			case SystemColour::Hotlight:                return SystemBrush::Hotlight;
			case SystemColour::ScrollBar:               return SystemBrush::ScrollBar;
			}
		}
	};
}