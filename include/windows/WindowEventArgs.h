#pragma once
#include "formsFramework.h"
#include "support/ObservableEvent.h"
#include "graphics/Graphics.h"

struct CreateWindowEventArgs {
	static_assert(sizeof(LPARAM) == sizeof(CREATESTRUCT*));

	CREATESTRUCT*	Data;

	CreateWindowEventArgs(WPARAM, LPARAM data) : Data(reinterpret_cast<CREATESTRUCT*>(data)) 
	{}

	template <typename Pointer>
	Pointer
	data() {
		static_assert(std::is_pointer_v<Pointer>);
		return static_cast<Pointer>(this->Data->lpCreateParams);
	}
};

using CreateWindowDelegate = Delegate<void (CreateWindowEventArgs)>;
using CreateWindowEvent = ObservableEvent<CreateWindowDelegate>;


struct MinMaxEventArgs {
	static_assert(sizeof(LPARAM) == sizeof(MINMAXINFO*));

	MINMAXINFO*   Extents;

	MinMaxEventArgs(WPARAM, LPARAM ext) : Extents(reinterpret_cast<MINMAXINFO*>(ext)) {
	}
};


class EraseBackgroundEventArgs
{
public:
	mutable DeviceContext  Graphics;

public:
	EraseBackgroundEventArgs(::HWND wnd, ::WPARAM w, ::LPARAM)
		: Graphics{reinterpret_cast<::HDC>(w), wnd}
	{}
};


class OwnerDrawEventArgs 
{
public:
	enum EventSource { Menu, Control };

	struct ItemData {
		EnumBitset<OwnerDrawAction> Action;
		Rect                        Area;
		uintptr_t                   Data;
		UINT                        Id;
		EnumBitset<OwnerDrawState>  State;
	};

	struct ControlData {
		OwnerDrawControl    Type;
		uint16_t            Id;
	};
	
public:
	std::optional<uint16_t> Ident;
	EventSource             Source;
	ControlData             Ctrl;
	ItemData                Item;
	::HWND                  Window;
	mutable DeviceContext   Graphics;

public:
	OwnerDrawEventArgs(::WPARAM id, ::LPARAM data) 
	  : Source{id ? Control : Menu},
		Ctrl{static_cast<OwnerDrawControl>(
			     reinterpret_cast<::DRAWITEMSTRUCT*>(data)->CtlType
			 ),
			 static_cast<uint16_t>(
				reinterpret_cast<::DRAWITEMSTRUCT*>(data)->CtlID
			 )},
		Item{static_cast<OwnerDrawAction>(
			   reinterpret_cast<::DRAWITEMSTRUCT*>(data)->itemAction
			 ),
		     reinterpret_cast<::DRAWITEMSTRUCT*>(data)->rcItem,
		     reinterpret_cast<::DRAWITEMSTRUCT*>(data)->itemData,
		     reinterpret_cast<::DRAWITEMSTRUCT*>(data)->itemID,
		     static_cast<OwnerDrawState>(
		         reinterpret_cast<::DRAWITEMSTRUCT*>(data)->itemState
			 )},
		Window{reinterpret_cast<::DRAWITEMSTRUCT*>(data)->hwndItem},
	    Graphics{reinterpret_cast<::DRAWITEMSTRUCT*>(data)->hDC,
	             reinterpret_cast<::DRAWITEMSTRUCT*>(data)->hwndItem}
	{
		if (Source == Control)
			Ident = static_cast<uint16_t>(id);
	}
};

using OwnerDrawDelegate = Delegate<void (OwnerDrawEventArgs)>;
using OwnerDrawEvent = ObservableEvent<OwnerDrawDelegate>;


struct TimerEventArgs 
{
	uintptr_t Ident;

	TimerEventArgs(::WPARAM w, ::LPARAM) 
	  : Ident(static_cast<uintptr_t>(w)) 
	{
	}
};

using TimerDelegate = Delegate<void (TimerEventArgs)>;
using TimerEvent = ObservableEvent<TimerDelegate>;


struct SetFontEventArgs {
	SetFontEventArgs(WPARAM w, LPARAM l) {
	}
};


struct ShowWindowEventArgs {
	unsigned	Flags;

	ShowWindowEventArgs(WPARAM w, LPARAM l) : Flags((unsigned)l) {
	}
};

using ShowWindowDelegate = Delegate<void (ShowWindowEventArgs)>;
using ShowWindowEvent = ObservableEvent<ShowWindowDelegate>;


class ActivateNonClientEventArgs {
public:
	enum CaptionState {Inactive = FALSE, Active = TRUE, Unknown = -1};
	
	std::optional<Region>  InvalidArea;
	CaptionState           State;
	::HWND                 Window;
	bool                   Repaint;

public:
	ActivateNonClientEventArgs(::HWND window, ::WPARAM w, ::LPARAM l) 
	  : State{static_cast<CaptionState>(w)},
		Window{window},
		Repaint{l != -1}
	{
		if (l > NULLREGION && !::IsAppThemed())
			this->InvalidArea = reinterpret_cast<::HRGN>(l);
	}

	~ActivateNonClientEventArgs() noexcept 
	{
		if (this->InvalidArea)
			this->InvalidArea->detach();
	}
};

using ActivateNonClientDelegate = Delegate<void (ActivateNonClientEventArgs)>;
using ActivateNonClientEvent = ObservableEvent<ActivateNonClientDelegate>;


enum class WindowHitTest
{
	Border			= HTBORDER,			//!< In the border of a window that does not have a sizing border.
	Bottom			= HTBOTTOM,			//!< In the lower-horizontal border of a resizable window (the user can click the mouse to resize the window vertically).
	BottomLeft		= HTBOTTOMLEFT,		//!< In the lower-left corner of a border of a resizable window (the user can click the mouse to resize the window diagonally).
	BottomRight		= HTBOTTOMRIGHT,	//!< In the lower-right corner of a border of a resizable window (the user can click the mouse to resize the window diagonally).
	Caption			= HTCAPTION,		//!< In a title bar.
	Client			= HTCLIENT,			//!< In a client area.
	Close			= HTCLOSE,			//!< In a Close button.
	Error			= HTERROR,			//!< On the screen background or on a dividing line between windows (same as HTNOWHERE, except that the DefWindowProc function produces a system beep to indicate an error).
	Help			= HTHELP,			//!< In a Help button.
	HScroll			= HTHSCROLL,		//!< In a horizontal scroll bar.
	Left			= HTLEFT,			//!< In the left border of a resizable window (the user can click the mouse to resize the window horizontally).
	Menu			= HTMENU,			//!< In a menu.
	MaxButton		= HTMAXBUTTON,		//!< In a Maximize button.
	MinButton		= HTMINBUTTON,		//!< In a Minimize button.
	Nowhere			= HTNOWHERE,		//!< On the screen background or on a dividing line between windows.
	Right			= HTRIGHT,			//!< In the right border of a resizable window (the user can click the mouse to resize the window horizontally).
	Size			= HTSIZE,			//!< In a size box (same as HTGROWBOX).
	SysMenu			= HTSYSMENU,		//!< In a window menu or in a Close button in a child window.
	Top				= HTTOP,			//!< In the upper-horizontal border of a window.
	TopLeft			= HTTOPLEFT,		//!< In the upper-left corner of a window border.
	TopRight		= HTTOPRIGHT,		//!< In the upper-right corner of a window border.
	Transparent		= HTTRANSPARENT,	//!< In a window currently covered by another window in the same thread (the message will be sent to underlying windows in the same thread until one of them returns a code that is not HTTRANSPARENT).
	VScroll			= HTVSCROLL,		//!< In the vertical scroll bar.
};

enum class MouseButton 
{
	None, Left, Middle, Right
};

enum class MouseEvent
{
	ButtonUp, ButtonDown, DoubleClick, Move, Hover
};

class MouseNonClientEventArgs {
public:
	MouseEvent    Event;
	MouseButton   Button;
	WindowHitTest Object;
	Point         Position;
	
public:
	MouseNonClientEventArgs(MouseEvent evn, MouseButton btn, ::WPARAM w, ::LPARAM l) 
	  : Button{btn},
		Event{evn},
		Object{static_cast<WindowHitTest>(w)},
	    Position{reinterpret_cast<::POINTS&>(l).x, reinterpret_cast<::POINTS&>(l).y}
	{}
};

using MouseNonClientDelegate = Delegate<void (MouseNonClientEventArgs)>;
using MouseNonClientEvent = ObservableEvent<MouseNonClientDelegate>;


class HitTestNonClientEventArgs {
public:
	Point  Position;      //!< Screen co-ordinates

public:
	HitTestNonClientEventArgs(::WPARAM, ::LPARAM l)
	  : Position{reinterpret_cast<::POINTS&>(l).x, reinterpret_cast<::POINTS&>(l).y}
	{}
};

using HitTestNonClientDelegate = Delegate<void (HitTestNonClientEventArgs)>;
using HitTestNonClientEvent = ObservableEvent<HitTestNonClientDelegate>;


class NonClientComponentBounds
{
public:
	Rect  Caption,		  //!< Window co-ordinates
	      Title,		  //!< Window co-ordinates
	      SysMenuBtn,	  //!< Window co-ordinates
	      MinimizeBtn,	  //!< Window co-ordinates
	      MaximizeBtn,	  //!< Window co-ordinates
	      Window;		  //!< Screen co-ordinates

public:
	explicit
	NonClientComponentBounds(Rect wnd) noexcept
	  : Window{wnd}
	{
		this->Caption = Rect{0, 0, wnd.width(), SystemMetric::cyCaption};
		this->Caption.inflate(-2*Size{SystemMetric::cxSizeFrame,0});
		this->Caption.translate(2*Point{0,SystemMetric::cySizeFrame});

		// Caption text
		Size const rcIcon {this->Caption.height(), this->Caption.height()};
		this->Title = this->Caption;
		this->Title.Left += rcIcon.Width + (LONG)GuiMeasurement{SystemMetric::cxSizeFrame}*2;
		
		// Caption buttons
		this->SysMenuBtn = Rect{this->Caption.topLeft(), rcIcon};
		--this->SysMenuBtn.Left; --this->SysMenuBtn.Top;
		this->MaximizeBtn = Rect{this->Caption.topRight(),rcIcon} - Point{rcIcon.Width,0};
		this->MinimizeBtn = Rect{this->Caption.topRight(),rcIcon} - Point{2*rcIcon.Width,0};
	}
};


class PaintNonClientEventArgs {
public:
	using CaptionState = ActivateNonClientEventArgs::CaptionState;
public:
	Region                                Area;
	Rect                                  Bounds;
	mutable std::optional<DeviceContext>  Graphics;
	std::optional<Region>                 InvalidArea;
	::HWND                                Window;
	CaptionState                          State;

public:
	PaintNonClientEventArgs(::HWND window, ::WPARAM w, ::LPARAM) 
	  : Window{window}, 
	    State{CaptionState::Unknown}
	{
		if (w > NULLREGION)
			this->InvalidArea = reinterpret_cast<::HRGN>(w);
	}
	
	PaintNonClientEventArgs(ActivateNonClientEventArgs const& args) 
	  : InvalidArea{args.InvalidArea},
		Window{args.Window},
		State{args.State}
	{}
	
	~PaintNonClientEventArgs() noexcept 
	{
		if (this->InvalidArea)
			this->InvalidArea->detach();
	}

public:
	bool 
	beginPaint() {
		auto constinit
		static Flags = DCX_WINDOW|DCX_CACHE|DCX_LOCKWINDOWUPDATE;

		::HDC dc {};
		if (this->InvalidArea) 
			dc = ::GetDCEx(this->Window, Region{*this->InvalidArea}.detach(), Flags|DCX_INTERSECTRGN);
		else 
			dc = ::GetDCEx(this->Window, nullptr, Flags);
		if (!dc)
			return false;

		Rect rcClient;
		Point ptClient;
		::GetClientRect(this->Window, rcClient);
		::ClientToScreen(this->Window, ptClient);
		rcClient += ptClient;
		Rect rcWindow;
		::GetWindowRect(this->Window, rcWindow);
		rcClient -= rcWindow.topLeft();
		rcWindow -= rcWindow.topLeft();
		this->Area = rcWindow;
		this->Area -= Region{rcClient};
		this->Bounds = rcWindow;
		this->Graphics = DeviceContext{dc, this->Window};
		if (this->State == CaptionState::Unknown) {
			::WINDOWINFO info{sizeof(info)};
			::GetWindowInfo(this->Window, &info);
			this->State = info.dwWindowStatus == WS_ACTIVECAPTION ? CaptionState::Active : CaptionState::Inactive;
		}
		return true;
	}

	void 
	endPaint() {
		::ReleaseDC(this->Window, this->Graphics->handle());
		this->Graphics.reset();
	}
};

using PaintNonClientDelegate = Delegate<void (PaintNonClientEventArgs)>;
using PaintNonClientEvent = ObservableEvent<PaintNonClientDelegate>;


class PaintWindowEventArgs {
	::PAINTSTRUCT  Data {};

public:
	std::optional<Rect>           Area;
	mutable 
	std::optional<DeviceContext>  Graphics;
	std::optional<bool>           Erase = false, 
	                              Restore = false, 
	                              Update = false;
	::HWND                        Window;

	PaintWindowEventArgs(::HWND w) : Window(w) 
	{}

	void 
	beginPaint() {
		if (auto dc = ::BeginPaint(this->Window, &this->Data)) {
			this->Graphics = DeviceContext{dc, this->Window};
			this->Area = this->Data.rcPaint;
			this->Erase = this->Data.fErase;
			this->Restore = this->Data.fRestore;
			this->Update = this->Data.fIncUpdate;
		}
	}

	void 
	endPaint() {
		if (::EndPaint(this->Window, &this->Data)) {
			this->Graphics.reset();
			this->Area.reset();
			this->Erase.reset();
			this->Restore.reset();
			this->Update.reset();
		}
	}
};

using PaintWindowDelegate = Delegate<void (PaintWindowEventArgs)>;
using PaintWindowEvent = ObservableEvent<PaintWindowDelegate>;


struct CommandEventArgs {
	static_assert(sizeof(LPARAM) == sizeof(HWND));

	enum EventSource { Menu, Accelerator, Control };
	struct EventData
	{
		uint16_t   Code;
		::HWND     Handle;
	};

	uint16_t					Ident; 
	EventSource					Source;
	std::optional<EventData>	Notification;

	CommandEventArgs(::WPARAM wParam, ::LPARAM ctrl)
	  : Ident(LOWORD(wParam)), 
		Source(ctrl ? Control : HIWORD(wParam) ? Accelerator : Menu)
	{
		if (Source == Control)
			this->Notification = EventData{HIWORD(wParam), reinterpret_cast<HWND>(ctrl)};
	}
};
