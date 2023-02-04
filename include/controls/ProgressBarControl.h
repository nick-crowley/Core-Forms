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
			::WNDPROC	OriginalWndProc;

		public:
			ProgressBarWindowClass() : WindowClass{ResourceId{PROGRESS_CLASS}}  {
				this->Name = ResourceId{L"Custom.PROGRESS"};
				this->OriginalWndProc = std::exchange(this->WndProc, Window::DefaultMessageHandler);
				this->register_();
			}
		};

	public:
		ProgressBarControl(uint16_t id) 
		  : Control{id}
		{}
	
	public:
		bool
		ownerDraw() const override {
			return false;
		}

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
			return ::CallWindowProc(wndcls().OriginalWndProc, hWnd, message, wParam, lParam);
		}
	};
}	// namespace core::forms