#pragma once
#include "formsFramework.h"
#include "windows/Window.h"
#include "windows/WindowClass.h"
#include "controls/StaticStyle.h"

namespace core::forms
{
	class StaticControl : public Window {
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

	public:
		StaticControl() 
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
		::LRESULT 
		unhandled_message(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
			return ::CallWindowProc(this->wndcls().OriginalMessageHandler, hWnd, message, wParam, lParam);
		}
	};
}	// namespace core::forms