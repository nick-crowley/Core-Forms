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
#include "system/SharedHandle.h"
#include "graphics/ObjectFlags.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	enum class PointSize : uint32_t 
	{ 
		Undefined = 0,
		Default = 12,
		OneInch = 72,
	};
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class Font
	{
		SharedFont  Handle;

	public:
		explicit
		Font(SharedFont existingFont) : Handle{std::move(ThrowIfEmpty(existingFont))}
		{
		}

		Font(std::wstring_view name, 
			 int32_t           twipsHeight, 
			 FontWeight        weight = FontWeight::Normal, 
			 bool              italic = false, 
			 bool              underline = false, 
			 FontQuality       quality = FontQuality::AntiAliased, 
			 FontCharSet       charSet = FontCharSet::Default)
		{
			::LOGFONT req {};
			req.lfHeight = twipsHeight;
			req.lfWeight = static_cast<LONG>(weight);
			req.lfItalic = static_cast<BYTE>(italic);
			req.lfUnderline = static_cast<BYTE>(underline);
			req.lfQuality = static_cast<BYTE>(quality);
			req.lfCharSet = static_cast<BYTE>(charSet);
			wcscpy_s(req.lfFaceName, name.data());
			if (auto const font = ::CreateFontIndirectW(&req); !font)
				win::LastError{}.throwAlways();
			else
				this->Handle.reset(font);
		}
		
		Font(::HFONT                           existing, 
			 std::optional<std::wstring_view>  name = std::nullopt,
			 std::optional<int32_t>            twipsHeight = std::nullopt, 
			 std::optional<FontWeight>         weight = std::nullopt, 
			 std::optional<bool>               italic = std::nullopt, 
			 std::optional<bool>               underline = std::nullopt
		) {
			::LOGFONT props {};
			if (!::GetObjectW(existing, sizeof(::LOGFONT), &props))
				win::LastError{}.throwAlways();

			if (name)
				wcscpy_s(props.lfFaceName, name->data());
			if (twipsHeight)
				props.lfHeight = *twipsHeight;
			if (weight)
				props.lfWeight = static_cast<::LONG>(*weight);
			if (italic)
				props.lfItalic = static_cast<::BYTE>(*italic);
			if (underline)
				props.lfUnderline = static_cast<::BYTE>(*underline);
			if (auto const font = ::CreateFontIndirectW(&props); !font)
				win::LastError{}.throwAlways();
			else
				this->Handle.reset(font);
		}
		
	public:
		SharedFont
		handle() const
		{
			return this->Handle;
		}
	};
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms::literals
{
PointSize consteval
	operator""_pt(uint64_t n) noexcept {
		return static_cast<PointSize>(n);
	}
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o