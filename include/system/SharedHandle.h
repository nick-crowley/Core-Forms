#pragma once
#include "library/core.Forms.h"
#include "core/SmartHandle.h"

namespace core::forms
{
    namespace detail 
    {
        struct AtomTraits {
            ::ATOM constexpr
            inline static empty = 0;

            auto constexpr
            inline static release = [](::ATOM at) {
                ::UnregisterClassA(reinterpret_cast<gsl::czstring>(static_cast<uintptr_t>(at)), nullptr);
            };
        };
        
        struct DeviceContextTraits {
            ::HDC constexpr
            inline static empty = nullptr;

            auto constexpr
            inline static release = [](::HDC dc) {
                ::ReleaseDC(::WindowFromDC(dc), dc);
            };
        };

        struct GdiObjectTraits {
            ::HGDIOBJ constexpr
            inline static empty = nullptr;

            auto constexpr
            inline static release = &::DeleteObject;
        };
        
        struct WindowTraits {
            ::HWND constexpr
            inline static empty = nullptr;

            auto constexpr
            inline static release = &::DestroyWindow;
        };
    }
	
    //! @brief  Shared @c ::HBITMAP released using @c ::DeleteObject()
    using SharedBitmap = SmartHandle<::HBITMAP, detail::GdiObjectTraits>;
    
    //! @brief  Shared @c ::HBRUSH released using @c ::DeleteObject()
    using SharedBrush = SmartHandle<::HBRUSH, detail::GdiObjectTraits>;
    
    //! @brief  Shared @c ::HFONT released using @c ::DeleteObject()
    using SharedFont = SmartHandle<::HFONT, detail::GdiObjectTraits>;
    
    //! @brief  Shared @c ::HGDIOBJ released using @c ::DeleteObject()
    using SharedObject = SmartHandle<::HGDIOBJ, detail::GdiObjectTraits>;
    
    //! @brief  Shared @c ::HICON released using @c ::DeleteObject()
    using SharedIcon = SmartHandle<::HICON, detail::GdiObjectTraits>;
    
    //! @brief  Shared @c ::HPEN released using @c ::DeleteObject()
    using SharedPen = SmartHandle<::HPEN, detail::GdiObjectTraits>;
    
	//! @brief  Shared @c ::ATOM released using @c ::UnregisterClassA()
    using SharedAtom = SmartHandle<::ATOM, detail::AtomTraits>;
    
    //! @brief  Shared @c ::HWND released using @c ::DestroyWindow()
    using SharedWindow = SmartHandle<::HWND, detail::WindowTraits>;
    
    //! @brief  Shared @c ::HDC released using @c ::ReleaseDC()
    using SharedDeviceContext = SmartHandle<::HDC, detail::DeviceContextTraits>;
    
}	// namespace core::forms