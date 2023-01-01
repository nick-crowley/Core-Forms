#pragma once
#include "formsFramework.h"
#include "windows/Window.h"
#include "windows/WindowClass.h"
#include "controls/EditStyle.h"
#include "system/MessageDictionary.h"

class EditControl : public Window {
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
	EditControl() 
	{}
	
public:
	EditWindowClass const& 
	wndcls() override {
		static EditWindowClass c;
		return c;
	}

protected:
	const char*
	notification_name(::UINT notification) override {
		static const EditNotificationDictionary names;
		return names.at(notification);
	}

	::LRESULT 
	unhandled_message(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
		return ::CallWindowProc(wndcls().OriginalMessageHandler, hWnd, message, wParam, lParam);
	}
};

