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
#include "forms/WindowClass.h"
#include "forms/WindowInfo.h"
#include "forms/WindowStyle.h"
#include "win/Boolean.h"
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
		UnmanagedWindow(::HWND existingWindow) 
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
		void
		attach(::HWND wnd) {
			this->Handle = wnd;
		}

		DeviceContext
		clientDC() const {
			return DeviceContext{
				SharedDeviceContext{::GetDC(this->handle()), this->handle()}
			};
		}
	
		Rect
		clientRect() const {
			Rect rc;
			::GetClientRect(this->handle(), rc);
			return rc;
		}
	
		Rect
		clientRect(::HWND alternateCoordinateSystem) const {
			Rect rc = this->clientRect();
			auto* pointsArray = reinterpret_cast<::POINT*>(static_cast<::RECT*>(rc));
			::MapWindowPoints(this->handle(), alternateCoordinateSystem, pointsArray, 2);
			return rc;
		}
		
		void
		detach() {
			this->Handle = nullptr;
		}
		
		bool
		enabled() const {
			return ::IsWindowEnabled(this->handle()) != FALSE;
		}
	
		bool
		exists() const {
			return this->handle() && ::IsWindow(this->handle()) != FALSE;
		}

		template <nstd::Enumeration Style = ExWindowStyle>
		nstd::bitset<Style>
		exStyle() const {
			return static_cast<Style>(::GetWindowLongW(this->handle(),GWL_EXSTYLE));
		}
	
		uint16_t
		ident() const {
			return static_cast<uint16_t>(::GetDlgCtrlID(this->handle()));
		}
	
		WindowInfo
		info() const {
			::WINDOWINFO info{sizeof(info)};
			::GetWindowInfo(this->handle(), &info);
			return WindowInfo{info};
		}

		::HWND 
		handle() const {
			return this->Handle;
		}
		
		::HWND
		parent() const {
			return ::GetParent(this->handle());
		}
		
		template <unsigned MessageId>
		::LRESULT
		post(std::optional<::WPARAM> first = std::nullopt, std::optional<::LPARAM> second = std::nullopt) const {
			return ::PostMessageW(this->handle(), MessageId, first.value_or(0), second.value_or(0));
		}
		
		template <unsigned MessageId>
		::LRESULT
		send(std::optional<::WPARAM> first = std::nullopt, std::optional<::LPARAM> second = std::nullopt) const {
			return ::SendMessageW(this->handle(), MessageId, first.value_or(0), second.value_or(0));
		}

		template <nstd::Enumeration Style = WindowStyle>
		nstd::bitset<Style>
		style() const {
			return static_cast<Style>(::GetWindowLongW(this->handle(),GWL_STYLE));
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
		
		WindowClass::const_reference
		virtual wndcls() const {
			Invariant(this->exists());
			static WindowClass cls{this->handle()};
			return cls;
		}
		
		Rect
		wndRect() const {
			Rect rc;
			::GetWindowRect(this->handle(), rc);
			return rc;
		}
	
		Rect
		wndRect(::HWND alternateCoordinateSystem) const {
			Rect rc = this->wndRect();
			::POINT* pointsArray = reinterpret_cast<POINT*>(static_cast<::RECT*>(rc));
			::MapWindowPoints(nullptr, alternateCoordinateSystem, pointsArray, 2);
			return rc;
		}
	
		std::optional<Region>
		wndRgn() const {
			Region rgn;
			if (!::GetWindowRgn(this->handle(), rgn))
				return std::nullopt;
			return rgn;
		}
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		void 
		destroy() {
			if (::DestroyWindow(this->handle()))
				this->Handle = nullptr;
		}
	
		void
		enable(bool enabled) const {
			::EnableWindow(this->handle(), win::Boolean{enabled});
		}
	
		void 
		hide() {
			::ShowWindow(this->handle(), SW_HIDE);
		}
		
		void
		invalidate(bool redraw = false) {
			::InvalidateRect(this->handle(), nullptr, win::Boolean{redraw});
		}

		void
		invalidate(Rect rc, bool redraw = false) {
			::InvalidateRect(this->handle(), rc, win::Boolean{redraw});
		}

		void
		order(::HWND after) {
			::SetWindowPos(this->handle(), after, -1, -1, -1, -1, 
				SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
		}

		void 
		show() {
			::ShowWindow(this->handle(), SW_SHOW);
		}
		
		void 
		show(signed flags) {
			::ShowWindow(this->handle(), flags);
		}
		
		void 
		move(Point pt) {
			::SetWindowPos(this->handle(), nullptr, pt.X, pt.Y, -1, -1, 
				SWP_NOSIZE|SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOACTIVATE);
		}
	
		void 
		reposition(Rect wnd) {
			::SetWindowPos(this->handle(), nullptr, wnd.Left, wnd.Top, wnd.width(), wnd.height(),
				SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOACTIVATE);
		}
	
		void 
		resize(Size sz) {
			::SetWindowPos(this->handle(), nullptr, -1, -1, sz.Width, sz.Height,
				SWP_NOMOVE|SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOACTIVATE);
		}
	
		void
		text(std::wstring_view s)  {
			::SetWindowTextW(this->handle(), s.data());
		}

		void
		update() {
			::UpdateWindow(this->handle());
		}
		
		void
		wndRgn(Region rgn) const {
			::SetWindowRgn(this->handle(), rgn.detach(), win::Boolean{true});
		}
	};
} // namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o