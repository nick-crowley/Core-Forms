#pragma once
#include "library/core.Forms.h"
#include "system/Module.h"
#include "system/SharedHandle.h"
#include "system/ResourceId.h"

namespace core::forms
{
	struct WindowClass : public ::WNDCLASSEXW {
		using base = ::WNDCLASSEXW;
	
	private:
		ResourceId	Name;
		ResourceId	Menu;

	protected:
		SharedAtom	Atom;

	public:
		WindowClass() : base({sizeof(::WNDCLASSEXW)}) {
		}

		WindowClass(ResourceId name, std::optional<Module> owner = std::nullopt) 
		  : base({sizeof(::WNDCLASSEXW)}),
			Name(name)
		{
			auto module = owner ? owner->handle() : nullptr;
			if (!::GetClassInfoExW(module, name, static_cast<base*>(this))) {
				win::LastError{}.throwIfError("Cannot find '{}' window class", to_string(name));
			}
		}

		WindowClass(ResourceId name, std::optional<Module> owner, ::WNDPROC proc) 
			: WindowClass() 
		{
			this->style = CS_HREDRAW | CS_VREDRAW | (owner ? NULL : CS_GLOBALCLASS);
			this->lpfnWndProc = proc;
			//this->Class.cbClsExtra = 0;
			//this->Class.cbWndExtra = 0;
			this->hInstance = (owner ? owner->handle() : nullptr);
			//this->Class.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DESKTOPWIZARDAPP));
			this->hCursor = LoadCursor(nullptr, IDC_ARROW);
			this->hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			//this->Class.lpszMenuName = MAKEINTRESOURCEW(IDC_DESKTOPWIZARDAPP);
			//this->Class.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
			this->name(name);
			this->register$();
		}

	public:
		void 
		register$() {	
			if (::ATOM atom = ::RegisterClassExW(static_cast<base*>(this)); !atom) {
				win::LastError{}.throwIfError("Failed to register '{}' window class", to_string(this->lpszClassName));
			}
			else {
				this->Atom.reset(atom, [inst=this->hInstance](::ATOM at) {
					::UnregisterClassA(reinterpret_cast<gsl::czstring>(static_cast<uintptr_t>(at)), inst);
				});
			}
		}

		void 
		name(ResourceId name) {
			this->Name = name;
			this->lpszClassName = this->Name;
		}
	};
}	// namespace core::forms