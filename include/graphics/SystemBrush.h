/* o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o */ /*!
* @copyright	Copyright (c) 2023, Nick Crowley. All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted
* provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions
*    and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of
*    conditions and the following disclaimer in the documentation and/or other materials provided
*    with the distribution.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
* FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
* WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
* The views and conclusions contained in the software and documentation are those of the author 
* and should not be interpreted as representing official policies, either expressed or implied, of
* the projects which contain it.
*/
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Preprocessor Directives o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#pragma once
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Header Files o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#include "library/core.Forms.h"
#include "graphics/Brush.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
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
		nstd::return_t<SystemBrush const&>
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
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o