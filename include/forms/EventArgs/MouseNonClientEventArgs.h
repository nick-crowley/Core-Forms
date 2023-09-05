/* o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o */ /*!
* @copyright	Copyright (c) 2023, Nick Crowley. All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted
* provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions
*    and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of
*    conditions and the following disclaimer in the documentation and/or other materials provided
*    with the distribution.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
* FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
* WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
* The views and conclusions contained in the software and documentation are those of the author 
* and should not be interpreted as representing official policies, either expressed or implied, of
* the projects which contain it.
*/
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Preprocessor Directives o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#pragma once
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Header Files o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#include "library/core.Forms.h"
#include "support/ObservableEvent.h"
#include "graphics/SizePoint.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class FormsExport Window;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
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
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class MouseNonClientEventArgs {
	public:
		MouseMessage  Event;
		MouseButton   Button;
		WindowHitTest Object;
		Point         Position;		//!< Screen co-ordinates
	
	public:
		MouseNonClientEventArgs(MouseMessage evn, MouseButton btn, ::WPARAM w, ::LPARAM l) 
		  : Button{btn},
			Event{evn},
			Object{static_cast<WindowHitTest>(w)},
			Position{reinterpret_cast<::POINTS&>(l).x, reinterpret_cast<::POINTS&>(l).y}
		{}
	};

	using MouseNonClientDelegate = Delegate<void (Window&,MouseNonClientEventArgs)>;
	using MouseNonClientEvent = ObservableEvent<MouseNonClientDelegate>;

}	// namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o