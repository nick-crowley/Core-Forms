#pragma once

#include "windows/WindowStyle.h"

enum class ListBoxStyle {
	Notify            = LBS_NOTIFY            ,	// 0x0001L
	Sort              = LBS_SORT              ,	// 0x0002L
	NoRedraw          = LBS_NOREDRAW          ,	// 0x0004L
	MultipleSel       = LBS_MULTIPLESEL       ,	// 0x0008L
	OwnerDrawFixed    = LBS_OWNERDRAWFIXED    ,	// 0x0010L
	OwnerDrawVariable = LBS_OWNERDRAWVARIABLE ,	// 0x0020L
	HasStrings        = LBS_HASSTRINGS        ,	// 0x0040L
	UseTabStops       = LBS_USETABSTOPS       ,	// 0x0080L
	NoIntegralHeight  = LBS_NOINTEGRALHEIGHT  ,	// 0x0100L
	MultiColumn       = LBS_MULTICOLUMN       ,	// 0x0200L
	WantKeyboardInput = LBS_WANTKEYBOARDINPUT ,	// 0x0400L
	ExtendedSel       = LBS_EXTENDEDSEL       ,	// 0x0800L
	DisableNoScroll   = LBS_DISABLENOSCROLL   ,	// 0x1000L
	NoData            = LBS_NODATA            ,	// 0x2000L
	NoSel             = LBS_NOSEL             ,	// 0x4000L
	Combobox          = LBS_COMBOBOX          ,	// 0x8000L
	Standard          = LBS_STANDARD          ,	// (LBS_NOTIFY | LBS_SORT | WS_VSCROLL | WS_BORDER)

	None			  = 0
};

ListBoxStyle constexpr inline operator|(ListBoxStyle a,ListBoxStyle b) { return detail::enum_bit_or(a,b);  }
ListBoxStyle constexpr inline operator&(ListBoxStyle a,ListBoxStyle b) { return detail::enum_bit_and(a,b); }
ListBoxStyle constexpr inline operator^(ListBoxStyle a,ListBoxStyle b) { return detail::enum_bit_xor(a,b); }

WindowStyle constexpr inline operator|(WindowStyle a,ListBoxStyle b) { return detail::enum_bit_or(a,b);  }
WindowStyle constexpr inline operator&(WindowStyle a,ListBoxStyle b) { return detail::enum_bit_and(a,b); }
WindowStyle constexpr inline operator^(WindowStyle a,ListBoxStyle b) { return detail::enum_bit_xor(a,b); }

ListBoxStyle constexpr inline operator|(ListBoxStyle a,WindowStyle b) { return detail::enum_bit_or(a,b);  }
ListBoxStyle constexpr inline operator&(ListBoxStyle a,WindowStyle b) { return detail::enum_bit_and(a,b); }
ListBoxStyle constexpr inline operator^(ListBoxStyle a,WindowStyle b) { return detail::enum_bit_xor(a,b); }
