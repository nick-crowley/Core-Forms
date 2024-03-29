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
#include "graphics/Graphics.h"
#include "forms/AccessibilityFlags.h"
#include "forms/Menu.h"
#include "forms/MenuBar.h"
#include "forms/ScrollBarInfo.h"
#include "forms/WindowClass.h"
#include "forms/WindowInfo.h"
#include "forms/WindowStyle.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	//! @brief	Thin wrapper around an existing window handle
	class FormsExport UnmanagedWindow
	{
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Representation o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		::HWND   Handle = nullptr;
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		explicit
		UnmanagedWindow(::HWND existingWindow) noexcept
			: Handle{existingWindow}
		{}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		satisfies(UnmanagedWindow,
			IsRegular noexcept,
			IsSortable noexcept,
			IsPolymorphic
		);
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		bool
		active() const noexcept {
			return this->handle() && (this->handle() == ::GetActiveWindow());
		}

		void
		attach(::HWND wnd) noexcept {
			this->Handle = wnd;
		}

		DeviceContext
		clientDC() const {
			return DeviceContext{
				SharedDeviceContext{::GetDC(this->handle()), this->handle()}
			};
		}
	
		Point
		clientPoint(Point screenCoords) const noexcept {
			::ScreenToClient(this->handle(), screenCoords);
			return screenCoords;
		}
			
		Rect
		clientRect() const noexcept {
			Rect rc;
			::GetClientRect(this->handle(), rc);
			return rc;
		}
	
		Rect
		clientRect(::HWND alternateCoordinateSystem) const noexcept {
			Rect rc = this->clientRect();
			auto* pointsArray = reinterpret_cast<::POINT*>(static_cast<::RECT*>(rc));
			::MapWindowPoints(this->handle(), alternateCoordinateSystem, pointsArray, 2);
			return rc;
		}
		
		void
		detach() noexcept {
			this->Handle = nullptr;
		}
		
		bool
		enabled() const noexcept {
			return ::IsWindowEnabled(this->handle()) != FALSE;
		}
	
		bool
		exists() const noexcept {
			return this->handle() && ::IsWindow(this->handle()) != FALSE;
		}

		template <nstd::Enumeration Style = ExWindowStyle>
		nstd::bitset<Style>
		exStyle() const noexcept {
			return static_cast<Style>(::GetWindowLongPtr(this->handle(),GWL_EXSTYLE));
		}
	
		uint16_t
		ident() const noexcept {
			return static_cast<uint16_t>(::GetDlgCtrlID(this->handle()));
		}
	
		WindowInfo
		info() const noexcept {
			::WINDOWINFO info{sizeof(info)};
			::GetWindowInfo(this->handle(), &info);
			return WindowInfo{info};
		}

		::HWND 
		handle() const noexcept {
			return this->Handle;
		}
		
		bool
		maximized() const noexcept {
			return ::IsZoomed(this->handle()) != FALSE;
		}
		
		bool
		minimized() const noexcept {
			return ::IsIconic(this->handle()) != FALSE;
		}
		
		std::optional<Menu>
		menu() const {
			::HMENU const m = ::GetMenu(this->handle());
			return nstd::make_optional_if<Menu>(::IsMenu(m), SharedMenu{m, weakref});
		}
		
		std::optional<MenuBar>
		menuBar() const noexcept
		{
			::HMENU const mb = ::GetMenu(this->handle());
			return nstd::make_optional_if<MenuBar>(::IsMenu(mb), SharedMenu{mb, weakref});
		}

		::HWND
		parent() const noexcept {
			return ::GetParent(this->handle());
		}
		
		uint32_t
		pid() const {
			::DWORD pid {};
			if (::DWORD tid = ::GetWindowThreadProcessId(this->handle(), &pid); !tid)
				win::LastError{}.throwAlways("GetWindowThreadProcessId() failed");
			return pid;
		}

		template <unsigned MessageId>
		::LRESULT
		post(std::optional<::WPARAM> first = nullopt, std::optional<::LPARAM> second = nullopt) const noexcept {
			return ::PostMessageW(this->handle(), MessageId, first.value_or(0), second.value_or(0));
		}
		
		Point
		screenPoint(Point clientCoords) const noexcept {
			::ClientToScreen(this->handle(), clientCoords);
			return clientCoords;
		}

		ScrollBarInfo
		scrollBar(ObjectId bar) const noexcept {
			ThrowIfNot(bar, bar == ObjectId::Client || bar == ObjectId::HScroll || bar == ObjectId::VScroll);
			::SCROLLBARINFO info{sizeof(info)};
			::GetScrollBarInfo(this->handle(), std::to_underlying(bar), &info);
			return ScrollBarInfo{info};
		}

		template <unsigned MessageId>
		::LRESULT
		send(std::optional<::WPARAM> first = nullopt, std::optional<::LPARAM> second = nullopt) const noexcept {
			return ::SendMessageW(this->handle(), MessageId, first.value_or(0), second.value_or(0));
		}

		template <nstd::Enumeration Style = WindowStyle>
		nstd::bitset<Style>
		style() const noexcept {
			return static_cast<Style>(::GetWindowLongPtr(this->handle(),GWL_STYLE));
		}
		
		std::optional<Menu>
		systemMenu() const {
			::HMENU const m = ::GetSystemMenu(this->handle(), FALSE);
			return nstd::make_optional_if<Menu>(m != nullptr, SharedMenu{m, weakref});
		}

		std::wstring
		text() const {
			if (std::wstring::size_type const capacity = ::GetWindowTextLengthW(this->handle())+1; capacity == 1) 
				return {};
			else {
				std::wstring buffer(capacity, L'\0');
				if (auto const n = ::GetWindowTextW(this->handle(), buffer.data(), win::DWord{capacity}); n != capacity)
					buffer.erase(n, buffer.npos);
				return buffer;
			}
		}

		uint32_t
		tid() const {
			if (::DWORD tid = ::GetWindowThreadProcessId(this->handle(), nullptr); !tid)
				win::LastError{}.throwAlways("GetWindowThreadProcessId() failed");
			else
				return tid;
		}
		
		bool
		visible() const noexcept {
			return ::IsWindowVisible(this->handle()) != FALSE;
		}
		
		WindowClass::const_reference
		virtual wndcls() const {
			Invariant(this->exists());
			WindowClass const  static cls{this->handle()};
			return cls;
		}
		
		Rect
		wndRect() const noexcept {
			Rect rc;
			::GetWindowRect(this->handle(), rc);
			return rc;
		}
	
		Rect
		wndRect(::HWND alternateCoordinateSystem) const noexcept {
			Rect rc = this->wndRect();
			::POINT* pointsArray = reinterpret_cast<POINT*>(static_cast<::RECT*>(rc));
			::MapWindowPoints(nullptr, alternateCoordinateSystem, pointsArray, 2);
			return rc;
		}
	
		std::optional<Region>
		wndRgn() const noexcept {
			Region rgn;
			if (!::GetWindowRgn(this->handle(), rgn))
				return nullopt;
			return rgn;
		}
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		void
		close() noexcept {
			::CloseWindow(this->handle());
		}

		void 
		destroy() noexcept {
			if (::DestroyWindow(this->handle()))
				this->Handle = nullptr;
		}
	
		void
		enable(bool enabled) const noexcept {
			::EnableWindow(this->handle(), win::Boolean{enabled});
		}
	
		template <nstd::Enumeration ExStyle = ExWindowStyle>
		void
		exStyle(nstd::bitset<ExStyle> bits) noexcept {
			::SetWindowLongPtr(this->handle(), GWL_EXSTYLE, bits.value());
		}
	
		void
		font(::HFONT newFont, bool redraw = false) noexcept {
			SetWindowFont(this->handle(), newFont, win::Boolean{redraw});
		}
	
		void 
		hide() noexcept {
			::ShowWindow(this->handle(), SW_HIDE);
		}
		
		void
		invalidate(bool redraw = false) noexcept {
			::InvalidateRect(this->handle(), nullptr, win::Boolean{redraw});
		}

		void
		invalidate(Rect rc, bool redraw = false) noexcept {
			::InvalidateRect(this->handle(), rc, win::Boolean{redraw});
		}

		void
		maximize() noexcept {
			this->post<WM_SYSCOMMAND>(SC_MAXIMIZE);
		}
		
		void
		minimize() noexcept {
			this->post<WM_SYSCOMMAND>(SC_MINIMIZE);
		}
		
		void 
		move(Point pt) noexcept {
			::SetWindowPos(this->handle(), nullptr, pt.X, pt.Y, -1, -1, 
				SWP_NOSIZE|SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOACTIVATE);
		}

		void
		order(::HWND after) noexcept {
			::SetWindowPos(this->handle(), after, -1, -1, -1, -1, 
				SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
		}
		
		void
		restore() noexcept {
			this->post<WM_SYSCOMMAND>(SC_RESTORE);
		}
		
		void 
		show() noexcept {
			::ShowWindow(this->handle(), SW_SHOW);
		}
		
		void 
		show(signed flags) noexcept {
			::ShowWindow(this->handle(), flags);
		}
	
		void 
		reposition(Rect wnd) noexcept {
			::SetWindowPos(this->handle(), nullptr, wnd.Left, wnd.Top, wnd.width(), wnd.height(),
				SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOACTIVATE);
		}
	
		void 
		resize(Size sz) noexcept {
			::SetWindowPos(this->handle(), nullptr, -1, -1, sz.Width, sz.Height,
				SWP_NOMOVE|SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOACTIVATE);
		}
		
		template <nstd::Enumeration Style = WindowStyle>
		void
		style(nstd::bitset<Style> bits) noexcept {
			::SetWindowLongPtr(this->handle(), GWL_STYLE, bits.value());
		}

		void
		text(std::wstring_view s)  noexcept {
			::SetWindowTextW(this->handle(), s.data());
		}

		void
		update() noexcept {
			::UpdateWindow(this->handle());
		}
		
		void
		wndRgn(Region rgn) const noexcept {
			::SetWindowRgn(this->handle(), rgn.detach(), win::Boolean{true});
		}
	};
} // namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o