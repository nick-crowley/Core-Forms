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
	
		::HFONT
		handle() const
		{
			return *this->Handle;
		}
	};
}