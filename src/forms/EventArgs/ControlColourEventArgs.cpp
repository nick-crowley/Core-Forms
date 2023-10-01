#include "forms/EventArgs/ControlColourEventArgs.h"
#include "forms/Window.h"
using namespace core;
using namespace forms;

forms::ControlColourEventArgs::ControlColourEventArgs(::UINT msg, ::WPARAM hdc, ::LPARAM wnd)
  : Graphics{SharedDeviceContext{reinterpret_cast<::HDC>(hdc), weakref}},
	Managed{Window::ExistingWindows.find(reinterpret_cast<::HWND>(wnd))},
	Window{reinterpret_cast<::HWND>(wnd)},
	Source{static_cast<SupportedSender>(msg)}
{
}
