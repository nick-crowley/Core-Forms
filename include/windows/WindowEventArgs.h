#pragma once
#include "formsFramework.h"
#include "support/ObservableEvent.h"
#include "graphics/Graphics.h"
#include "windows/WindowInfo.h"

namespace core::forms
{
	class Window;

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
		struct ItemData {
			using IdentOrIndex = std::variant<ResourceId,uint32_t>;

			ItemData(::DRAWITEMSTRUCT& data);

			Rect                        Area;
			uintptr_t                   Data;
			IdentOrIndex                Ident;
			EnumBitset<OwnerDrawState>  State;
		};

	public:
		EnumBitset<OwnerDrawAction> Action;
		uint16_t                    Ident;
		ItemData                    Item;
		mutable DeviceContext       Graphics;
		OwnerDrawControl            Type;
		Window*                     Window;

	public:
		OwnerDrawEventArgs(::WPARAM w, ::LPARAM l);

	private:
		OwnerDrawEventArgs(::DRAWITEMSTRUCT& data);
	};

	using OwnerDrawDelegate = Delegate<void (OwnerDrawEventArgs)>;
	using OwnerDrawEvent = ObservableEvent<OwnerDrawDelegate>;


	class OwnerDrawMenuEventArgs 
	{
	public:
		EnumBitset<OwnerDrawAction>   Action;
		OwnerDrawEventArgs::ItemData  Item;
		::HMENU                       Menu;
		mutable DeviceContext         Graphics;

	public:
		OwnerDrawMenuEventArgs(::WPARAM w, ::LPARAM l);

	private:
		OwnerDrawMenuEventArgs(::DRAWITEMSTRUCT&);
	};

	using OwnerDrawMenuDelegate = Delegate<void (OwnerDrawMenuEventArgs)>;
	using OwnerDrawMenuEvent = ObservableEvent<OwnerDrawMenuDelegate>;


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
		std::optional<Region>  InvalidArea;
		WindowCaptionState     State;
		Window*                Window;
		bool                   Repaint;

	public:
		ActivateNonClientEventArgs(forms::Window* window, ::WPARAM w, ::LPARAM l) 
		  : State{static_cast<WindowCaptionState>(w)},
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
		Region                                Area;
		Rect                                  Bounds;
		mutable std::optional<DeviceContext>  Graphics;
		std::optional<Region>                 InvalidArea;
		Window*                               Window;
		WindowCaptionState                    State;

	public:
		PaintNonClientEventArgs(forms::Window* window, ::WPARAM w, ::LPARAM) 
		  : Window{window}, 
			State{WindowCaptionState::Unknown}
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
		beginPaint();

		void 
		endPaint();
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
		Window*                       Window;

		PaintWindowEventArgs(forms::Window* w) : Window(w) 
		{}

		void 
		beginPaint();

		void 
		endPaint();
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
}	// namespace core::forms