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
#include "graphics/SystemBrush.h"
#include "nstd/Bitset.h"
#include "forms/ClassStyle.h"
#include "system/ResourceId.h"
#include "system/Module.h"
#include "system/ResourceId.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class WindowClass {
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		SharedBrush               Background;
		uint32_t                  ClsExtra = 0;
		::HCURSOR                 Cursor = nullptr;
		::HICON                   LargeIcon = nullptr, 
		                          SmallIcon = nullptr;
		ResourceId                Name;
		ResourceId                Menu;
		::HMODULE                 Instance = nullptr;
		nstd::bitset<ClassStyle>  Style;
		uint32_t                  WndExtra = 0;
		::WNDPROC                 WndProc = nullptr;

	protected:
		SharedAtom	Atom;

	private:
		::WNDCLASSEXW  Properties {sizeof(::WNDCLASSEXW)};
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		WindowClass() {
		}

		WindowClass(ResourceId name, std::optional<::HMODULE> container = std::nullopt) 
		  : Name{name}, Instance{container.value_or(nullptr)}
		{
			if (!::GetClassInfoExW(this->Instance, name, &this->Properties)) {
				win::LastError{}.throwIfError("Cannot find '{}' window class", to_string(name));
			}
			if (this->Properties.hbrBackground) {
				if (auto const sysCol = reinterpret_cast<uintptr_t>(this->Properties.hbrBackground); sysCol <= STOCK_LAST)
					this->Background = SystemBrush::get(static_cast<SystemColour>(sysCol-1)).handle();
				else
					this->Background = SharedBrush{this->Properties.hbrBackground, weakref};
			}
			this->ClsExtra = this->Properties.cbClsExtra;
			this->Cursor = this->Properties.hCursor;
			this->LargeIcon = this->Properties.hIcon;
			this->SmallIcon = this->Properties.hIconSm;
			this->Menu = ResourceId::parse(this->Properties.lpszMenuName);
			this->Style = static_cast<ClassStyle>(this->Properties.style);
			this->WndExtra = this->Properties.cbWndExtra;
			this->WndProc = this->Properties.lpfnWndProc;
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		void 
		register_() {
			this->Properties.hbrBackground = this->Background.get();
			this->Properties.cbClsExtra = this->ClsExtra;
			this->Properties.lpszClassName = this->Name;
			this->Properties.hCursor = this->Cursor;
			this->Properties.hIcon = this->LargeIcon;
			this->Properties.hIconSm = this->SmallIcon;
			this->Properties.hInstance = this->Instance;
			this->Properties.lpszMenuName = this->Menu;
			this->Properties.style = this->Style.value();
			this->Properties.cbWndExtra = this->WndExtra;
			this->Properties.lpfnWndProc = this->WndProc;

			if (::ATOM atom = ::RegisterClassExW(&this->Properties); !atom) {
				if (auto lastError = win::LastError{}; lastError != ERROR_CLASS_ALREADY_EXISTS)
					lastError.throwIfError("Failed to register '{}' window class", to_string(this->Name));
			}
			else {
				auto const releaser = [instance = this->Instance](::ATOM _atom) {
					::UnregisterClassA(reinterpret_cast<gsl::czstring>(static_cast<uintptr_t>(_atom)), instance);
				};
				this->Atom.reset(atom, releaser);
			}
		}
	};
}	// namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o