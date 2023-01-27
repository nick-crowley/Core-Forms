#pragma once
#include "library/core.Forms.h"
#include "system/ResourceId.h"
#include "graphics/Rectangle.h"
#include "windows/WindowStyle.h"
#include "windows/ExWindowStyle.h"

namespace core::forms
{
	struct DialogItemTemplate 
	{
		std::optional<uint32_t>      HelpId;
		nstd::bitset<ExWindowStyle>  ExtendedStyle;
		nstd::bitset<WindowStyle>    Style;
		Rect                         Area;
		uint32_t                     Ident;
		std::optional<ResourceId>    Title;
		std::optional<ResourceId>    ClassName;
		std::vector<std::byte>       Data;
	};
}