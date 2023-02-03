#pragma once
#include "library/core.Forms.h"
#include "graphics/Rectangle.h"
#include "system/SharedHandle.h"

namespace core::forms
{
	class Icon
	{
		SharedIcon    Handle;
		Size          Dimensions;

	public:
		Icon() noexcept = default;

		explicit
		Icon(::HICON icon, Size dimensions) 
		  : Handle{icon},
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
				win::LastError{}.throwAlways();
			else
				return Icon{icon, dimensions};
		}

		::HICON
		handle() const
		{
			return *this->Handle;
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