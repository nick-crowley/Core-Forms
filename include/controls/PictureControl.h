#pragma once
#include "formsFramework.h"
#include "controls/StaticControl.h"
#include "graphics/Bitmap.h"
#include "graphics/Icon.h"

namespace core::forms
{
	class PictureControl : public StaticControl 
	{	
		using Picture = std::variant<Icon,Bitmap>;

	private:
		std::optional<Picture> Image;

	public:
		PictureControl() 
		{}
	
	public:
		std::optional<Icon>
		icon() const {
			if (auto* icon = this->Image ? std::get_if<Icon>(&*this->Image) : nullptr; !icon)
				return {};
			else
				return *icon;
		}
	
		std::optional<Bitmap>
		image() const {
			if (auto* bitmap = this->Image ? std::get_if<Bitmap>(&*this->Image) : nullptr; !bitmap)
				return {};
			else
				return *bitmap;
		}
	
	public:
		void
		icon(Icon icon) {
			this->Image = icon;
		}
	
		void
		image(Bitmap bmp) {
			this->Image = bmp;
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