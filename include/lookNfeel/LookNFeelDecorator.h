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
#include "lookNfeel/ILookNFeelProvider.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class LookNFeelDecorator : public ILookNFeelProvider
	{
		SharedLookNFeelProvider ColourImpl;
		SharedLookNFeelProvider LogicImpl;

	public:
		LookNFeelDecorator(SharedLookNFeelProvider colours, SharedLookNFeelProvider logic) 
		  : ColourImpl{colours},
		    LogicImpl{logic}
		{}

	public:
		bool 
		virtual customCaption() const override {
			return this->LogicImpl->customCaption();
		}

		void
		virtual draw(ButtonControl& ctrl, OwnerDrawEventArgs const& args) override {
			this->LogicImpl->draw(ctrl, args);
		}

		void
		virtual draw(CheckBoxControl& ctrl, OwnerDrawEventArgs const& args) override {
			this->LogicImpl->draw(ctrl, args);
		}

		void
		virtual draw(ComboBoxControl& ctrl, OwnerDrawEventArgs const& args) override {
			this->LogicImpl->draw(ctrl, args);
		}

		void
		virtual measure(ComboBoxControl& ctrl, MeasureItemEventArgs const& args) override {
			this->LogicImpl->measure(ctrl, args);
		}
		
		void
		virtual draw(GroupBoxControl& ctrl, OwnerDrawEventArgs const& args) override {
			this->LogicImpl->draw(ctrl, args);
		}
	
		void
		virtual erase(GroupBoxControl& ctrl, EraseBackgroundEventArgs const& args) override {
			this->LogicImpl->erase(ctrl, args);
		}

		void
		virtual draw(LabelControl& ctrl, OwnerDrawEventArgs const& args) override {
			this->LogicImpl->draw(ctrl, args);
		}

		void
		virtual draw(ListBoxControl& ctrl, OwnerDrawEventArgs const& args) override {
			this->LogicImpl->draw(ctrl, args);
		}

		void
		virtual erase(ListBoxControl& ctrl, EraseBackgroundEventArgs const& args) override {
			this->LogicImpl->erase(ctrl, args);
		}

		void
		virtual measure(ListBoxControl& ctrl, MeasureItemEventArgs const& args) override {
			this->LogicImpl->measure(ctrl, args);
		}

		void
		virtual draw(ListViewControl& ctrl, OwnerDrawEventArgs const& args) override {
			this->LogicImpl->draw(ctrl, args);
		}

		void
		virtual erase(ListViewControl& ctrl, EraseBackgroundEventArgs const& args) override {
			this->LogicImpl->erase(ctrl, args);
		}

		void
		virtual measure(ListViewControl& ctrl, MeasureItemEventArgs const& args) override {
			this->LogicImpl->measure(ctrl, args);
		}

		void
		virtual draw(PictureControl& ctrl, OwnerDrawEventArgs const& args) override {
			this->LogicImpl->draw(ctrl, args);
		}

		void
		virtual draw(RadioButtonControl& ctrl, OwnerDrawEventArgs const& args) override {
			this->LogicImpl->draw(ctrl, args);
		}

		void
		virtual draw(StaticControl& ctrl, OwnerDrawEventArgs const& args) override {
			this->LogicImpl->draw(ctrl, args);
		}
		
		void
		virtual draw(Dialog& dlg, PaintWindowEventArgs const& args) override {
			this->LogicImpl->draw(dlg, args);
		}
	
		Response 
		virtual draw(Dialog& dlg, NonClientPaintEventArgs& args) override {
			return this->LogicImpl->draw(dlg, args);
		}

		void 
		virtual onCreated(Window& wnd, CreateWindowEventArgs const& args) override {
			this->LogicImpl->onCreated(wnd, args);
		}

		AnyColour 
		virtual button() override {
			return this->ColourImpl->button();
		}

		AnyColourPair 
		virtual caption() override {
			return this->ColourImpl->caption();
		}

		AnyColour 
		virtual control() override {
			return this->ColourImpl->control();
		}
		
		FontDescription
		virtual default() override {
			return this->ColourImpl->default();
		}

		Font 
		virtual heading1() override {
			return this->ColourImpl->heading1();
		}

		Font 
		virtual heading2() override {
			return this->ColourImpl->heading2();
		}

		AnyColour 
		virtual highlight() override {
			return this->ColourImpl->highlight();
		}

		Font 
		virtual paragraph() override {
			return this->ColourImpl->paragraph();
		}

		AnyColour 
		virtual primary() override {
			return this->ColourImpl->primary();
		}

		AnyColour 
		virtual secondary() override {
			return this->ColourImpl->secondary();
		}

		AnyColour 
		virtual tertiary() override {
			return this->ColourImpl->tertiary();
		}

		AnyColour 
		virtual window() override {
			return this->ColourImpl->window();
		}
	};
	
}	// namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o