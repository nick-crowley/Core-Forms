#pragma once
#include "formsFramework.h"
#include "graphics/Bitmap.h"
#include "graphics/Font.h"
#include "graphics/Icon.h"
#include "graphics/Brush.h"
#include "graphics/Pen.h"
#include "graphics/Colours.h"
#include "graphics/DrawFlags.h"
#include "graphics/ObjectFlags.h"
#include "graphics/Region.h"
#include "support/Percentage.h"
#include "system/SharedHandle.h"
#include "win/ApiHelpers.h"

class DeviceContext 
{
public:
	struct EarlierState
	{
		std::optional<DrawingMode> PrevDrawingMode;
		std::optional<Colour>      PrevBackColour;
		std::optional<Colour>      PrevTextColour;
		std::optional<::HBITMAP>   PrevBitmap;
		std::optional<::HBRUSH>    PrevBrush;
		std::optional<::HFONT>     PrevFont;
		std::optional<::HPEN>      PrevPen;

		/*bool
		empty() const {
			return this->PrevDrawingMode || this->PrevBackColour || this->PrevTextColour
				|| this->PrevBitmap || this->PrevBrush || this->PrevFont || this->PrevPen;
		}*/
	};

public:
	/*DeviceContext
	static ScreenDC;*/

protected:
	::HDC Context; 
	::HWND Window;
	EarlierState Modified;

public:
	DeviceContext(::HDC dc, ::HWND wnd) 
	  : Context{dc}, 
	    Window{wnd}
	{}

public:
	Colour
	static get(SystemColour col)
	{
		return static_cast<Colour>(::GetSysColor(win::DWord{col}));
	}

	template <StockObject Object>
	auto 
	static get()
	{
		if constexpr (Object == StockObject::WhiteBrush
		           || Object == StockObject::LtGreyBrush
		           || Object == StockObject::GreyBrush
		           || Object == StockObject::DkBreyBrush
		           || Object == StockObject::BlackBrush
		           || Object == StockObject::NullBrush
		           || Object == StockObject::DcBrush
		           || Object == StockObject::DcPen)
			return (::HBRUSH)::GetStockObject(static_cast<int>(Object));

		else if constexpr (Object == StockObject::WhitePen
		                || Object == StockObject::BlackPen
		                || Object == StockObject::NullPen)
			return (::HPEN)::GetStockObject(static_cast<int>(Object));

		else if constexpr (Object == StockObject::OemFixedFont
		                || Object == StockObject::AnsiFixedFont
		                || Object == StockObject::AnsiVarFont
		                || Object == StockObject::SystemFont
		                || Object == StockObject::DeviceDefaultFont
		                || Object == StockObject::SystemFixedFont
		                || Object == StockObject::DefaultGuiFont)
			return (::HFONT)::GetStockObject(static_cast<int>(Object));

		else
			return (::HGDIOBJ)nullptr;
	}

public:
	::HDC
	handle() const
	{
		return this->Context;
	}

	::HWND
	window() const
	{
		return this->Window;
	}
	
	void
	copyBitmap(::HDC source, Rect const& dest, RasterOp op = RasterOp::SrcCopy) const
	{
		if (!::BitBlt(this->handle(), 
	                  dest.Left, dest.Top, dest.width(), dest.height(),
	                  source,
	                  0, 0, 
		              win::DWord{op}))
			win::throw_exception(::GetLastError());
	}
	
	void
	copyBitmap(::HDC source, Point const& src, Rect const& dest, RasterOp op = RasterOp::SrcCopy) const
	{
		if (!::BitBlt(this->handle(), 
	                  dest.Left, dest.Top, dest.width(), dest.height(),
	                  source,
	                  src.X, src.Y, 
	                  win::DWord{op}))
			win::throw_exception(::GetLastError());
	}
	
	void
	copyBitmap(::HDC source, Rect const& src, Rect const& dest, RasterOp op = RasterOp::SrcCopy) const
	{
		if (!::StretchBlt(this->handle(), 
	                      dest.Left, dest.Top, dest.width(), dest.height(),
	                      source,
	                      src.Left, src.Top, src.width(), src.height(),
	                      win::DWord{op}))
			win::throw_exception(::GetLastError());
	}
	
	void
	copyBitmap(::HDC source, ColourDepth depth, Rect const& src, Rect const& dest, Percentage opacity = Percentage::Max) const
	{
		::BLENDFUNCTION const blend{
			AC_SRC_OVER, win::Reserved<BYTE>, opacity*255ui8, /*(BYTE)(depth == ColourDepth::bpp32)*/ 0ui8
		};
		
		if (!::AlphaBlend(this->handle(), 
	                      dest.Left, dest.Top, dest.width(), dest.height(),
	                      source,
	                      src.Left, src.Top, src.width(), src.height(),
			              blend))
			win::throw_exception(::GetLastError());
	}

	//! @brief	Draws one or more edges of rectangle
	void 
	drawEdge(Rect const& rc, EdgeFlags edge, BorderFlags flags = BorderFlags::Rect) const
	{
		if (!::DrawEdge(this->handle(), const_cast<Rect&>(rc), win::DWord{edge}, win::DWord{flags}))
			win::throw_exception(::GetLastError());
	}
	
	//!	@brief	Fills a focus rectangle 
	void 
	drawFocus(Rect const& rc) const
	{
		if (!::DrawFocusRect(this->handle(), rc))
			win::throw_exception(::GetLastError());
	}

	//! @brief	Fills a frame rectangle with a custom brush
	void 
	drawFrame(Rect const& rc, SharedHandle const& brush) const
	{
		// Draw frame rectangle with custom brush
		if (!::FrameRect(this->handle(), rc, ::get_handle<::HBRUSH>(brush)))
			win::throw_exception(::GetLastError());
	}
	
	//! @brief	Draws component of built-in control
	void 
	drawFrameCtrl(Rect const& rc, UINT ctrl, UINT state) const
	{
		// Draw control state
		if (!::DrawFrameControl(this->handle(), const_cast<Rect&>(rc), ctrl, state))
			win::throw_exception(::GetLastError());
	}

	//!	@brief	Draw an icon at a position
	void 
	drawIcon(::HICON icon, Point const& pt, Size const& sz) const
	{
		// [ICON] Draw icon
		if (!::DrawIconEx(this->handle(), 
		                  pt.X, pt.Y, 
		                  icon, 
		                  sz.Width, sz.Height, 
		                  0, nullptr, 
		                  DI_IMAGE | DI_MASK))
			win::throw_exception(::GetLastError(), "Unable to draw icon");
	}

	//!	@brief	Draw an icon into a rectangle
	void 
	drawIcon(::HICON icon, Rect const& rc) const
	{
		// [ICON] Draw icon
		if (!::DrawIconEx(this->handle(), 
		                  rc.Left, rc.Top, 
		                  icon,
		                  rc.width(), rc.height(), 
		                  0, nullptr, 
		                  DI_IMAGE | DI_MASK))
			win::throw_exception(::GetLastError(), "Unable to draw icon");
	}
	
	//! @brief	Draws a filled rectangle with the current brush and pen
	void  
	drawRect(Rect const& rc) const
	{
		// Outline source rectangle with current pen
		if (!::Rectangle(this->handle(), rc.Left, rc.Top, rc.Right, rc.Bottom))
			win::throw_exception(::GetLastError());
	}
	//! @brief	Writes text into a rectangle
	//! @return Height of the text in logical units if successful; otherwise 0
	int32_t 
	drawText(std::wstring_view txt, Rect const& rc, DrawTextFlags flags = DrawTextFlags::SimpleLeft) const
	{
		if (int32_t height = ::DrawTextW(this->handle(), 
		                                 txt.data(), static_cast<int>(txt.size()), 
		                                 const_cast<Rect&>(rc), 
		                                 win::DWord{flags}); !height)
			win::throw_exception(::GetLastError(), "Unable to draw text");
		else
			return height;
	}
	
	//!	@brief	Fills a rectangle interior with the current brush
	void
	fillRect(Rect const& rc) const
	{
		::HGDIOBJ curBrush = ::GetCurrentObject(this->handle(), 
		                                        win::DWord{DrawObjectType::Brush});
		
		this->fillRect(rc, (::HBRUSH)curBrush);
	}
	
	//!	@brief	Fills a rectangle interior with a sytem-coloured brush
	void
	fillRect(Rect const& rc, SystemColour col) const
	{
		this->fillRect(rc, ::GetSysColorBrush(static_cast<int>(col)));
	}

	//!	@brief	Fills a rectangle interior with a custom brush
	void
	fillRect(Rect const& rc, ::HBRUSH brush) const
	{
		// Fill target rectangle with custom brush
		if (!::FillRect(this->handle(), rc, brush))
			win::throw_exception(::GetLastError());
	}
	
	//!	@brief	Fills a region interior with current brush
	void
	fillRegion(Region const& rgn) const
	{
		if (!::PaintRgn(this->handle(), const_cast<Region&>(rgn)))
			win::throw_exception(::GetLastError());
	}
	
	//!	@brief	Fills a region interior with a sytem-coloured brush
	void
	fillRegion(Region const& rgn, SystemColour col) const
	{
		if (!::FillRgn(this->handle(), const_cast<Region&>(rgn), ::GetSysColorBrush(static_cast<int>(col))))
			win::throw_exception(::GetLastError());
	}
	
	//!	@brief	Fills a region interior with a custom brush
	void
	fillRegion(Region const& rgn, ::HBRUSH brush) const
	{
		if (!::FillRgn(this->handle(), const_cast<Region&>(rgn), brush))
			win::throw_exception(::GetLastError());
	}
	
	//!	@brief	Outlines a region with a system-coloured brush
	void
	frameRegion(Region const& rgn, SystemColour col, Size const thickness) const
	{
		this->frameRegion(rgn, ::GetSysColorBrush(static_cast<int>(col)), thickness);
	}
	
	//!	@brief	Outlines a region with a custom brush
	void
	frameRegion(Region const& rgn, ::HBRUSH brush, Size const thickness) const
	{
		if (!::FrameRgn(this->handle(), const_cast<Region&>(rgn), brush, thickness.Width, thickness.Height))
			win::throw_exception(::GetLastError());
	}
	
	// @returns		Height in twips
	int32_t 
	getFontHeight(int32_t points) const
	{
		// Query logical pixel height & convert
		if (auto const logPixels = ::GetDeviceCaps(this->handle(), LOGPIXELSY); !logPixels)
			win::throw_exception(::GetLastError());
		else
			return static_cast<int32_t>(-points * logPixels / 72ll); 
	}

	//! @brief	Measure text using the current font
	Size
	measureText(std::wstring_view txt) const
	{
		Size sz;

		// Measure text
		if (!::GetTextExtentPoint32W(this->handle(), txt.data(), static_cast<int>(txt.size()), sz))
			win::throw_exception(::GetLastError());

		return sz;
	}
	
public:
	//!	@brief	Restores the state upon creation
	void 
	restore()
	{
		if (this->Modified.PrevBackColour) 
			this->setBack(*this->Modified.PrevBackColour);
		if (this->Modified.PrevTextColour)
			this->setText(*this->Modified.PrevTextColour);
		if (this->Modified.PrevDrawingMode)
			this->setBack(*this->Modified.PrevDrawingMode);
		if (this->Modified.PrevBitmap)
			this->setObj(*this->Modified.PrevBitmap);
		if (this->Modified.PrevBrush)
			this->setObj(*this->Modified.PrevBrush);
		if (this->Modified.PrevFont)
			this->setObj(*this->Modified.PrevFont);
		if (this->Modified.PrevPen)
			this->setObj(*this->Modified.PrevPen);

		this->Modified = {};
	}

	void
	setObj(StockObject obj)
	{
		switch (obj)
		{
		case StockObject::WhiteBrush:
		case StockObject::LtGreyBrush:
		case StockObject::GreyBrush:
		case StockObject::DkBreyBrush:
		case StockObject::BlackBrush:
		case StockObject::NullBrush:
		case StockObject::DcBrush:
			this->setObj((::HBRUSH)::GetStockObject(static_cast<int>(obj)));
			return;

		case StockObject::DcPen:
		case StockObject::WhitePen:
		case StockObject::BlackPen:
		case StockObject::NullPen:
			this->setObj((::HPEN)::GetStockObject(static_cast<int>(obj)));
			return;

		case StockObject::OemFixedFont:
		case StockObject::AnsiFixedFont:
		case StockObject::AnsiVarFont:
		case StockObject::SystemFont:
		case StockObject::DeviceDefaultFont:
		case StockObject::SystemFixedFont:
		case StockObject::DefaultGuiFont:
			this->setObj((::HFONT)::GetStockObject(static_cast<int>(obj)));
			return;
		}
	}
	
	void
	setObj(::HBITMAP bitmap)
	{
		if (auto prev = (::HBITMAP)::SelectObject(this->handle(), bitmap); !prev) 
			win::throw_exception(::GetLastError());
		else if (!this->Modified.PrevBitmap) 
			this->Modified.PrevBitmap = prev;
	}
	
	void
	setObj(::HBRUSH brush)
	{
		if (auto prev = (::HBRUSH)::SelectObject(this->handle(), brush); !prev) 
			win::throw_exception(::GetLastError());
		else if (!this->Modified.PrevBrush) 
			this->Modified.PrevBrush = prev;
	}
	
	void
	setObj(SystemColour brush)
	{
		this->setObj(::GetSysColorBrush(static_cast<int>(brush)));
	}
	
	void
	setObj(::HFONT font)
	{
		if (auto prev = (::HFONT)::SelectObject(this->handle(), font); !prev) 
			win::throw_exception(::GetLastError());
		else if (!this->Modified.PrevFont) 
			this->Modified.PrevFont = prev;
	}
	
	void
	setObj(::HPEN pen)
	{
		if (auto prev = (::HPEN)::SelectObject(this->handle(), pen); !prev) 
			win::throw_exception(::GetLastError());
		else if (!this->Modified.PrevPen) 
			this->Modified.PrevPen = prev;
	}
	
	//! @brief	Changes the background drawing mode
	void
	setBack(DrawingMode mode)
	{
		// Change background drawing mode
		if (auto const prev = static_cast<DrawingMode>(::SetBkMode(this->handle(), static_cast<int>(mode))); 
			  prev == DrawingMode::Invalid)
			win::throw_exception(::GetLastError());
		else if (!this->Modified.PrevDrawingMode) 
			this->Modified.PrevDrawingMode = prev;
	}

	//! @brief	Changes the background colour (used for text backgrounds and non-solid pen backgrounds)
	//! @return	Previous colour
	void
	setBack(Colour col)
	{
		// Change background colour
		if (auto const prev = static_cast<Colour>(::SetBkColor(this->handle(), win::DWord{col})); 
			  prev == Colour::Invalid)
			win::throw_exception(::GetLastError());
		else if (!this->Modified.PrevBackColour) 
			this->Modified.PrevBackColour = prev;
	}

	void
	setBack(SystemColour col)
	{
		this->setBack(DeviceContext::get(col));
	}

	//! @brief	Changes the current text colour (Used for text foregrounds)
	//! @return Previous colour
	void
	setText(Colour col)
	{
		// Change text colour
		if (auto const prev = static_cast<Colour>(::SetTextColor(this->handle(), win::DWord{col}));
		      prev == Colour::Invalid)
			win::throw_exception(::GetLastError());
		else if (!this->Modified.PrevTextColour) 
			this->Modified.PrevTextColour = prev;
	}
	
	//! @brief	Changes the current text colour (Used for text foregrounds)
	//! @return Previous colour
	void
	setText(SystemColour col)
	{
		this->setText(DeviceContext::get(col));
	}
};
