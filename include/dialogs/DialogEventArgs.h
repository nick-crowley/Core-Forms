#pragma once
#include "library/core.Forms.h"
#include "support/ObservableEvent.h"

namespace core::forms
{
	struct InitDialogEventArgs {
		static_assert(sizeof(::LPARAM) == sizeof(void*));

		::HWND		InitialFocus;
		::LPARAM	CustomData;

		InitDialogEventArgs(::WPARAM focusCtrl, ::LPARAM data) 
		  : InitialFocus(reinterpret_cast<::HWND>(focusCtrl)),
			CustomData(data) 
		{}

		template <typename Pointer>
		Pointer
		data() {
			static_assert(std::is_pointer_v<Pointer>);
			return reinterpret_cast<Pointer>(this->CustomData);
		}
	};

	using InitDialogDelegate = Delegate<void (InitDialogEventArgs)>;
	using InitDialogEvent = ObservableEvent<InitDialogDelegate>;

	struct LoadDialogEventArgs 
	{
		DialogTemplate&  Template;

		implicit
		LoadDialogEventArgs(DialogTemplate& dlg) 
		  : Template{dlg}
		{}
	};
}	// namespace core::forms