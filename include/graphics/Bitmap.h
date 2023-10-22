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
#include "graphics/Colours.h"
#include "graphics/Rectangle.h"
#include "filesystem/MemoryStream.h"
#include "filesystem/BinaryReader.h"
#include "filesystem/BinaryWriter.h"
#include "system/SharedHandle.h"
#include "win/Module.h"
#include "win/ApiHelpers.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class Bitmap
	{
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		SharedBitmap  Handle;
		Size          Dimensions;
		ColourDepth   Depth;
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		explicit
		Bitmap(SharedBitmap bmp, Size dimensions, ColourDepth depth) 
		  : Handle{std::move(ThrowIfEmpty(bmp))},
			Dimensions{dimensions},
			Depth{depth}
		{
		}
		
		explicit
		Bitmap(SharedBitmap bmp)
		  : Handle{std::move(ThrowIfEmpty(bmp))},
			Depth{ColourDepth::bpp1}
		{
			if (::BITMAP info{}; !::GetObject(*bmp, sizeof(info), &info))
				win::LastError{}.throwAlways();
			else {
				this->Dimensions = {info.bmWidth,info.bmHeight};
				this->Depth = static_cast<ColourDepth>(info.bmBitsPixel);
			}
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		satisfies(Bitmap,
			NotDefaultConstructible,
			IsCopyable noexcept,
			IsMovable noexcept,
			IsEqualityComparable,
			NotSortable
		);
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		Bitmap
		static create(::HDC compatible, Size dimensions) {
			return Bitmap{
				SharedBitmap{::CreateCompatibleBitmap(compatible, dimensions.Width, dimensions.Height)}
			};
		}
		
		Bitmap
		static load(std::wstring_view path, Size dimensions, ColourDepth depth) 
		{
			ThrowIfEmpty(path);

			if (auto const bitmap = (::HBITMAP)::LoadImageW(nullptr, path.data(), 
														    IMAGE_BITMAP, 
														    dimensions.Width, dimensions.Height, 
														    LR_LOADFROMFILE); !bitmap) {
				win::LastError err;
				err.throwAlways("LoadImage({}) failed", to_utf8(path));
			}
			else 
				return Bitmap{SharedBitmap{bitmap}, dimensions, depth};
		}
		
		Bitmap
		static load(win::Module source, win::ResourceId name, Size dimensions, ColourDepth depth) 
		{
			if (auto const bitmap = (::HBITMAP)::LoadImageW(source.handle(), name, 
														    IMAGE_BITMAP, 
														    dimensions.Width, dimensions.Height, 
														    NULL); !bitmap)
				win::LastError{}.throwAlways("LoadImage({}) failed", to_string(name));
			else 
				return Bitmap{SharedBitmap{bitmap}, dimensions, depth};
		}

		Bitmap
		static load(std::span<std::byte const> bytes) {
			::WORD constexpr  static MagicNumber = MAKEWORD('B','M');

			// Read file header
			filesystem::MemoryStream inputStream{bytes};
			filesystem::BinaryReader input{inputStream};
			::BITMAPFILEHEADER const fileHeader = input.read<::BITMAPFILEHEADER>();
			ThrowIfNot(bytes, fileHeader.bfType == MagicNumber);
			ThrowIfNot(bytes, fileHeader.bfSize == bytes.size());

			// Read uncompressed 24/32-bit image header
			::BITMAPINFOHEADER const imageHeader = input.read<::BITMAPINFOHEADER>();
			ThrowIf(bytes, imageHeader.biBitCount < 24);
			//ThrowIfNot(bytes, imageHeader.biCompression == BI_RGB);
			ThrowIfNot(bytes, imageHeader.biSize == sizeof(::BITMAPINFOHEADER)
			               || imageHeader.biSize == sizeof(::BITMAPV4HEADER)
			               || imageHeader.biSize == sizeof(::BITMAPV5HEADER));
			
			// Setup a memory stream for manipulating the pixel-data
			size_t const pixelDataLength = fileHeader.bfSize - fileHeader.bfOffBits;
			size_t const pixelSize = imageHeader.biBitCount / 8;
			size_t const numPixels = pixelDataLength / pixelSize;
			std::vector<std::byte> pixelData{pixelDataLength};
			filesystem::MemoryStream outputStream{std::span{pixelData}};
			filesystem::BinaryWriter output{outputStream};
			
			// [32-bit] Copy the pixel data; pre-multiply alpha
			inputStream.seek(filesystem::Origin::Begin, fileHeader.bfOffBits);
			if (imageHeader.biBitCount == 32) {
				for (auto idx = 0; idx < numPixels; ++idx) {
					auto rgb = input.read<::RGBQUAD>();
					rgb.rgbRed = rgb.rgbRed*rgb.rgbReserved / 0xff;
					rgb.rgbBlue = rgb.rgbBlue*rgb.rgbReserved / 0xff;
					rgb.rgbGreen = rgb.rgbGreen*rgb.rgbReserved / 0xff;
					output.write(rgb);
				}
			}
			// [24-bit] Just copy the pixel data
			else 
				output.write<::RGBTRIPLE>(input.read<::RGBTRIPLE>(numPixels));

			// Create device-dependent bitmap
			auto const depth = static_cast<ColourDepth>(imageHeader.biBitCount);
			Size const dimensions{std::abs(imageHeader.biWidth), std::abs(imageHeader.biHeight)};
			if (SharedBitmap handle{::CreateBitmap(dimensions.Width, dimensions.Height, 1, std::to_underlying(depth), pixelData.data())}; !handle)
				win::LastError{}.throwAlways("CreateBitmap() failed");
			else
				return Bitmap{handle, dimensions, depth};
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
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

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	};
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o