/* o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o */ /*!
* @copyright	Copyright (c) 2023, Nick Crowley. All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted
* provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions
*    and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of
*    conditions and the following disclaimer in the documentation and/or other materials provided
*    with the distribution.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
* FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
* WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
* The views and conclusions contained in the software and documentation are those of the author 
* and should not be interpreted as representing official policies, either expressed or implied, of
* the projects which contain it.
*/
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Preprocessor Directives o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#pragma once
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Header Files o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#include "library/core.Forms.h"
#include "com/Adapter.h"
#include "com/Annotations.h"
#include "com/SharedPtr.h"
#include "graphics/Graphics.h"
#include "win/DWord.h"
#pragma comment(lib, "Comctl32.lib")
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com 
{
	template <>
	shared_ptr<::IImageList>
	inline make_shared<::IImageList,CLSID_ImageList>(DWORD /*context*/);
	
	template <>
	shared_ptr<::IImageList2>
	inline make_shared<::IImageList2,CLSID_ImageList>(DWORD /*context*/);
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms 
{
	class ImageList
	{
		class CoImageListImpl : public com::adapter<::IImageList2>
		{
			using base = com::adapter<interface_type>;

		public:
			method_t<1,&interface_type::Add>              add;
			method_t<0,&interface_type::Draw>             draw;
			method_t<1,&interface_type::GetBkColor>       getBkColour;
			method_t<1,&interface_type::GetIcon>          getIcon;
			method_t<1,&interface_type::GetImageCount>    getImageCount;
			method_t<1,&interface_type::GetImageInfo>     getImageInfo;
			method_t<1,&interface_type::GetImageRect>     getImageRect;
			method_t<2,&interface_type::GetIconSize>      getIconSize;
			method_t<1,&interface_type::GetOverlayImage>  getOverlayImage;
			method_t<0,&interface_type::Initialize>       initialize;
			method_t<0,&interface_type::Remove>           remove;
			method_t<1,&interface_type::ReplaceIcon>      replaceIcon;
			method_t<1,&interface_type::SetBkColor>       setBkColour;
			method_t<0,&interface_type::SetIconSize>      setIconSize;
			method_t<0,&interface_type::SetImageCount>    setImageCount;
			method_t<0,&interface_type::SetOverlayImage>  setOverlayImage;

		public:
			CoImageListImpl(com::shared_ptr<interface_type> ptr) 
			  : base{ptr},
				add{this->method<1>(&interface_type::Add)},
				draw{this->method<0>(&interface_type::Draw)},
				getBkColour{this->method<1>(&interface_type::GetBkColor)},
				getIcon{this->method<1>(&interface_type::GetIcon)},
				getImageCount{this->method<1>(&interface_type::GetImageCount)},
				getImageInfo{this->method<1>(&interface_type::GetImageInfo)},
				getImageRect{this->method<1>(&interface_type::GetImageRect)},
				getIconSize{this->method<2>(&interface_type::GetIconSize)},
				getOverlayImage{this->method<1>(&interface_type::GetOverlayImage)},
				initialize{this->method<0>(&interface_type::Initialize)},
				remove{this->method<0>(&interface_type::Remove)},
				replaceIcon{this->method<1>(&interface_type::ReplaceIcon)},
				setBkColour{this->method<1>(&interface_type::SetBkColor)},
				setIconSize{this->method<0>(&interface_type::SetIconSize)},
				setImageCount{this->method<0>(&interface_type::SetImageCount)},
				setOverlayImage{this->method<0>(&interface_type::SetOverlayImage)}
			{}
		};

	private:
		CoImageListImpl m_impl;

	public:
		ImageList(Size imageSize, unsigned capacity, std::optional<unsigned> growCount = std::nullopt)
		  : m_impl{com::make_shared<::IImageList2,CLSID_ImageList>(CLSCTX_INPROC_SERVER)}
		{
			this->m_impl.initialize(imageSize.Width, imageSize.Height, ILC_COLOR32|ILC_MASK, capacity, growCount.value_or(0));
		}
		
		explicit
		ImageList(com::shared_ptr<::IImageList2> existing)
		  : m_impl{existing}
		{}

	public:
		void
		appendIcon(Icon const& icon) {
			this->m_impl.replaceIcon(-1, icon.handle());
		}

		void
		draw(::HDC dest, unsigned idx, std::variant<Point,Rect> at) {
			::IMAGELISTDRAWPARAMS params = make_params(dest,idx,at);
			params.fStyle |= ILD_TRANSPARENT;
			params.rgbBk = CLR_NONE;
			params.rgbFg = CLR_NONE;

			this->m_impl.draw(&params);
		}
		
#if 0
		void
		draw(::HDC dest, unsigned idx, std::variant<Point,Rect> at, std::optional<Colour> blend) {
			::IMAGELISTDRAWPARAMS params = make_params(dest,idx,at);
			params.fStyle |= ILD_TRANSPARENT|ILD_BLEND50;
			params.rgbBk = CLR_NONE;
			params.rgbFg = win::DWord{blend.value_or(static_cast<Colour>(CLR_NONE))};	// BUG: Doesn't operate like MSDN says it should
			
			this->m_impl.Draw(&params);
		}
#endif

		void
		draw(::HDC dest, unsigned idx, Rect at, std::optional<Colour> blend) {
			DeviceContext invertedMask{SharedDeviceContext{::CreateCompatibleDC(dest), weakref}};
			::HBITMAP memoryBitmap = ::CreateCompatibleBitmap(dest, at.width(), at.height());
			invertedMask.setBitmap(memoryBitmap);
			scoped {	
				// Invert the mask
				::IMAGELISTDRAWPARAMS params = make_params(invertedMask.handle(,idx,at-at.topLeft());
				params.fStyle |= ILD_MASK | ILD_ROP;
				params.dwRop = NOTSRCCOPY;
				params.rgbBk = CLR_NONE;
				params.rgbFg = CLR_NONE;
				this->m_impl.draw(&params);

				// Colour the mask red
				Brush foreground{*blend};
				invertedMask.setBrush(foreground);
				invertedMask.copyBitmap(invertedMask.handle(), at-at.topLeft(), RasterOp::MergeCopy);
				invertedMask.restore();

				// Draw the mask transparently
				params = make_params(dest,idx,at);
				params.fStyle |= ILD_MASK | ILD_ROP;
				params.dwRop = SRCCOPY;
				params.rgbBk = CLR_NONE;
				params.rgbFg = CLR_NONE;
				this->m_impl.draw(&params);

				DeviceContext _dest{SharedDeviceContext{dest, weakref}};
				_dest.copyBitmap(invertedMask.handle(), at, RasterOp::SrcPaint);

				// Alphablend the icon onto the mask
				this->draw(dest, idx, at, Percentage{50});
			}
			::DeleteBitmap(memoryBitmap);
		}
		
		void
		draw(::HDC dest, unsigned idx, std::variant<Point,Rect> at, Percentage alpha) {
			::IMAGELISTDRAWPARAMS params = make_params(dest,idx,at);
			params.fStyle |= ILD_TRANSPARENT;
			params.rgbBk = CLR_NONE;
			params.rgbFg = CLR_NONE;
			if (alpha == forms::Percentage{25})
				params.fStyle |= ILD_BLEND25;
			else if (alpha == forms::Percentage{50})
				params.fStyle |= ILD_BLEND50;
			else {
				params.fState = ILS_ALPHA;
				params.Frame = alpha * 255u;
			}
			this->m_impl.draw(&params);
		}
		
		::HIMAGELIST
		handle() const {
			return ::IImageListToHIMAGELIST(this->m_impl);
		}

		forms::Icon
		getIcon(unsigned idx) const {
			return forms::Icon{
				forms::SharedIcon{this->m_impl.getIcon(idx,ILD_TRANSPARENT), &::DestroyIcon}, 
				this->getIconSize()
			};
		}
		
		forms::Size
		getIconSize() const {
			auto [cx,cy] = this->m_impl.getIconSize();
			return {cx,cy};
		}
		
		void
		setIcon(unsigned idx, Icon const& icon) {
			this->m_impl.replaceIcon(idx, icon.handle());
		}

	private:
		::IMAGELISTDRAWPARAMS
		make_params(::HDC dest, unsigned idx, std::variant<Point,Rect> at) {
			::IMAGELISTDRAWPARAMS params{sizeof(params)};
			params.himl = ::IImageListToHIMAGELIST(this->m_impl);
			params.i = idx;
			params.hdcDst = dest;
			if (std::holds_alternative<forms::Rect>(at)) {
				params.fStyle = ILD_SCALE;
				params.x = std::get<Rect>(at).Left;
				params.y = std::get<Rect>(at).Top;
				params.cx = std::get<Rect>(at).width();
				params.cy = std::get<Rect>(at).height();
			}
			else {
				params.x = std::get<Point>(at).X;
				params.y = std::get<Point>(at).Y;
			}
			return params;
		}
	};
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com 
{
	template <>
	shared_ptr<::IImageList>
	inline make_shared<::IImageList,CLSID_ImageList>(DWORD /*context*/)
	{
		// Since @c CLSID_ImageList isn't really registered we must use ::ImageList_CoCreateInstance()
		//  instead of ::CoCreateInstance() 
		shared_ptr<::IImageList> object;
		win::ThrowingHResult hr = ::ImageList_CoCreateInstance(CLSID_ImageList, nullptr, IID_IImageList, std::out_ptr(object,adopt));
		return object;
	}
	
	template <>
	shared_ptr<::IImageList2>
	inline make_shared<::IImageList2,CLSID_ImageList>(DWORD /*context*/)
	{
		shared_ptr<::IImageList2> object;
		win::ThrowingHResult hr = ::ImageList_CoCreateInstance(CLSID_ImageList, nullptr, IID_IImageList2, std::out_ptr(object,adopt));
		return object;
	}
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o