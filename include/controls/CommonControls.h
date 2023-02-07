#pragma once
#include "library/core.Forms.h"
#include "dialogs/Dialog.h"
#include "controls/ButtonControl.h"
#include "controls/CheckBoxControl.h"
#include "controls/EditControl.h"
#include "controls/LabelControl.h"
#include "controls/GroupBoxControl.h"
#include "controls/PictureControl.h"
#include "controls/RadioButtonControl.h"
#include "controls/StaticControl.h"
#include "controls/ComboBoxControl.h"
#include "controls/ListBoxControl.h"
#include "controls/ListViewControl.h"
#include "controls/ProgressBarControl.h"
#pragma comment(lib, "Comctl32.lib")

namespace core::forms
{
	namespace detail {
		class CommCtrl32Registration {
		public:
			CommCtrl32Registration() {
				::INITCOMMONCONTROLSEX cmnControls{sizeof ::INITCOMMONCONTROLSEX, ICC_WIN95_CLASSES|ICC_LINK_CLASS};
				::InitCommonControlsEx(&cmnControls);
			}
		};
	}

	class ControlRegistration : private detail::CommCtrl32Registration
	{
		Dialog::WindowClass Dialogs;
		ButtonControl::WindowClass Buttons;
		ComboBoxControl::WindowClass ComboBoxes;
		EditControl::WindowClass Edits;
		ListBoxControl::WindowClass ListBoxes;
		ListViewControl::WindowClass ListViews;
		ProgressBarControl::WindowClass ProgressBars;
		StaticControl::WindowClass Statics;

	public:
		satisfies(ControlRegistration, 
			IsDefaultConstructible, 
			NotCopyable, 
			NotMovable,
			NotEqualityComparable,
			NotSortable
		);
	};
}