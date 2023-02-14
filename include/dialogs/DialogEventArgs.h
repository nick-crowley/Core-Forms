#pragma once
#include "library/core.Forms.h"
#include "support/ObservableEvent.h"

namespace core::forms
{
	class InitDialogEventArgs {
		static_assert(sizeof(::LPARAM) == sizeof(void*));

	public:
		::HWND		InitialFocus;
		::LPARAM	CustomData;

	public:
		InitDialogEventArgs(::WPARAM focusCtrl, ::LPARAM data) 
		  : InitialFocus(reinterpret_cast<::HWND>(focusCtrl)),
			CustomData(data) 
		{}

	public:
		template <typename Pointer>
		Pointer
		data() {
			static_assert(std::is_pointer_v<Pointer>);
			return reinterpret_cast<Pointer>(this->CustomData);
		}
	};

	using InitDialogDelegate = Delegate<void (InitDialogEventArgs)>;
	using InitDialogEvent = ObservableEvent<InitDialogDelegate>;

	class LoadDialogEventArgs 
	{
	public:
		DialogTemplate&  Template;

	public:
		implicit
		LoadDialogEventArgs(DialogTemplate& dlg) 
		  : Template{dlg}
		{}
	};
}	// namespace core::forms