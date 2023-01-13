#pragma once
#include "formsFramework.h"
#include "graphics/Rectangle.h"
#include "system/SharedHandle.h"

namespace core::forms
{
	class Icon
	{
		SharedHandle  Handle;
		Size          Dimensions;

	public:
		Icon() noexcept = default;

		explicit
		Icon(::HICON icon, Size dimensions) 
		  : Handle{make_handle(icon)},
			Dimensions{dimensions}
		{
		}

		Icon
		static loadFromFile(std::wstring_view path) {
		
			Size const 
			static dimensions{SystemMetric::cxIcon,SystemMetric::cyIcon};

			if (auto const icon = (::HICON)::LoadImageW(nullptr, path.data(), 
														IMAGE_ICON, 
														dimensions.Width, dimensions.Height, 
														LR_LOADFROMFILE|LR_LOADTRANSPARENT); !icon)
				win::throw_exception(::GetLastError());
			else
				return Icon{icon, dimensions};
		}

		::HICON
		handle() const
		{
			return get_handle<::HICON>(this->Handle);
		}
	
		Rect
		rect() const
		{
			return Rect{{0,0}, this->Dimensions};
		}

		Size
		size() const
		{
			return this->Dimensions;
		}
	};
}