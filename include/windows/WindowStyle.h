#pragma once
#include "library/core.Forms.h"
#include "support/EnumOperators.h"

namespace core::forms
{
	enum class WindowStyle : uint32_t {
		Overlapped			= WS_OVERLAPPED					, // 0x00000000L
		Popup				= WS_POPUP						, // 0x80000000L
		Child				= WS_CHILD						, // 0x40000000L
		Minimize			= WS_MINIMIZE					, // 0x20000000L
		Visible				= WS_VISIBLE					, // 0x10000000L
		Disabled			= WS_DISABLED					, // 0x08000000L
		ClipSiblings		= WS_CLIPSIBLINGS				, // 0x04000000L
		ClipChildren		= WS_CLIPCHILDREN				, // 0x02000000L
		Maximize			= WS_MAXIMIZE					, // 0x01000000L
		Caption				= WS_CAPTION					, // 0x00C00000L     /* WS_BORDER | WS_DLGFRAME  */
		Border				= WS_BORDER						, // 0x00800000L
		DlgFrame			= WS_DLGFRAME					, // 0x00400000L
		VScroll				= WS_VSCROLL					, // 0x00200000L
		HScroll				= WS_HSCROLL					, // 0x00100000L
		SysMenu				= WS_SYSMENU					, // 0x00080000L
		ThickFrame			= WS_THICKFRAME					, // 0x00040000L
		Group				= WS_GROUP						, // 0x00020000L
		Tabstop				= WS_TABSTOP					, // 0x00010000L
		MinimizeBox			= WS_MINIMIZEBOX				, // 0x00020000L
		MaximizeBox			= WS_MAXIMIZEBOX				, // 0x00010000L

		Tiled				= WS_TILED						, // WS_OVERLAPPED
		Iconic				= WS_ICONIC						, // WS_MINIMIZE
		SizeBox				= WS_SIZEBOX					, // WS_THICKFRAME
		TiledWindow			= WS_TILEDWINDOW				, // WS_OVERLAPPEDWINDOW
		ChildWindow			= Child							, // WS_CHILDWINDOW
		PopupWindow			= Popup|Border|SysMenu			, // WS_POPUPWINDOW
		OverlappedWindow	= Overlapped|Caption|SysMenu|ThickFrame|MinimizeBox|MaximizeBox, // WS_OVERLAPPEDWINDOW

		None				= Overlapped,
	};

	WindowStyle constexpr inline operator~(WindowStyle a) { return detail::enum_bit_not(a); }
	WindowStyle constexpr inline operator|(WindowStyle a,WindowStyle b) { return detail::enum_bit_or(a,b);  }
	WindowStyle constexpr inline operator&(WindowStyle a,WindowStyle b) { return detail::enum_bit_and(a,b); }
	WindowStyle constexpr inline operator^(WindowStyle a,WindowStyle b) { return detail::enum_bit_xor(a,b); }
}	// namespace core::forms