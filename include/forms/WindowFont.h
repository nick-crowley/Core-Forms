#pragma once
#include "library/core.Forms.h"
#include "graphics/Colours.h"
#include "graphics/Font.h"

namespace core::forms
{
	class FormsExport WindowFont {
		std::wstring_view  Name;
		PointSize          Height;
		bool               Bold, Italic, Underline;
		AnyColour          Colour;
	};

}	// namespace core::forms