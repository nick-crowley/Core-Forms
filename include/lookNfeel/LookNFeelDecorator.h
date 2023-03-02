#pragma once
#include "lookNfeel/ILookNFeelProvider.h"

namespace core::forms
{
	class LookNFeelDecorator : public ILookNFeelProvider
	{
		SharedLookNFeelProvider m_lookNfeel;

	public:
		LookNFeelDecorator(SharedLookNFeelProvider wrapped) : m_lookNfeel{wrapped}
		{}

	public:
		void
		draw(ButtonControl& ctrl, OwnerDrawEventArgs const& args) override {
			return this->m_lookNfeel->draw(ctrl, args);
		}
	
		void
		draw(CheckBoxControl& ctrl, OwnerDrawEventArgs const& args) override {
			return this->m_lookNfeel->draw(ctrl, args);
		}
	
		void
		draw(LabelControl& ctrl, OwnerDrawEventArgs const& args) override {
			return this->m_lookNfeel->draw(ctrl, args);
		}
	
		void
		draw(ListBoxControl& ctrl, OwnerDrawEventArgs const& args) override {
			return this->m_lookNfeel->draw(ctrl, args);
		}
	
		void
		erase(ListBoxControl& ctrl, EraseBackgroundEventArgs const& args) override {
			return this->m_lookNfeel->erase(ctrl, args);
		}
	
		void
		draw(GroupBoxControl& ctrl, OwnerDrawEventArgs const& args) override {
			return this->m_lookNfeel->draw(ctrl, args);
		}
	
		void
		draw(PictureControl& ctrl, OwnerDrawEventArgs const& args) override {
			return this->m_lookNfeel->draw(ctrl, args);
		}
	
		void
		draw(RadioButtonControl& ctrl, OwnerDrawEventArgs const& args) override {
			return this->m_lookNfeel->draw(ctrl, args);
		}

		void
		draw(StaticControl& ctrl, OwnerDrawEventArgs const& args) override {
			return this->m_lookNfeel->draw(ctrl, args);
		}

		void
		draw(Dialog& dlg, PaintWindowEventArgs const& args) override {
			return this->m_lookNfeel->draw(dlg, args);
		}
	
		void
		draw(Window& wnd, PaintNonClientEventArgs const& args) override {
			return this->m_lookNfeel->draw(wnd, args);
		}
		
		void
		initialize(Dialog& dlg, InitDialogEventArgs const& args) override {
			return this->m_lookNfeel->initialize(dlg, args);
		}
	};
	
}	// namespace core::forms