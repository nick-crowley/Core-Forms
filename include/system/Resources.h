#pragma once
#include "library/core.Forms.h"
#include "system/Module.h"

namespace core::forms
{
	class Bitmap;
	class Dialog;
	class Window;
}

namespace core::meta
{
	template <typename T>
	concept AnyResource = std::derived_from<T,::core::forms::Window>
	                   || std::derived_from<T,::core::forms::Bitmap>;
}

namespace core::forms
{
	template <meta::AnyResource R>
	Module 
	inline Resources = Module{nullptr};
}
