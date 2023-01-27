#pragma once
#include "library/core.Forms.h"
#include "controls/Control.h"
#include "controls/EditStyle.h"
#include "windows/WindowClass.h"

namespace core::forms
{
	class EditControl : public Control {
		class EditWindowClass : public WindowClass {
		public:
			::WNDPROC	OriginalMessageHandler;

		public:
			EditWindowClass() : WindowClass{ResourceId{WC_EDIT}}  {
				this->name(ResourceId{L"Custom.EDIT"});
				this->OriginalMessageHandler = std::exchange(this->lpfnWndProc, Window::DefaultMessageHandler);
				this->register$();
			}
		};

		class EditNotificationDictionary : public MessageDictionary {
			using base = MessageDictionary;
		public:
			EditNotificationDictionary() : base({
	#define MakeMessageName(msg)  { msg, #msg }
				MakeMessageName(EN_SETFOCUS),
				MakeMessageName(EN_KILLFOCUS),
				MakeMessageName(EN_CHANGE),
				MakeMessageName(EN_UPDATE),
				MakeMessageName(EN_ERRSPACE),
				MakeMessageName(EN_MAXTEXT),
				MakeMessageName(EN_HSCROLL),
				MakeMessageName(EN_VSCROLL),
				MakeMessageName(EN_ALIGN_LTR_EC),
				MakeMessageName(EN_ALIGN_RTL_EC),
				MakeMessageName(EN_BEFORE_PASTE),
				MakeMessageName(EN_AFTER_PASTE)
	#undef MakeMessageName
				})
			{}
		};

	public:
		EditControl(uint16_t id) 
		  : Control{id}
		{}
	
	public:
		EditWindowClass const& 
		wndcls() override {
			static EditWindowClass c;
			return c;
		}

	protected:
		gsl::czstring
		notificationName(::UINT notification) override {
			static const EditNotificationDictionary names;
			return names.at(notification);
		}

		::LRESULT 
		unhandledMessage(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
			return ::CallWindowProc(wndcls().OriginalMessageHandler, hWnd, message, wParam, lParam);
		}
	};
}	// namespace core::forms
