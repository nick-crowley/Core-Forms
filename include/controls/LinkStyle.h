#pragma once
#include "forms/WindowStyle.h"

namespace core::forms
{
	enum class LinkStyle : uint32_t {
		Transparent    = LWS_TRANSPARENT     , //!< Background mix mode is transparent.
		IgnoreReturn   = LWS_IGNORERETURN    , //!< When link has focus, ENTER keystroke is delegated to host dialog box
		NoPrefix       = LWS_NOPREFIX        , //!< [Vista] Ampersand is treated as a literal character rather than the prefix to a shortcut key
		UseVisualStyle = LWS_USEVISUALSTYLE  , //!< [Vista] Link is displayed in the current visual style.
		UseCustomText  = LWS_USECUSTOMTEXT   , //!< [Vista] An NM_CUSTOMTEXT notification is sent when the control is drawn, so that the application can supply text dynamically
		Right          = LWS_RIGHT           , //!< [Vista] The text is right-justified.
	
		None                = 0,
	};
}	// namespace core::forms

namespace core::meta 
{
	metadata bool Settings<bitwise_enum, core::forms::LinkStyle> = true;
	metadata bool Settings<compatible_enum, core::forms::LinkStyle, core::forms::WindowStyle> = true;
	metadata bool Settings<compatible_enum, core::forms::WindowStyle, core::forms::LinkStyle> = true;
}
