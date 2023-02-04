#pragma once
#include "library/core.Forms.h"

namespace core::forms
{
	enum class ClassStyle : uint32_t {
		VRedraw         = CS_VREDRAW,           //!< Redraws the entire window if a movement or size adjustment changes the height of the client area
		HRedraw         = CS_HREDRAW,           //!< Redraws the entire window if a movement or size adjustment changes the width of the client area
		DblClks         = CS_DBLCLKS,           //!< Sends a double-click message to the window procedure when the user double-clicks the mouse 
		OwnDC           = CS_OWNDC,             //!< Allocates a unique device context for each window in the class.
		ClassDC         = CS_CLASSDC,           //!< Allocates one device context to be shared by all windows in the class, multiplexed between threads
		ParentDC        = CS_PARENTDC,          //!< Sets the clipping rectangle of the child window to that of the parent window so that the child can draw on the parent
		NoClose         = CS_NOCLOSE,           //!< Disables @c Close on the window menu
		SaveBits        = CS_SAVEBITS,          //!< Redraws area beneath the window upon destruction using a stored bitmap instead of sending windows @c WM_PAINT
		ByteAlignClient = CS_BYTEALIGNCLIENT,   //!< Aligns the window's client area on a byte boundary (in the x direction)
		ByteAlignWindow = CS_BYTEALIGNWINDOW,   //!< Aligns the window on a byte boundary (in the x direction)
		GlobalClass     = CS_GLOBALCLASS,       //!< Indicates that the window class is an application global class.
		Ime             = CS_IME,               //!< Used with Input Method Manager for East Asian localization
		DropShadow      = CS_DROPSHADOW,        //!< Enables the drop shadow effect on a window if @c SPI_SETDROPSHADOW is enabled
		None            = 0,
	};
}	// namespace core::forms

constdata bool core::meta::Settings<core::bitwise_enum, core::forms::ClassStyle> = true;
