#pragma once
#include "library/core.Forms.h"
#include "controls/ButtonControl.h"

namespace core::forms
{
	class RadioButtonControl : public ButtonControl {
		bool Checked = false;

	public:
		RadioButtonControl(uint16_t id) : ButtonControl{id}
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
				this->invalidate(true);
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
				this->invalidate(true);
			}
		}
		
	protected:
		/*Response
		offerNotification(::UINT notification) override {
			auto const on_exit = this->Debug.setTemporaryState(
				{ProcessingState::NotificationProcessing, this->notificationName(notification)}
			); 

			switch (notification) {
			case BN_CLICKED:
				this->Clicked.raise();
				return 0;
			}

			return Unhandled;
		}*/
	};
}	// namespace core::forms