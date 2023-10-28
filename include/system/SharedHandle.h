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
#include "core/SmartHandle.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::detail
{
    template <>
    struct HandleTraits<::HDROP> {
        ::HDROP constexpr
        inline static empty = nullptr;

        auto constexpr
        inline static release = [](::HDROP target) {
            ::DragFinish(target);
        };
    };
    
    template <>
    struct HandleTraits<::HMENU> {
        ::HMENU constexpr
        inline static empty = nullptr;

        auto constexpr
        inline static release = [](::HMENU menu) {
            ::DestroyMenu(menu);
        };
    };
}
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
            inline static destroy = [](::HDC dc) {
                ::DeleteDC(dc);
            };

            auto constexpr
            inline static release = [](::HDC dc) {
                ::ReleaseDC(::WindowFromDC(dc), dc);
            };
        };

        template <typename RawHandle>
        struct GdiObjectTraits {
            RawHandle constexpr
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
    using SharedBitmap = SmartHandle<::HBITMAP, detail::GdiObjectTraits<::HBITMAP>>;
    
    //! @brief  Shared @c ::HBRUSH released using @c ::DeleteObject()
    using SharedBrush = SmartHandle<::HBRUSH, detail::GdiObjectTraits<::HBRUSH>>;
    
    //! @brief  Shared @c ::HDROP released using @c ::DragFinish()
    using SharedDrop = SmartHandle<::HDROP>;
    
    //! @brief  Shared @c ::HFONT released using @c ::DeleteObject()
    using SharedFont = SmartHandle<::HFONT, detail::GdiObjectTraits<::HFONT>>;
    
    //! @brief  Shared @c ::HGDIOBJ released using @c ::DeleteObject()
    using SharedObject = SmartHandle<::HGDIOBJ, detail::GdiObjectTraits<::HGDIOBJ>>;
    
    //! @brief  Shared @c ::HICON released using @c ::DeleteObject()
    using SharedIcon = SmartHandle<::HICON, detail::GdiObjectTraits<::HICON>>;
    
    //! @brief  Shared @c ::HMENU released using @c ::DestroyMenu()
    using SharedMenu = SmartHandle<::HMENU>;
    
    //! @brief  Shared @c ::HPEN released using @c ::DeleteObject()
    using SharedPen = SmartHandle<::HPEN, detail::GdiObjectTraits<::HPEN>>;
    
	//! @brief  Shared @c ::ATOM released using @c ::UnregisterClassA()
    using SharedAtom = SmartHandle<::ATOM, detail::AtomTraits>;
    
    //! @brief  Shared @c ::HWND released using @c ::DestroyWindow()
    using SharedWindow = SmartHandle<::HWND, detail::WindowTraits>;
    
    //! @brief  Shared @c ::HDC released using @c ::ReleaseDC()
    class SharedDeviceContext : public SmartHandle<::HDC, detail::DeviceContextTraits> {
        using base = SmartHandle<::HDC, detail::DeviceContextTraits>;
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    public:
        explicit
        SharedDeviceContext(::HDC dc)
          : base{dc, detail::DeviceContextTraits::release}
        {}

        //! @brief  Construct with window handle (used during release)
        SharedDeviceContext(::HDC dc, ::HWND wnd)
          : base{dc, [wnd](::HDC handle) { ::ReleaseDC(wnd, handle); }}
        {}
        
        SharedDeviceContext(::HDC dc, meta::destroy_t)
          : base{dc, detail::DeviceContextTraits::destroy}
        {}

        SharedDeviceContext(::HDC dc, meta::weakref_t)
          : base{dc, weakref}
        {}
    
        // o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
    public:
        satisfies(SharedDeviceContext,
            IsRegular noexcept,
            NotSortable
        );
    };
    
}	// namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o