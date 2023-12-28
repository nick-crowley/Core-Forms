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
#include "graphics/Cursor.h"
#include "graphics/Icon.h"
#include "graphics/SystemBrush.h"
#include "nstd/Bitset.h"
#include "forms/ClassStyle.h"
#include "win/ResourceId.h"
#include "win/Module.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class WindowClass {
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		using const_reference = WindowClass const&;

		struct CustomStorageSize {
			uint32_t  Class = 0;
			uint32_t  Window = 0;
		};
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		SharedBrush               Background;
		SharedCursor              Cursor;
		CustomStorageSize         Extra;
		SharedIcon                LargeIcon, 
		                          SmallIcon;
		win::ResourceId           Name;
		win::ResourceId           Menu;
		win::SharedModule         Instance;
		nstd::bitset<ClassStyle>  Style;
		::WNDPROC                 WndProc = nullptr;

	protected:
		SharedAtom	              Atom;

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		WindowClass(win::ResourceId name, std::optional<::HMODULE> container = nullopt) 
		  : WindowClass{WindowClass::fromName(name,container)}
		{
		}
		
		WindowClass(::HWND existingWindow) 
		  : WindowClass{WindowClass::fromHandle(existingWindow)}
		{
		}

		WindowClass(::WNDCLASSEXW const& props)
		{
			if (props.hbrBackground) {
				if (auto const sysCol = reinterpret_cast<uintptr_t>(props.hbrBackground); sysCol <= STOCK_LAST)
					this->Background = SystemBrush::get(static_cast<SystemColour>(sysCol-1)).handle();
				else
					this->Background = SharedBrush{props.hbrBackground, weakref};
			}
			this->Cursor = SharedCursor{props.hCursor, weakref};
			this->Extra.Class = props.cbClsExtra;
			this->Extra.Window = props.cbWndExtra;
			this->LargeIcon = SharedIcon{props.hIcon, weakref};
			this->SmallIcon = SharedIcon{props.hIconSm, weakref};
			this->Instance = win::SharedModule{props.hInstance, weakref};
			this->Menu = win::ResourceId::parse(props.lpszMenuName);
			this->Name = win::ResourceId::parse(props.lpszClassName);
			this->Style = static_cast<ClassStyle>(props.style);
			this->WndProc = props.lpfnWndProc;
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		satisfies(WindowClass
			IsSemiRegular,
		);
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		win::ResourceId
		static nameFromHandle(::HWND existingWindow) 
		{
			wchar_t name[96] {};
			if (!::GetClassNameW(existingWindow, name, lengthof(name))) 
				win::LastError{}.throwIfError("Failed to resolve window class");
			return win::ResourceId::parse(name);
		}

	private:
		::WNDCLASSEXW
		static fromHandle(::HWND existingWindow) 
		{
			return WindowClass::fromName(WindowClass::nameFromHandle(existingWindow), GetWindowInstance(existingWindow));
		}

		::WNDCLASSEXW
		static fromName(win::ResourceId name, std::optional<::HMODULE> container = nullopt) 
		{
			::WNDCLASSEXW wndcls{sizeof(::WNDCLASSEXW)};
			if (!::GetClassInfoExW(container.value_or(nullptr), name, &wndcls)) 
				win::LastError{}.throwIfError("Cannot find '{}' window class", to_string(name));
			return wndcls;
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		void 
		registér() {
			::WNDCLASSEXW wndcls{sizeof(::WNDCLASSEXW)};
			wndcls.hbrBackground = this->Background.get();
			wndcls.cbClsExtra = this->Extra.Class;
			wndcls.lpszClassName = this->Name;
			wndcls.hCursor = *this->Cursor;
			wndcls.hIcon = *this->LargeIcon;
			wndcls.hIconSm = *this->SmallIcon;
			wndcls.hInstance = *this->Instance;
			wndcls.lpszMenuName = this->Menu;
			wndcls.style = this->Style.value();
			wndcls.cbWndExtra = this->Extra.Window;
			wndcls.lpfnWndProc = this->WndProc;

			if (::ATOM atom = ::RegisterClassExW(&wndcls); !atom) {
				if (auto lastError = win::LastError{}; lastError != ERROR_CLASS_ALREADY_EXISTS)
					lastError.throwIfError("Failed to register '{}' window class", to_string(this->Name));
			}
			else {
				auto const releaser = [instance = this->Instance](::ATOM _atom) {
					::UnregisterClassA(reinterpret_cast<gsl::czstring>(static_cast<uintptr_t>(_atom)), *instance);
				};
				this->Atom.reset(atom, releaser);
			}
		}
	};
}	// namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o