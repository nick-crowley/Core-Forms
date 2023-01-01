#pragma once

#include "windows/WindowStyle.h"

enum class ComboBoxStyle : uint32_t {
	Simple            = CBS_SIMPLE            , // 0x0001L Displays the list box at all times (the current selection is displayed in the edit control)
	Dropdown          = CBS_DROPDOWN          , // 0x0002L Similar to 'Simple' except that the list box is not displayed unless the user selects an icon next to the edit control.
	DropdownList      = CBS_DROPDOWNLIST      , // 0x0003L Similar to ''DropDown' except that the edit control is replaced by a static text item that displays the current selection in the list box.
	OwnerDrawFixed    = CBS_OWNERDRAWFIXED    , // 0x0010L
	OwnerDrawVariable = CBS_OWNERDRAWVARIABLE , // 0x0020L
	AutoHScroll       = CBS_AUTOHSCROLL       , // 0x0040L
	OemConvert        = CBS_OEMCONVERT        , // 0x0080L
	Sort              = CBS_SORT              , // 0x0100L Automatically sorts strings added to the list box
	HasStrings        = CBS_HASSTRINGS        , // 0x0200L The combo box maintains the memory and address for the strings
	NoIntegralHeight  = CBS_NOINTEGRALHEIGHT  , // 0x0400L Specifies that the size of the combo box is exactly the size specified by the application on creation
	DisableNoScroll   = CBS_DISABLENOSCROLL   , // 0x0800L Shows a disabled vertical scroll bar in the list box when the box does not contain enough items to scroll
	Uppercase         = CBS_UPPERCASE         , // 0x2000L
	Lowercase         = CBS_LOWERCASE         , // 0x4000L
	
	None			  = 0x0000
};

ComboBoxStyle constexpr inline operator|(ComboBoxStyle a,ComboBoxStyle b) { return detail::enum_bit_or(a,b);  }
ComboBoxStyle constexpr inline operator&(ComboBoxStyle a,ComboBoxStyle b) { return detail::enum_bit_and(a,b); }
ComboBoxStyle constexpr inline operator^(ComboBoxStyle a,ComboBoxStyle b) { return detail::enum_bit_xor(a,b); }

WindowStyle constexpr inline operator|(WindowStyle a,ComboBoxStyle b) { return detail::enum_bit_or(a,b);  }
WindowStyle constexpr inline operator&(WindowStyle a,ComboBoxStyle b) { return detail::enum_bit_and(a,b); }
WindowStyle constexpr inline operator^(WindowStyle a,ComboBoxStyle b) { return detail::enum_bit_xor(a,b); }

ComboBoxStyle constexpr inline operator|(ComboBoxStyle a,WindowStyle b) { return detail::enum_bit_or(a,b);  }
ComboBoxStyle constexpr inline operator&(ComboBoxStyle a,WindowStyle b) { return detail::enum_bit_and(a,b); }
ComboBoxStyle constexpr inline operator^(ComboBoxStyle a,WindowStyle b) { return detail::enum_bit_xor(a,b); }
