#pragma once
#include "formsFramework.h"
#include "controls/ButtonControl.h"

class GroupBoxControl : public ButtonControl
{
	Colour  TextColour = DeviceContext::get(SystemColour::WindowText);

public:
	GroupBoxControl() 
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
