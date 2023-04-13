#pragma once
#include "library/core.Forms.h"
#include "graphics/Rectangle.h"
#include "system/SharedHandle.h"

namespace core::forms
{
	enum class ColourDepth
	{
		bpp1 = 1,
		bpp4 = 4,
		bpp8 = 8,
		bpp16 = 16,
		bpp24 = 24,
		bpp32 = 32,
	};

	ColourDepth consteval
	operator""_bpp(uint64_t n)
	{
		return static_cast<ColourDepth>(n);
	}

	class Bitmap
	{
		SharedBitmap  Handle;
		Size          Dimensions;
		ColourDepth   Depth;

	public:
		Bitmap() noexcept = default;

		explicit
		Bitmap(::HBITMAP bmp, Size dimensions, ColourDepth depth) 
		  : Handle{bmp},
			Dimensions{dimensions},
			Depth{depth}
		{
		}

		Bitmap
		static loadFromFile(std::wstring_view path) {
		
			if (auto const bitmap = (HBITMAP)::LoadImageW(nullptr, path.data(), 
														  IMAGE_BITMAP, 
														  0, 0, 
														  LR_LOADFROMFILE); !bitmap)
				win::LastError{}.throwAlways();
			else if (::BITMAP info{}; !::GetObject(bitmap, sizeof(info), &info))
				win::LastError{}.throwAlways();
			else
				return Bitmap{bitmap, {info.bmWidth,info.bmHeight}, static_cast<ColourDepth>(info.bmBitsPixel)};
		}

		ColourDepth
		depth() const
		{
			return this->Depth;
		}

		SharedBitmap
		handle() const
		{
			return this->Handle;
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