#pragma once
#include "library/core.Forms.h"
#include "controls/Control.h"
#include "controls/EditStyle.h"
#include "forms/WindowClass.h"

namespace core::forms
{
	class EditControl : public Control {
	public:
		class WindowClass : public forms::WindowClass {
		public:
			::WNDPROC	OriginalWndProc;

		public:
			WindowClass() : forms::WindowClass{ResourceId{WC_EDIT}}  {
				this->Name = ResourceId{L"Custom.EDIT"};
				this->OriginalWndProc = std::exchange(this->WndProc, Window::DefaultMessageHandler);
				this->Style |= ClassStyle::GlobalClass;
				this->register_();
			}
		};

	protected:
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
		WindowEvent    TextChanged;

	public:
		implicit
		EditControl(uint16_t id) 
		  : Control{id}
		{}
	
	public:
		bool
		ownerDraw() const override {
			return false;
		}
		
		WindowRole
		role() const override {
			return WindowRole::Text;
		}

	public:
		WindowClass const& 
		wndcls() const override {
			static WindowClass c;
			return c;
		}

	protected:
		gsl::czstring
		notificationName(::UINT notification) override {
			static const EditNotificationDictionary names;
			return names.at(notification);
		}
		
		Response
		offerNotification(::UINT notification) override {
			auto const on_exit = this->Debug.setTemporaryState(
				{ProcessingState::NotificationProcessing, this->notificationName(notification)}
			); 

			switch (notification) {
			case EN_CHANGE:
				this->TextChanged.raise(*this);
				return 0;
			}

			return Unhandled;
		}

		::LRESULT 
		unhandledMessage(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
			return ::CallWindowProc(wndcls().OriginalWndProc, hWnd, message, wParam, lParam);
		}
	};
}	// namespace core::forms
