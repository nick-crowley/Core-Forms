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
		Bitmap
		static loadBitmap(ResourceId name, std::optional<Module> source) 
		{
			::HINSTANCE module = source ? source->handle() : nullptr;
			if (auto const bitmap = (HBITMAP)::LoadImageW(module, name, 
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
		static loadIcon(ResourceId name, std::optional<Module> source) 
		{	
			Size const 
			static dimensions{SystemMetric::cxIcon,SystemMetric::cyIcon};
		
			::HINSTANCE module = source ? source->handle() : nullptr;
			if (auto const icon = (::HICON)::LoadImageW(module, name, 
														IMAGE_ICON, 
														dimensions.Width, dimensions.Height, 
														LR_LOADTRANSPARENT); !icon)
				win::LastError{}.throwAlways();
			else
				return Icon{icon, dimensions};
		}

		std::vector<std::byte> 
		static load_resource(const wchar_t* name, const wchar_t* category, std::optional<Module> source) 
		{
			::HINSTANCE module = source ? source->handle() : nullptr;
			if (::HRSRC resource = ::FindResourceW(module, name /*MAKEINTRESOURCE(1)*/, category /*RT_MANIFEST*/); !resource) {
				win::LastError{}.throwAlways("Failed to find resource '{}'", to_string(name));
			}
			else if (::HGLOBAL block = ::LoadResource(module, resource); !block) {
				win::LastError{}.throwAlways("Failed to load resource '{}'", to_string(name));
			}
			else if (::DWORD size = ::SizeofResource(module, resource); !size) {
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

	public:
		::HINSTANCE 
		handle() const {
			return get_handle<::HINSTANCE>(this->Handle);
		}

		std::vector<std::byte> 
		load_resource(const wchar_t* name, const wchar_t* category) const
		{
			return Module::load_resource(name, category, *this);
		}

		Bitmap
		loadBitmap(ResourceId name) const
		{
			return Module::loadBitmap(name, *this);
		}
	
		Icon
		loadIcon(ResourceId name) const
		{
			return Module::loadIcon(name, *this);
		}

	};
}	// namespace core::forms