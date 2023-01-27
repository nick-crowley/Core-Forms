#pragma once
#include "library/core.Forms.h"
#include "controls/Control.h"
#include "controls/StaticStyle.h"
#include "windows/WindowClass.h"

namespace core::forms
{
	class StaticControl : public Control {
		class StaticWindowClass : public WindowClass {
		public:
			::WNDPROC	OriginalMessageHandler;

		public:
			StaticWindowClass() : WindowClass{ResourceId{WC_STATIC}}  {
				this->name(ResourceId{L"Custom.STATIC"});
				this->OriginalMessageHandler = std::exchange(this->lpfnWndProc, Window::DefaultMessageHandler);
				this->register$();
			}
		};
		
		class StaticNotificationDictionary : public MessageDictionary {
			using base = MessageDictionary;
		public:
			StaticNotificationDictionary() : base({
	#define MakeMessageName(msg)  { msg, #msg }
				MakeMessageName(STN_CLICKED),
				MakeMessageName(STN_ENABLE),
				MakeMessageName(STN_DISABLE),
				MakeMessageName(STN_DBLCLK),
	#undef MakeMessageName
				})
			{}
		};

	public:
		StaticControl(uint16_t id) 
		  : Control{id}
		{}
	
	public:
		StaticWindowClass const& 
		wndcls() override {
			static StaticWindowClass c;
			return c;
		}
	
		Response 
		onOwnerDraw(OwnerDrawEventArgs args) override {
			if (args.Ident == this->ident()) {
				this->LookNFeel->draw(*this, args);
				return TRUE;
			}

			return Unhandled;
		}
	
	protected:
		gsl::czstring
		notificationName(::UINT notification) override {
			static const StaticNotificationDictionary names;
			return names.at(notification);
		}

		::LRESULT 
		unhandledMessage(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
			return ::CallWindowProc(this->wndcls().OriginalMessageHandler, hWnd, message, wParam, lParam);
		}
	};
}	// namespace core::forms