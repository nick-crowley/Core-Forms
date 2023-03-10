#pragma once
#include "library/core.Forms.h"
#include "graphics/SystemBrush.h"
#include "nstd/Bitset.h"
#include "windows/ClassStyle.h"
#include "system/ResourceId.h"
#include "system/Module.h"
#include "system/ResourceId.h"

namespace core::forms
{
	class WindowClass {
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