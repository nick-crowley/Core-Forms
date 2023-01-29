#pragma once
#include "library/core.Forms.h"
#include "controls/StaticControl.h"
#include "graphics/Colours.h"

namespace core::forms
{
	class LabelControl : public StaticControl 
	{
	public:
		enum HorizontalAlign { Left, Centre, Right };

	private:
		HorizontalAlign TextAlignment = Left;
		Colour          TextColour = DeviceContext::get(SystemColour::WindowText);

	public:
		LabelControl(uint16_t id) : StaticControl{id}
		{}
	
	public:
		HorizontalAlign
		align() const {
			return this->TextAlignment;
		}

		Colour
		colour() const {
			return this->TextColour;
		}
	
	public:
		void
		align(HorizontalAlign horz) {
			this->TextAlignment = horz;
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

constdata bool core::meta::Settings<core::bitwise_enum, core::forms::LabelControl::HorizontalAlign> = true;
constdata bool core::meta::Settings<core::compatible_enum, core::forms::StaticStyle, core::forms::LabelControl::HorizontalAlign> = true;
constdata bool core::meta::Settings<core::compatible_enum, core::forms::LabelControl::HorizontalAlign, core::forms::StaticStyle> = true;
