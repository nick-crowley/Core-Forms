#pragma once
#include "library/core.Forms.h"
#include "controls/ButtonControl.h"

namespace core::forms
{
	class RadioButtonControl : public ButtonControl {
		bool Checked = false;

	public:
		RadioButtonControl() 
		{}

	public:
		bool
		checked() const {
			// Owner-draw buttons don't maintain radio-button state
			if (this->ownerDraw()) 
				return this->Checked;

			return this->state().test(ButtonState::Checked);
		}
		
	public:
		void
		check() {
			Button_SetCheck(this->handle(), ButtonState::Checked);

			if (this->ownerDraw()) {
				this->Checked = true;
				::InvalidateRect(this->handle(), nullptr, TRUE);
			}
		}
		
		Response 
		onOwnerDraw(OwnerDrawEventArgs args) override {
			if (args.Ident == this->ident()) {
				if (this->Checked)
					args.Item.State |= OwnerDrawState::Checked;
				this->LookNFeel->draw(*this, args);
				return TRUE;
			}

			return Unhandled;
		}
		
		void
		uncheck() {
			Button_SetCheck(this->handle(), ButtonState::Unchecked);
			
			if (this->ownerDraw()) {
				this->Checked = false;
				::InvalidateRect(this->handle(), nullptr, TRUE);
			}
		}
		
	protected:
		/*Response
		offer_notification(::UINT notification) override {
			auto const on_exit = this->Debug.setTemporaryState(
				{ProcessingState::NotificationProcessing, this->notification_name(notification)}
			); 

			switch (notification) {
			case BN_CLICKED:
				this->Clicked.raise();
				return 0;
			}

			return Unhandled;
		}*/

		bool
		ownerDraw() const {
			return (this->style<ButtonStyle>() & ButtonStyle::TypeMask) == ButtonStyle::OwnerDraw;
		}
	};
}	// namespace core::forms