#pragma once
#include "library/core.Forms.h"
#include "controls/Control.h"
#include "windows/WindowClass.h"

namespace core::forms
{
	class ListViewControl : public Control {
		class ListViewWindowClass : public WindowClass {
		public:
			::WNDPROC	OriginalMessageHandler;

		public:
			ListViewWindowClass() : WindowClass{ResourceId{WC_LISTVIEW}}  {
				this->name(ResourceId{L"Custom.LISTVIEW"});
				this->OriginalMessageHandler = std::exchange(this->lpfnWndProc, Window::DefaultMessageHandler);
				this->register$();
			}
		};
	
		class ListViewNotificationDictionary : public MessageDictionary {
			using base = MessageDictionary;
		public:
			ListViewNotificationDictionary() : base({
	#define MakeMessageName(msg)  { msg, #msg }
				MakeMessageName(LVN_ITEMCHANGING),
				MakeMessageName(LVN_ITEMCHANGED),
				MakeMessageName(LVN_INSERTITEM),
				MakeMessageName(LVN_DELETEITEM),
				MakeMessageName(LVN_DELETEALLITEMS),
				MakeMessageName(LVN_BEGINLABELEDIT),
				MakeMessageName(LVN_ENDLABELEDIT),
				MakeMessageName(LVN_COLUMNCLICK),
				MakeMessageName(LVN_BEGINDRAG),
				MakeMessageName(LVN_BEGINRDRAG),
				MakeMessageName(LVN_ODCACHEHINT),
				MakeMessageName(LVN_ODFINDITEM),
				MakeMessageName(LVN_ITEMACTIVATE),
				MakeMessageName(LVN_ODSTATECHANGED),
				MakeMessageName(LVN_ODFINDITEM),
				MakeMessageName(LVN_HOTTRACK),
				MakeMessageName(LVN_GETDISPINFO),
				MakeMessageName(LVN_SETDISPINFO),
				MakeMessageName(LVN_BEGINLABELEDIT),
				MakeMessageName(LVN_ENDLABELEDIT),
				MakeMessageName(LVN_GETDISPINFO),
				MakeMessageName(LVN_SETDISPINFO),
				MakeMessageName(LVN_KEYDOWN),
				MakeMessageName(LVN_MARQUEEBEGIN),
				MakeMessageName(LVN_GETINFOTIP),
				MakeMessageName(LVN_INCREMENTALSEARCH),
				MakeMessageName(LVN_COLUMNDROPDOWN),
				MakeMessageName(LVN_COLUMNOVERFLOWCLICK),
				MakeMessageName(LVN_BEGINSCROLL),
				MakeMessageName(LVN_ENDSCROLL),
				MakeMessageName(LVN_LINKCLICK),
				MakeMessageName(LVN_GETEMPTYMARKUP)
	#undef MakeMessageName
				})
			{}
		};

	public:
		ListViewControl(uint16_t id) 
		  : Control{id}
		{}
		
	public:
		bool
		ownerDraw() const override {
			return false;		// FIXME: Implement ListViewControl::ownerDraw()
		}

	public:
		ListViewWindowClass const& 
		wndcls() override {
			static ListViewWindowClass c;
			return c;
		}

	protected:
		gsl::czstring
		notificationName(::UINT notification) override {
			static const ListViewNotificationDictionary names;
			return names.at(notification);
		}

		/*Response
		offerNotification(::UINT notification) override {
			auto const on_exit = this->Debug.setTemporaryState(
				{ProcessingState::NotificationProcessing, this->notificationName(notification)}
			); 
			switch (notification) {
			case BN_CLICKED:
				this->Clicked.raise();
				return 0;
			}

			return Unhandled;
		}*/

		::LRESULT 
		unhandledMessage(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
			return ::CallWindowProc(this->wndcls().OriginalMessageHandler, hWnd, message, wParam, lParam);
		}
	};
}	// namespace core::forms