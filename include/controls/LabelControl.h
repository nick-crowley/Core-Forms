#pragma once
#include "library/core.Forms.h"
#include "controls/StaticControl.h"
#include "graphics/Colours.h"

namespace core::forms
{
	class LabelControl : public StaticControl 
	{
		Colour  TextColour = DeviceContext::get(SystemColour::WindowText);

	public:
		LabelControl() 
		{}
	
	public:
		Colour
		colour() const {
			return this->TextColour;
		}
	
		void
		colour(Colour col) {
			this->TextColour = col;
		}
	
		Response 
		onOwnerDraw(OwnerDrawEventArgs args) override {
			if (args.Ident == this->ident()) {
				this->LookNFeel->draw(*this, args);
				return TRUE;
			}

			return Unhandled;
		}
	};
}	// namespace core::forms