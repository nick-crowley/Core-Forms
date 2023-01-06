#include "windows/WindowEventArgs.h"

bool
PaintNonClientEventArgs::beginPaint()
{
	auto constinit
	static Flags = DCX_WINDOW|DCX_CACHE|DCX_LOCKWINDOWUPDATE;

	::HDC dc {};
	if (this->InvalidArea) 
		dc = ::GetDCEx(this->Window->handle(), Region{*this->InvalidArea}.detach(), Flags|DCX_INTERSECTRGN);
	else 
		dc = ::GetDCEx(this->Window->handle(), nullptr, Flags);
	if (!dc)
		return false;

	Rect rcClient;
	Point ptClient;
	::GetClientRect(this->Window->handle(), rcClient);
	::ClientToScreen(this->Window->handle(), ptClient);
	rcClient += ptClient;
	Rect rcWindow;
	::GetWindowRect(this->Window->handle(), rcWindow);
	rcClient -= rcWindow.topLeft();
	rcWindow -= rcWindow.topLeft();
	this->Area = rcWindow;
	this->Area -= Region{rcClient};
	this->Bounds = rcWindow;
	this->Graphics = DeviceContext{dc, this->Window->handle()};
	if (this->State == CaptionState::Unknown) {
		::WINDOWINFO info{sizeof(info)};
		::GetWindowInfo(this->Window->handle(), &info);
		this->State = info.dwWindowStatus == WS_ACTIVECAPTION ? CaptionState::Active : CaptionState::Inactive;
	}
	return true;
}

void 
PaintNonClientEventArgs::endPaint() {
	::ReleaseDC(this->Window->handle(), this->Graphics->handle());
	this->Graphics.reset();
}
