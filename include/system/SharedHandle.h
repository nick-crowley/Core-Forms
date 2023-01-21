#pragma once
#include "library/core.Forms.h"

namespace core::forms
{
	using SharedHandle = std::shared_ptr<void>;
	//using WindowHandle = std::shared_ptr<std::remove_pointer_t<::HWND>>;
	//using ClassAtom = std::shared_ptr<::ATOM>;
	//using ModuleHandle = std::shared_ptr<std::remove_pointer_t<::HINSTANCE>>;

	template <typename RawHandle, typename FancyHandle>
	RawHandle 
	inline get_handle(FancyHandle& h) {
		return (RawHandle)(uintptr_t)h.get();
	}

	SharedHandle 
	inline make_handle(::HBITMAP b) {
		return SharedHandle((void*)(uintptr_t)b, [b](void*) {
			::DeleteObject(b);
		});
	}

	SharedHandle 
	inline make_handle(::HBRUSH b) {
		return SharedHandle((void*)(uintptr_t)b, [b](void*) {
			::DeleteObject(b);
		});
	}

	SharedHandle 
	inline make_handle(::HFONT f) {
		return SharedHandle((void*)(uintptr_t)f, [f](void*) {
			::DeleteObject(f);
		});
	}

	SharedHandle 
	inline make_handle(::HICON i) {
		return SharedHandle((void*)(uintptr_t)i, [i](void*) {
			::DeleteObject(i);
		});
	}

	SharedHandle 
	inline make_handle(::HGDIOBJ obj) {
		return SharedHandle((void*)(uintptr_t)obj, [obj](void*) {
			::DeleteObject(obj);
		});
	}

	SharedHandle 
	inline make_handle(::HPEN p) {
		return SharedHandle((void*)(uintptr_t)p, [p](void*) {
			::DeleteObject(p);
		});
	}

	SharedHandle 
	inline make_handle(::HDC dc, ::HWND wnd) {
		return SharedHandle((void*)(uintptr_t)dc, [dc,wnd](void*) {
			::ReleaseDC(wnd,dc);
		});
	}

	SharedHandle 
	inline make_handle(::HWND w) {
		return SharedHandle((void*)(uintptr_t)w, [w](void*) {
			::DestroyWindow(w);
		});
	}

	SharedHandle 
	inline make_handle(::ATOM a, ::HINSTANCE m) {
		return SharedHandle((void*)(uintptr_t)a, [a, m](void*) {
			::UnregisterClassA((gsl::czstring)(uintptr_t)a, m);
		});
	}

	SharedHandle 
	inline make_handle(::HINSTANCE m) {
		return SharedHandle((void*)(uintptr_t)m, [m](void*) {});
	}
}	// namespace core::forms