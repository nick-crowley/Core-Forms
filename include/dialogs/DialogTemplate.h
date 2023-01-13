#pragma once
#include "formsFramework.h"
#include "system/ResourceId.h"
#include "graphics/Rectangle.h"
#include "windows/WindowStyle.h"
#include "windows/ExWindowStyle.h"

namespace core::forms
{
	struct DialogItemTemplate {
		std::optional<uint32_t>      HelpId;
		EnumBitset<ExWindowStyle>    ExtendedStyle;
		EnumBitset<WindowStyle>      Style;
		Rect                         Area;
		uint32_t                     Ident;
		std::optional<ResourceId> Title;
		std::optional<ResourceId> ClassName;
		std::vector<std::byte>       Data;
	};

	struct DialogTemplate {
		std::optional<uint32_t>         HelpId;
		EnumBitset<ExWindowStyle>       ExtendedStyle;
		EnumBitset<WindowStyle>         Style;
		uint16_t                        NumControls;
		Rect                            Area;
		std::optional<ResourceId>    Menu;
		std::optional<ResourceId>    ClassName;
		std::optional<ResourceId>    Title;
		std::optional<ResourceId>    Font;
		std::optional<uint16_t>         Height;
		std::optional<uint16_t>         Weight;
		std::optional<uint8_t>          Italic;
		std::optional<uint8_t>          CharSet;
		std::vector<DialogItemTemplate> Controls;
	};
}