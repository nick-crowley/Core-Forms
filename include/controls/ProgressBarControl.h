#pragma once
#include "library/core.Forms.h"
#include "controls/Control.h"
#include "controls/ProgressBarStyle.h"
#include "windows/WindowClass.h"

namespace core::forms
{
	class ProgressBarControl : public Control {
		class ProgressBarWindowClass : public WindowClass {
		public:
			::WNDPROC	OriginalMessageHandler;

		public:
			ProgressBarWindowClass() : WindowClass{ResourceId{PROGRESS_CLASS}}  {
				this->name(ResourceId{L"Custom.PROGRESS"});
				this->OriginalMessageHandler = std::exchange(this->lpfnWndProc, Window::DefaultMessageHandler);
				this->register$();
			}
		};

	public:
		ProgressBarControl(uint16_t id) 
		  : Control{id}
		{}
	
	public:
		ProgressBarWindowClass const& 
		wndcls() override {
			static ProgressBarWindowClass c;
			return c;
		}

	protected:
		gsl::czstring
		notificationName(::UINT notification) override {
			return "Unknown";
		}

		::LRESULT 
		unhandledMessage(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
			return ::CallWindowProc(wndcls().OriginalMessageHandler, hWnd, message, wParam, lParam);
		}
	};
}	// namespace core::forms