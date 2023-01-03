#pragma once

#include "formsFramework.h"
#include "system/Module.h"
#include "system/SharedHandle.h"
#include "system/ResourceId.h"

struct WindowClass : public ::WNDCLASSEXW {
	using base = ::WNDCLASSEXW;
	
private:
	ResourceId	Name;
	ResourceId	Menu;

protected:
	SharedHandle	Atom;

public:
	WindowClass() : base({sizeof(::WNDCLASSEXW)}) {
	}

	WindowClass(ResourceId name, std::optional<Module> owner = std::nullopt) 
	  : base({sizeof(::WNDCLASSEXW)}),
	    Name(name)
	{
		auto module = owner ? owner->handle() : nullptr;
		if (!::GetClassInfoExW(module, name, static_cast<base*>(this))) {
			win::LastError{}.throw_if_failed(std::format("Cannot find '{}' window class", to_string(name)));
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
			win::LastError{}.throw_if_failed(std::format("Failed to register '{}' window class", to_string(this->lpszClassName)));
		}
		else {
			this->Atom = make_handle(atom, this->hInstance);
		}
	}

	void 
	name(ResourceId name) {
		this->Name = name;
		this->lpszClassName = this->Name;
	}

};