#pragma once
#include "library/core.Forms.h"

namespace core::forms
{
	class Window;
	class Dialog;
	class ButtonControl;
	class CheckBoxControl;
	class LabelControl;
	class ListBoxControl;
	class GroupBoxControl;
	class PictureControl;
	class RadioButtonControl;
	class StaticControl;
	class EraseBackgroundEventArgs;
	class InitDialogEventArgs;
	class OwnerDrawEventArgs;
	class PaintWindowEventArgs;
	class PaintNonClientEventArgs;

	struct ILookNFeelProvider
	{
		satisfies(ILookNFeelProvider,
			protected: IsDefaultConstructible,
			public: IsPolymorphic
		);

		void
		virtual draw(ButtonControl& ctrl, OwnerDrawEventArgs const& args) abstract;
	
		void
		virtual draw(CheckBoxControl& ctrl, OwnerDrawEventArgs const& args) abstract;
	
		void
		virtual draw(LabelControl& ctrl, OwnerDrawEventArgs const& args) abstract;
	
		void
		virtual draw(ListBoxControl& ctrl, OwnerDrawEventArgs const& args) abstract;
	
		void
		virtual erase(ListBoxControl& ctrl, EraseBackgroundEventArgs const& args) abstract;
	
		void
		virtual draw(GroupBoxControl& ctrl, OwnerDrawEventArgs const& args) abstract;
	
		void
		virtual draw(PictureControl& ctrl, OwnerDrawEventArgs const& args) abstract;
	
		void
		virtual draw(RadioButtonControl& ctrl, OwnerDrawEventArgs const& args) abstract;

		void
		virtual draw(StaticControl& ctrl, OwnerDrawEventArgs const& args) abstract;

		void
		virtual draw(Dialog& dlg, PaintWindowEventArgs const& args) abstract;
	
		void
		virtual draw(Window& wnd, PaintNonClientEventArgs const& args) abstract;
		
		void
		virtual initialize(Dialog& dlg, InitDialogEventArgs const& args) abstract;
	};
}	// namespace core::forms