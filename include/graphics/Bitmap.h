#pragma once
#include "formsFramework.h"
#include "graphics/Rectangle.h"
#include "system/SharedHandle.h"

enum class ColourDepth
{
    bpp1 = 1,
    bpp4 = 4,
    bpp8 = 8,
    bpp16 = 16,
    bpp24 = 24,
    bpp32 = 32,
};

ColourDepth constexpr
operator""_bpp(uint64_t n)
{
	return static_cast<ColourDepth>(n);
}

class Bitmap
{
	SharedHandle  Handle;
	Size          Dimensions;
	ColourDepth   Depth;

public:
	Bitmap() noexcept = default;

	explicit
	Bitmap(::HBITMAP bmp, Size dimensions, ColourDepth depth) 
	  : Handle{make_handle(bmp)},
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
			win::throw_exception(::GetLastError());
		else if (::BITMAP info{}; !::GetObject(bitmap, sizeof(info), &info))
			win::throw_exception(::GetLastError());
		else
			return Bitmap{bitmap, {info.bmWidth,info.bmHeight}, static_cast<ColourDepth>(info.bmBitsPixel)};
	}

	ColourDepth
	depth() const
	{
		return this->Depth;
	}

	::HBITMAP
	handle() const
	{
		return get_handle<::HBITMAP>(this->Handle);
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
