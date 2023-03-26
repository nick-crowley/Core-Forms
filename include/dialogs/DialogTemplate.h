#pragma once
#include "library/core.Forms.h"
#include "dialogs/DialogItemTemplate.h"
#include "system/ResourceId.h"
#include "graphics/Rectangle.h"
#include "forms/WindowStyle.h"
#include "forms/ExWindowStyle.h"

namespace core::forms
{
	class DialogTemplate 
	{
	public:
		std::optional<uint32_t>         HelpId;
		nstd::bitset<ExWindowStyle>     ExtendedStyle;
		nstd::bitset<WindowStyle>       Style;
		uint16_t                        NumControls;
		Rect                            Area;
		std::optional<ResourceId>       Menu;
		std::optional<ResourceId>       ClassName;
		std::optional<ResourceId>       Title;
		std::optional<ResourceId>       Font;
		std::optional<uint16_t>         Height;
		std::optional<uint16_t>         Weight;
		std::optional<uint8_t>          Italic;
		std::optional<uint8_t>          CharSet;
		std::vector<DialogItemTemplate> Controls;

	public:
		satisfies(DialogTemplate,
			IsRegular,
			NotSortable
		);
	};
}