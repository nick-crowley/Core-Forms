#pragma once
#include "library/core.Forms.h"
#include "system/SharedHandle.h"
#include "graphics/ObjectFlags.h"

namespace core::forms
{
	class Font
	{
		SharedFont  Handle;

	public:
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
		
		::HFONT
		handle() const
		{
			return *this->Handle;
		}
	};
}