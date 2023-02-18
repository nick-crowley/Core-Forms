#pragma once
#include "library/core.Forms.h"
#include "controls/ButtonControl.h"

namespace core::forms
{
	class GroupBoxControl : public ButtonControl
	{
		Colour  TextColour = to_colour(SystemColour::WindowText);

	public:
		implicit
		GroupBoxControl(uint16_t id) : ButtonControl{id}
		{}
	
	public:
		Colour
		colour() const {
			return this->TextColour;
		}
	
		WindowRole
		role() const override {
			return WindowRole::Grouping;
		}

	public:
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