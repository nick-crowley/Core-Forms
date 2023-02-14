#pragma once
#include "windows/ILookNFeelProvider.h"

namespace core::forms
{
	struct FormsExport Win31LookNFeel : public ILookNFeelProvider
	{
		void
		draw(ButtonControl& ctrl, OwnerDrawEventArgs const& args) override;
	
		void
		draw(CheckBoxControl& ctrl, OwnerDrawEventArgs const& args) override;
	
		void
		draw(LabelControl& ctrl, OwnerDrawEventArgs const& args) override;
	
		void
		draw(ListBoxControl& ctrl, OwnerDrawEventArgs const& args) override;
	
		void
		erase(ListBoxControl& ctrl, EraseBackgroundEventArgs const& args) override;
	
		void
		draw(GroupBoxControl& ctrl, OwnerDrawEventArgs const& args) override;
	
		void
		draw(PictureControl& ctrl, OwnerDrawEventArgs const& args) override;
	
		void
		draw(RadioButtonControl& ctrl, OwnerDrawEventArgs const& args) override;

		void
		draw(StaticControl& ctrl, OwnerDrawEventArgs const& args) override;
	
		void
		draw(Dialog& dlg, PaintWindowEventArgs const& args) override;
	
		void
		draw(Window& wnd, PaintNonClientEventArgs const& args) override;
		
		void
		initialize(Dialog& dlg, InitDialogEventArgs const& args) override;
	};
}	// namespace core::forms