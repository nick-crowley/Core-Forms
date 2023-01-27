#pragma once
#include "library/core.Forms.h"
#include "system/ResourceId.h"
#include "system/SharedHandle.h"
#include "graphics/Bitmap.h"
#include "graphics/Icon.h"

namespace core::forms
{
	class Module 
	{
		SharedHandle	Handle;

	public:
		explicit
		Module(SharedHandle m) : Handle(m) {
		}
	
	public:
		::HINSTANCE 
		handle() const {
			return get_handle<::HINSTANCE>(this->Handle);
		}

		Bitmap
		loadBitmap(ResourceId name) const
		{
			if (auto const bitmap = (::HBITMAP)::LoadImageW(this->handle(), name, 
														    IMAGE_BITMAP, 
														    0, 0, 
														    NULL); !bitmap)
				win::LastError{}.throwAlways();
			else if (::BITMAP info{}; !::GetObject(bitmap, sizeof(info), &info))
				win::LastError{}.throwAlways();
			else
				return Bitmap{bitmap, {info.bmWidth,info.bmHeight}, static_cast<ColourDepth>(info.bmBitsPixel)};
		}
	
		Icon
		loadIcon(ResourceId name) const
		{	
			Size const 
			static dimensions{SystemMetric::cxIcon,SystemMetric::cyIcon};
		
			if (auto const icon = (::HICON)::LoadImageW(this->handle(), name, 
														IMAGE_ICON, 
														dimensions.Width, dimensions.Height, 
														LR_LOADTRANSPARENT); !icon)
				win::LastError{}.throwAlways();
			else
				return Icon{icon, dimensions};
		}

		std::vector<std::byte> 
		loadResource(gsl::cwzstring name, gsl::cwzstring category) const
		{
			if (::HRSRC resource = ::FindResourceW(this->handle(), name, category); !resource) {
				win::LastError{}.throwAlways("Failed to find resource '{}'", to_string(name));
			}
			else if (::HGLOBAL block = ::LoadResource(this->handle(), resource); !block) {
				win::LastError{}.throwAlways("Failed to load resource '{}'", to_string(name));
			}
			else if (::DWORD size = ::SizeofResource(this->handle(), resource); !size) {
				win::LastError{}.throwAlways("Failed to measure resource '{}'", to_string(name));
			}
			else if (auto data = (const std::byte*)::LockResource(block); !data) {
				win::LastError{}.throwAlways("Failed to lock resource '{}'", to_string(name));
			}
			else {
				final_act(=) { UnlockResource(block); };
				return { data, data + size };
			}
		}
	};

	Module const
	inline static ProcessModule {make_handle(::GetModuleHandleW(nullptr))};
	
	Module const
	inline static SystemResource {nullptr};

}	// namespace core::forms