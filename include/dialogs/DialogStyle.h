#pragma once
#include "library/core.Forms.h"
#include "windows/WindowStyle.h"

namespace core::forms
{
	enum class DialogStyle {
		AbsAlign		= DS_ABSALIGN         , // 0x01L
		SysModal		= DS_SYSMODAL         , // 0x02L
		LocalEdit		= DS_LOCALEDIT        , // 0x20L		/* Edit items get Local storage. */
		SetFont			= DS_SETFONT          , // 0x40L		/* User specified font for Dlg controls */
		ModalFrame		= DS_MODALFRAME       , // 0x80L		/* Can be combined with WS_CAPTION  */
		NoidleMsg		= DS_NOIDLEMSG        , // 0x100L		/* WM_ENTERIDLE message will not be sent */
		SetForeground	= DS_SETFOREGROUND    , // 0x200L		/* not in win3.1 */
		RaisedLook		= DS_3DLOOK           , // 0x0004L
		FixedSys		= DS_FIXEDSYS         , // 0x0008L
		NoFailCreate	= DS_NOFAILCREATE     , // 0x0010L
		Control			= DS_CONTROL          , // 0x0400L
		Center			= DS_CENTER           , // 0x0800L
		CenterMouse		= DS_CENTERMOUSE      , // 0x1000L
		ContextHelp		= DS_CONTEXTHELP      , // 0x2000L
		ShellFont		= DS_SHELLFONT        , // (DS_SETFONT | DS_FIXEDSYS)
		//UsePixels     = DS_USEPIXELS        , // 0x8000L
	
		None			= 0,
	};

	DialogStyle constexpr inline operator|(DialogStyle a,DialogStyle b) { return detail::enum_bit_or(a,b);  }
	DialogStyle constexpr inline operator&(DialogStyle a,DialogStyle b) { return detail::enum_bit_and(a,b); }
	DialogStyle constexpr inline operator^(DialogStyle a,DialogStyle b) { return detail::enum_bit_xor(a,b); }

	WindowStyle constexpr inline operator|(WindowStyle a,DialogStyle b) { return detail::enum_bit_or(a,b);  }
	WindowStyle constexpr inline operator&(WindowStyle a,DialogStyle b) { return detail::enum_bit_and(a,b); }
	WindowStyle constexpr inline operator^(WindowStyle a,DialogStyle b) { return detail::enum_bit_xor(a,b); }

	DialogStyle constexpr inline operator|(DialogStyle a,WindowStyle b) { return detail::enum_bit_or(a,b);  }
	DialogStyle constexpr inline operator&(DialogStyle a,WindowStyle b) { return detail::enum_bit_and(a,b); }
	DialogStyle constexpr inline operator^(DialogStyle a,WindowStyle b) { return detail::enum_bit_xor(a,b); }
}