#pragma once
#include "library/core.Forms.h"
#include "controls/Control.h"
#include "controls/StaticStyle.h"
#include "windows/WindowClass.h"

namespace core::forms
{
	class StaticControl : public Control 
	{
	public:
		class WindowClass : public forms::WindowClass {
		public:
			::WNDPROC	OriginalWndProc;

		public:
			WindowClass() : forms::WindowClass{ResourceId{WC_STATIC}}  {
				this->Name = ResourceId{L"Custom.STATIC"};
				this->OriginalWndProc = std::exchange(this->WndProc, Window::DefaultMessageHandler);
				this->Style |= ClassStyle::GlobalClass;
				this->register_();
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
		implicit
		StaticControl(uint16_t id) 
		  : Control{id}
		{}
	
	public:
		bool
		ownerDraw() const override {
			return (this->style<StaticStyle>() & StaticStyle::TypeMask).test(StaticStyle::OwnerDraw);
		}
		
		WindowRole
		role() const override {
			return WindowRole::StaticText;
		}

	public:
		WindowClass const& 
		wndcls() const override {
			static WindowClass c;
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
			return ::CallWindowProc(this->wndcls().OriginalWndProc, hWnd, message, wParam, lParam);
		}
	};
}	// namespace core::forms