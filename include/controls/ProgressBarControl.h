#pragma once

#include "formsFramework.h"
#include "windows/Window.h"
#include "windows/WindowClass.h"
#include "controls/ProgressBarStyle.h"

class ProgressBarControl : public Window {
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
	ProgressBarControl() 
	{}
	
public:
	ProgressBarWindowClass const& 
	wndcls() override {
		static ProgressBarWindowClass c;
		return c;
	}

protected:
	::LRESULT 
	unhandled_message(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
		return ::CallWindowProc(wndcls().OriginalMessageHandler, hWnd, message, wParam, lParam);
	}
};
