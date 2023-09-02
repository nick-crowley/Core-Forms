#include "forms/WindowEventArgs.h"
#include "forms/Window.h"
using namespace core;
using namespace forms;

forms::OwnerDrawEventArgs::ItemData::ItemData(::DRAWITEMSTRUCT& data) 
  : Area{data.rcItem},
    Data{data.itemData},
    State{static_cast<OwnerDrawState>(data.itemState)}
{
	if (data.CtlType != ODT_MENU)
		this->Ident = static_cast<uint32_t>(data.itemID);
	else
		this->Ident = win::ResourceId{static_cast<uint16_t>(data.itemID)};
}

forms::OwnerDrawEventArgs::OwnerDrawEventArgs(::WPARAM id, ::LPARAM data) 
  : OwnerDrawEventArgs{*reinterpret_cast<::DRAWITEMSTRUCT*>(data)}
{
	Expects(id != 0);
}

forms::OwnerDrawEventArgs::OwnerDrawEventArgs(::DRAWITEMSTRUCT& data) 
  : Action{static_cast<OwnerDrawAction>(data.itemAction)},
    Ident{static_cast<uint16_t>(data.CtlID)},
	Item{data},
	Graphics{data.hDC, data.hwndItem},
	Type{static_cast<OwnerDrawControl>(data.CtlType)},
	Window{Window::ExistingWindows.find(data.hwndItem)}
{
}

forms::OwnerDrawMenuEventArgs::OwnerDrawMenuEventArgs(::WPARAM id, ::LPARAM data) 
  : OwnerDrawMenuEventArgs{*reinterpret_cast<::DRAWITEMSTRUCT*>(data)}
{
	Expects(id == 0);
}

forms::OwnerDrawMenuEventArgs::OwnerDrawMenuEventArgs(::DRAWITEMSTRUCT& data)
  : Action{static_cast<OwnerDrawAction>(data.itemAction)},
	Item{data},
	Menu{reinterpret_cast<::HMENU>(data.hwndItem)},
    Graphics{data.hDC, ::WindowFromDC(data.hDC)}
{
}

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

void 
forms::PaintWindowEventArgs::beginPaint() 
{
	if (auto dc = ::BeginPaint(this->Window->handle(), &this->Data)) {
		this->Graphics = DeviceContext{dc, this->Window->handle()};
		this->Area = this->Data.rcPaint;
		this->Erase = this->Data.fErase;
		this->Restore = this->Data.fRestore;
		this->Update = this->Data.fIncUpdate;
	}
}

void 
forms::PaintWindowEventArgs::endPaint() 
{
	if (::EndPaint(this->Window->handle(), &this->Data)) {
		this->Graphics.reset();
		this->Area.reset();
		this->Erase.reset();
		this->Restore.reset();
		this->Update.reset();
	}
}
