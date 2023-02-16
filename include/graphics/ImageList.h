#pragma once
#include "library/core.Forms.h"
#include "com/Adapter.h"
#include "com/Annotations.h"
#include "com/SharedPtr.h"
#include "graphics/Icon.h"
#include "graphics/SizePoint.h"
#include "graphics/Colours.h"
#include "graphics/Percentage.h"
#include "win/DWord.h"
#pragma comment(lib, "Comctl32.lib")

namespace core::com 
{
	//FIX: CLSID_ImageList isn't registered in practice ::CoCreateInstance() will never succeed
	template <>
	shared_ptr<::IImageList>
	inline make_shared<CLSID_ImageList,::IImageList>(DWORD /*context*/)
	{
		shared_ptr<::IImageList> object;
		ThrowingHResult hr = ::ImageList_CoCreateInstance(CLSID_ImageList, nullptr, IID_IImageList, std::out_ptr(object,adopt));
		return object;
	}
	
	template <>
	shared_ptr<::IImageList2>
	inline make_shared<CLSID_ImageList,::IImageList2>(DWORD /*context*/)
	{
		shared_ptr<::IImageList2> object;
		ThrowingHResult hr = ::ImageList_CoCreateInstance(CLSID_ImageList, nullptr, IID_IImageList2, std::out_ptr(object,adopt));
		return object;
	}
}

namespace core::forms 
{
	class ImageList
	{
		class CoImageListImpl : public com::adapter<::IImageList2>
		{
			using base = com::adapter<interface_type>;

		public:
			method_t<1,::HBITMAP,::HBITMAP,com::out_t<int>> Add;
			method_t<0,com::in_t<::IMAGELISTDRAWPARAMS>>    Draw;
			method_t<1,com::out_t<::COLORREF>>              GetBkColour;
			method_t<1,int,unsigned,com::out_t<::HICON>>    GetIcon;
			method_t<1,com::out_t<int>>                     GetImageCount;
			method_t<1,int,com::out_t<::IMAGEINFO>>         GetImageInfo;
			method_t<1,int,com::out_t<::RECT>>              GetImageRect;
			method_t<2,com::out_t<int>,com::out_t<int>>     GetIconSize;
			method_t<1,int,com::out_t<int>>                 GetOverlayImage;
			method_t<0,int,int,::UINT,int,int>              Initialize;
			method_t<0,int>                                 Remove;
			method_t<1,int,::HICON,com::out_t<int>>         ReplaceIcon;
			method_t<1,::COLORREF,com::out_t<::COLORREF>>   SetBkColour;
			method_t<0,int,int>                             SetIconSize;
			method_t<0,unsigned>                            SetImageCount;
			method_t<0,int,int>                             SetOverlayImage;

		public:
			CoImageListImpl(com::shared_ptr<interface_type> ptr) 
			  : base{ptr},
				Add{this->method<1>(&interface_type::Add)},
				Draw{this->method(&interface_type::Draw)},
				GetBkColour{this->method<1>(&interface_type::GetBkColor)},
				GetIcon{this->method<1>(&interface_type::GetIcon)},
				GetImageCount{this->method<1>(&interface_type::GetImageCount)},
				GetImageInfo{this->method<1>(&interface_type::GetImageInfo)},
				GetImageRect{this->method<1>(&interface_type::GetImageRect)},
				GetIconSize{this->method<2>(&interface_type::GetIconSize)},
				GetOverlayImage{this->method<1>(&interface_type::GetOverlayImage)},
				Initialize{this->method(&interface_type::Initialize)},
				Remove{this->method(&interface_type::Remove)},
				ReplaceIcon{this->method<1>(&interface_type::ReplaceIcon)},
				SetBkColour{this->method<1>(&interface_type::SetBkColor)},
				SetIconSize{this->method(&interface_type::SetIconSize)},
				SetImageCount{this->method(&interface_type::SetImageCount)},
				SetOverlayImage{this->method(&interface_type::SetOverlayImage)}
			{}
		};
	private:
		CoImageListImpl m_impl;

	public:
		ImageList(Size imageSize, unsigned capacity, std::optional<unsigned> growCount = std::nullopt)
		  : m_impl{com::make_shared<CLSID_ImageList,::IImageList2>(CLSCTX_INPROC_SERVER)}
		{
			this->m_impl.Initialize(imageSize.Width, imageSize.Height, ILC_COLOR32|ILC_MASK, capacity, growCount.value_or(0));
		}
		
		explicit
		ImageList(com::shared_ptr<::IImageList2> existing)
		  : m_impl{existing}
		{}

	public:
		void
		appendIcon(Icon const& icon) {
			this->m_impl.ReplaceIcon(-1, icon.handle());
		}

		void
		draw(::HDC dest, unsigned idx, std::variant<Point,Rect> at) {
			::IMAGELISTDRAWPARAMS params = make_params(dest,idx,at);
			params.fStyle |= ILD_TRANSPARENT;
			params.rgbBk = CLR_NONE;
			params.rgbFg = CLR_NONE;

			this->m_impl.Draw(&params);
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
			auto invertedMask = ::CreateCompatibleDC(dest);
			auto memoryBitmap = ::CreateCompatibleBitmap(dest, at.width(), at.height());
			auto prev = ::SelectObject(invertedMask,memoryBitmap);
			{	
				// Invert the mask
				::IMAGELISTDRAWPARAMS params = make_params(invertedMask,idx,at-at.topLeft());
				params.fStyle |= ILD_MASK | ILD_ROP;
				params.dwRop = NOTSRCCOPY;
				params.rgbBk = CLR_NONE;
				params.rgbFg = CLR_NONE;
				this->m_impl.Draw(&params);

				// Colour the mask red
				DeviceContext memDc{invertedMask, nullptr};
				Brush foreground{*blend};
				memDc.setObj(*foreground.handle());
				memDc.copyBitmap(memDc.handle(), at-at.topLeft(), RasterOp::MergeCopy);
				memDc.restore();

				// Draw the mask transparently
				params = make_params(dest,idx,at);
				params.fStyle |= ILD_MASK | ILD_ROP;
				params.dwRop = SRCCOPY;
				params.rgbBk = CLR_NONE;
				params.rgbFg = CLR_NONE;
				this->m_impl.Draw(&params);

				DeviceContext destDc{dest, nullptr};
				destDc.copyBitmap(memDc.handle(), at, RasterOp::SrcPaint);

				// Alphablend the icon onto the mask
				this->draw(dest, idx, at, Percentage{50});
			}
			::SelectObject(invertedMask,prev);
			::DeleteBitmap(memoryBitmap);
			::DeleteDC(invertedMask);
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
			this->m_impl.Draw(&params);
		}
		
		::HIMAGELIST
		handle() const {
			return ::IImageListToHIMAGELIST(this->m_impl);
		}

		forms::Icon
		getIcon(unsigned idx) const {
			return forms::Icon{
				forms::SharedIcon{this->m_impl.GetIcon(idx,ILD_TRANSPARENT), &::DestroyIcon}, 
				this->getIconSize()
			};
		}
		
		forms::Size
		getIconSize() const {
			auto [cx,cy] = this->m_impl.GetIconSize();
			return {cx,cy};
		}
		
		void
		setIcon(unsigned idx, Icon const& icon) {
			this->m_impl.ReplaceIcon(idx, icon.handle());
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