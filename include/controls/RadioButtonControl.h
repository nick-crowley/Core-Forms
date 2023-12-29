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
#include "controls/ButtonControl.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class RadioButtonControl : public ButtonControl 
	{
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
		using base = ButtonControl;

	public:
		using ControlIdentRange = std::pair<uint16_t,uint16_t>;
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		nstd::bitset<ButtonState> StateWhenOwnerDraw = ButtonState::None;
		ControlIdentRange GroupBoundaries;
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		implicit
		RadioButtonControl(uint16_t id, ControlIdentRange group)
		  : base{id}, 
		    GroupBoundaries{group}
		{
			this->Clicked += {*this, &RadioButtonControl::this_Clicked};
			this->MouseDown += {*this, &RadioButtonControl::this_MouseEvent};
			this->MouseMove += {*this, &RadioButtonControl::this_MouseEvent};
			this->MouseUp += {*this, &RadioButtonControl::this_MouseEvent};
			this->backColour(this->LookNFeel->window());
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		template <typename Self>
		nstd::mirror_cv_ref_t<Self, RadioButtonControl>*
		sibling(this Self&&, Window const& parent, uint16_t id) {
			return Window::ExistingWindows.find<RadioButtonControl>(::GetDlgItem(parent.handle(),id));
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		bool
		checked() const noexcept {
			return this->state().test(ButtonState::Checked);
		}
		
		nstd::bitset<Alignment>
		defaultAlign() const noexcept override {
			return Alignment::Left|Alignment::VCentre;
		}

		WindowRole
		virtual role() const noexcept override {
			return WindowRole::RadioButton;
		}
		
		nstd::bitset<ButtonState>
		state() const noexcept {
			// Owner-draw buttons don't maintain radio-button state
			if (this->ownerDraw())
				return this->StateWhenOwnerDraw;
			return base::state();
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		void
		check() noexcept 
		{
			if (!this->ownerDraw()) 
				Button_SetCheck(this->handle(), ButtonState::Checked);
			else {
				// Invalidate this control and siblings within group
				Window const* dialog = this->parent();
					if (id == this->ident()) {
						this->StateWhenOwnerDraw.set(ButtonState::Checked, true);
						this->invalidate(true);
					}
					else if (RadioButtonControl* ctrl = this->sibling(*dialog,id); ctrl) {
						ctrl->StateWhenOwnerDraw.set(ButtonState::Checked, false);
						ctrl->invalidate(true);
					}
				}
			}
		}
		
		std::optional<int16_t>
		selection() const 
		{
			// Search for the checked sibling
			Window const* dialog = this->parent();
			for (auto id = this->GroupBoundaries.first; id <= this->GroupBoundaries.second; ++id) {
				RadioButtonControl const* ctrl = (id == this->ident()) ? this : this->sibling(*dialog,id);
				if (ctrl->checked())
					return id;
			}
			return nullopt;
		}

	protected:
		Response 
		virtual onOwnerDraw(OwnerDrawEventArgs args) override {
			if (args.Ident == this->ident()) {
				if (this->checked())
					args.Item.State |= OwnerDrawState::Checked;
				this->LookNFeel->draw(*this, args);
				return TRUE;
			}

			return Unhandled;
		}
		
	protected:
		/*Response
		virtual onOfferNotification(::UINT notification) override {
			switch (notification) {
			case BN_CLICKED:
				this->Clicked.raise();
				return 0;
			}
			return Unhandled;
		}*/
		
	private:
		void
		this_Clicked(Window& sender) {
			if (this->ownerDraw() && !this->checked()) 
				this->check();
		}
	
		void
		this_MouseEvent(forms::Window& sender, forms::MouseEventArgs args) {
			if (!this->ownerDraw())
				return;

			if (args.Event == forms::MouseMessage::ButtonDown) {
				this->StateWhenOwnerDraw.set(ButtonState::Pushed, true);
				this->invalidate();
			}
			else if (args.Event == forms::MouseMessage::ButtonUp) {
				this->StateWhenOwnerDraw.set(ButtonState::Pushed, false);
				this->invalidate();
			}
		}
	};
}	// namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o