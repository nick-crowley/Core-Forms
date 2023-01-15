#pragma once
#include "library/core.Forms.h"
#include "controls/ButtonControl.h"

namespace core::forms
{
	class RadioButtonControl : public ButtonControl {
	public:
		RadioButtonControl() 
		{}

	public:
		bool
		checked() const {
			return this->state().test(ButtonState::Checked);
		}
	
		Response 
		onOwnerDraw(OwnerDrawEventArgs args) override {
			if (args.Ident == this->ident()) {
				this->LookNFeel->draw(*this, args);
				return TRUE;
			}

			return Unhandled;
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
	};
}	// namespace core::forms