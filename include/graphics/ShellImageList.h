#pragma once
#include "library/core.Forms.h"
#include "graphics/ImageList.h"
#pragma comment(lib, "Shell32.lib")

namespace core::forms
{
	class ShellImageList : public ImageList
	{
		using base = ImageList;

		auto constexpr
		inline static getShellImageList = com::function<1>(&::SHGetImageList);

	public:
		enum IconSize { 
			Small = SHIL_SMALL, 		  //!< Default 16x16
			Large = SHIL_LARGE, 		  //!< Default 32x32
			ExtraLarge = SHIL_EXTRALARGE, //!< Default 48x48
			Jumbo = SHIL_JUMBO 			  //!< Fixed 256x256
		};

	public:
		ShellImageList(IconSize size)
		  : base{com::shared_ptr{adopt, static_cast<::IImageList2*>(getShellImageList(size,IID_IImageList2))}}
		{
		}
	};
}