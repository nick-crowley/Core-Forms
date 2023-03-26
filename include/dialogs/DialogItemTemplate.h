#pragma once
#include "library/core.Forms.h"
#include "controls/StaticStyle.h"
#include "controls/ButtonStyle.h"
#include "controls/ComboBoxStyle.h"
#include "controls/ListBoxStyle.h"
#include "system/ResourceId.h"
#include "graphics/Rectangle.h"
#include "forms/WindowStyle.h"
#include "forms/ExWindowStyle.h"

namespace core::forms
{
	class DialogItemTemplate 
	{
	public:
		std::optional<uint32_t>      HelpId;
		nstd::bitset<ExWindowStyle>  ExtendedStyle;
		nstd::bitset<WindowStyle>    Style;
		Rect                         Area;
		uint32_t                     Ident;
		std::optional<ResourceId>    Title;
		std::optional<ResourceId>    ClassName;
		std::vector<std::byte>       Data;

	public:
		satisfies(DialogItemTemplate,
			IsRegular,
			NotSortable
		);

	public:
		bool
		subclassed() const {
			return this->ClassName 
			    && !this->ClassName->is_numeric() 
			    && this->ClassName->as_string().starts_with(L"Custom.");
		}
		
	public:
		void
		setOwnerDraw() {
			ResourceId const static CustomStatic(L"Custom.STATIC");
			ResourceId const static CustomButton(L"Custom.BUTTON");
			ResourceId const static CustomListBox(L"Custom.COMBOBOX");
			ResourceId const static CustomComboBox(L"Custom.LISTBOX");

			if (this->ClassName == CustomStatic)
				this->Style = (this->Style & ~StaticStyle::TypeMask) | StaticStyle::OwnerDraw;
			else if (this->ClassName == CustomButton)
				this->Style = (this->Style & ~ButtonStyle::TypeMask) | ButtonStyle::OwnerDraw;
			else if (this->ClassName == CustomComboBox)
				this->Style |= ComboBoxStyle::OwnerDrawFixed | ComboBoxStyle::HasStrings;
			else if (this->ClassName == CustomListBox)
				this->Style |= ListBoxStyle::OwnerDrawFixed | ListBoxStyle::HasStrings;
		}
	};
}