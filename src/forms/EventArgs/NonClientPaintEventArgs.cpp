#include "forms/EventArgs/NonClientPaintEventArgs.h"
#include "forms/Window.h"
using namespace core;
using namespace forms;

bool
forms::NonClientPaintEventArgs::beginPaint()
{
	auto constexpr
	static Flags = DCX_WINDOW|DCX_CACHE|DCX_LOCKWINDOWUPDATE;

	::HDC dc {};
	if (this->InvalidArea) 
		dc = ::GetDCEx(this->Window.handle(), Region{*this->InvalidArea}.detach(), Flags|DCX_INTERSECTRGN);
	else 
		dc = ::GetDCEx(this->Window.handle(), nullptr, Flags);
	if (!dc)
		return false;

	// Retrieve window and client rect in window co-ordinates (with origin at {0,0})
	Rect const rcWindow = this->Window.wndRect();
	this->Bounds = rcWindow - rcWindow.topLeft();
	this->Client = this->Window.clientRect(nullptr) - rcWindow.topLeft();
	// Prefer pre-existing window region, if any; otherwise exclude client-area from window-rectangle
	if (auto rgn = this->Window.wndRgn(); rgn)
		this->Area = *rgn;
	else 
		this->Area = Region{this->Bounds} - Region{this->Client};
	this->Graphics = DeviceContext{SharedDeviceContext{dc}};
	if (this->CaptionState == WindowCaptionState::Unknown)
		this->CaptionState = this->Window.info().Caption;
	
	return true;
}

void 
forms::NonClientPaintEventArgs::endPaint() {
	this->Graphics.reset();
}
