#include "forms/EventArgs/PaintNonClientEventArgs.h"
#include "forms/Window.h"
using namespace core;
using namespace forms;

bool
forms::PaintNonClientEventArgs::beginPaint()
{
	auto constexpr
	static Flags = DCX_WINDOW|DCX_CACHE|DCX_LOCKWINDOWUPDATE;

	::HDC dc {};
	if (this->InvalidArea) 
		dc = ::GetDCEx(this->Window->handle(), Region{*this->InvalidArea}.detach(), Flags|DCX_INTERSECTRGN);
	else 
		dc = ::GetDCEx(this->Window->handle(), nullptr, Flags);
	if (!dc)
		return false;

	// Retrieve window rect with origin at {0,0}
	Rect rcWindow = this->Window->wndRect();
	rcWindow -= rcWindow.topLeft();
	if (auto rgn = this->Window->wndRgn(); rgn)
		this->Area = *rgn;
	else {
		// Retrieve client rect in window co-ordinates with origin at {0,0}
		Rect rcClient = this->Window->clientRect(nullptr);
		rcClient -= rcWindow.topLeft();
		this->Area = rcWindow;
		this->Area -= Region{rcClient};
	}
	this->Bounds = rcWindow;
	this->Graphics = DeviceContext{dc, this->Window->handle()};
	if (this->State == WindowCaptionState::Unknown)
		this->State = this->Window->info().State;
	
	return true;
}

void 
forms::PaintNonClientEventArgs::endPaint() {
	::ReleaseDC(this->Window->handle(), this->Graphics->handle());
	this->Graphics.reset();
}
