#pragma once
#include "library/core.Forms.h"
#include "controls/Control.h"
#include "controls/LinkStyle.h"
#include "windows/WindowClass.h"

namespace core::forms
{
	class LinkControl : public Control {
	public:
		class WindowClass : public forms::WindowClass {
		public:
			::WNDPROC	OriginalWndProc;

		public:
			WindowClass() : forms::WindowClass{ResourceId{WC_LINK}}  {
				this->Name = ResourceId{L"Custom.LINK"};
				this->OriginalWndProc = std::exchange(this->WndProc, Window::DefaultMessageHandler);
				this->Style |= ClassStyle::GlobalClass;
				this->register_();
			}
		};

	protected:
		class NotificationDictionary : public MessageDictionary {
			using base = MessageDictionary;
		public:
			NotificationDictionary() : base({
	#define MakeMessageName(msg)  { msg, #msg }
				MakeMessageName(NM_CLICK)
	#undef MakeMessageName
				})
			{}
		};

	public:
		LinkControl(uint16_t id) 
		  : Control{id}
		{}
	
	public:
		bool
		ownerDraw() const override {
			return false;
		}

	public:
		WindowClass const& 
		wndcls() override {
			static WindowClass c;
			return c;
		}

	protected:
		gsl::czstring
		notificationName(::UINT notification) override {
			static const NotificationDictionary names;
			return names.at(notification);
		}

		::LRESULT 
		unhandledMessage(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
			return ::CallWindowProc(wndcls().OriginalWndProc, hWnd, message, wParam, lParam);
		}
	};
}	// namespace core::forms
