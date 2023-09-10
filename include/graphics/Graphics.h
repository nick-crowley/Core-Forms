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
#include "graphics/Bitmap.h"
#include "graphics/Font.h"
#include "graphics/Icon.h"
#include "graphics/Brush.h"
#include "graphics/Pen.h"
#include "graphics/Colours.h"
#include "graphics/DrawFlags.h"
#include "graphics/ObjectFlags.h"
#include "graphics/Region.h"
#include "graphics/Percentage.h"
#include "graphics/StockBrush.h"
#include "graphics/StockFont.h"
#include "graphics/StockPen.h"
#include "graphics/SystemBrush.h"
#include "system/SharedHandle.h"
#include "win/ApiHelpers.h"
#pragma comment (lib, "Gdi32.lib")
#pragma comment (lib, "UxTheme.lib")
#pragma comment (lib, "Msimg32.lib")
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class DeviceContext 
	{
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
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

		/*DeviceContext
		static ScreenDC;*/

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	protected:
		SharedDeviceContext Handle;
		EarlierState Modified;
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		explicit
		DeviceContext(SharedDeviceContext h) 
		  : Handle{std::move(h)}
		{}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		::HDC
		handle() const
		{
			return this->Handle.get();
		}

		::HWND
		window() const
		{
			return ::WindowFromDC(this->handle());
		}
	
		void
		copyBitmap(::HDC source, Rect const& dest, RasterOp op = RasterOp::SrcCopy) const
		{
			if (!::BitBlt(this->handle(), 
						  dest.Left, dest.Top, dest.width(), dest.height(),
						  source,
						  0, 0, 
						  win::DWord{op}))
				win::LastError{}.throwAlways();
		}
	
		void
		copyBitmap(::HDC source, Rect const& src, Point const& dest, RasterOp op = RasterOp::SrcCopy) const
		{
			if (!::BitBlt(this->handle(), 
						  dest.X, dest.Y, src.width(), src.height(),
						  source,
						  src.Left, src.Top, 
						  win::DWord{op}))
				win::LastError{}.throwAlways();
		}
	
		void
		copyBitmap(::HDC source, Rect const& src, Rect const& dest, RasterOp op = RasterOp::SrcCopy) const
		{
			if (!::StretchBlt(this->handle(), 
							  dest.Left, dest.Top, dest.width(), dest.height(),
							  source,
							  src.Left, src.Top, src.width(), src.height(),
							  win::DWord{op}))
				win::LastError{}.throwAlways();
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
				win::LastError{}.throwAlways();
		}
		
		//! @brief	Draws component of built-in control
		void 
		drawControl(Rect const& rc, UINT ctrl, UINT state) const
		{
			// Draw control state
			if (!::DrawFrameControl(this->handle(), const_cast<Rect&>(rc), ctrl, state))
				win::LastError{}.throwAlways();
		}

		//! @brief	Draws one or more edges of rectangle
		void 
		drawEdge(Rect const& rc, EdgeFlags edge, BorderFlags flags = BorderFlags::Rect) const
		{
			if (!::DrawEdge(this->handle(), const_cast<Rect&>(rc), win::DWord{edge}, win::DWord{flags}))
				win::LastError{}.throwAlways();
		}
	
		//!	@brief	Fills a focus rectangle 
		void 
		drawFocus(Rect const& rc) const
		{
			if (!::DrawFocusRect(this->handle(), rc))
				win::LastError{}.throwAlways();
		}

		//! @brief	Outlines a rectangle using a custom brush
		void 
		drawFrame(Rect const& rc, SharedBrush const& brush) const
		{
			// Draw frame rectangle with custom brush
			if (!::FrameRect(this->handle(), rc, *brush))
				win::LastError{}.throwAlways();
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
				win::LastError{}.throwAlways("Unable to draw icon");
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
				win::LastError{}.throwAlways("Unable to draw icon");
		}
	
		//! @brief	Draws a filled rectangle using the current brush and pen
		void  
		drawRect(Rect const& rc) const
		{
			if (!::Rectangle(this->handle(), rc.Left, rc.Top, rc.Right, rc.Bottom))
				win::LastError{}.throwAlways();
		}
		
		//! @brief	Draws a filled rectangle with rounded corners using the current brush and pen
		void  
		drawRoundRect(Rect const& rc, Size const corners) const
		{
			if (!::RoundRect(this->handle(), rc.Left, rc.Top, rc.Right, rc.Bottom, corners.Width, corners.Height))
				win::LastError{}.throwAlways();
		}

		//! @brief	Writes text into a rectangle
		//! @return Height of the text in logical units if successful; otherwise 0
		int32_t 
		drawText(std::wstring_view txt, Rect const& rc, nstd::bitset<DrawTextFlags> flags = DrawTextFlags::SimpleLeft) const
		{
			ThrowIf(flags, flags.test(DrawTextFlags::ModifyString));
			if (int32_t height = ::DrawTextW(this->handle(), 
											 txt.data(), win::DWord{txt.size()}, 
											 const_cast<Rect&>(rc), 
											 flags.value()); !height)
				win::LastError{}.throwAlways("Unable to draw text");
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
		fillRect(Rect const& rc, SystemColour col) const {
			this->fillRect(rc, SystemBrush::get(col).handle());
		}
		
		//!	@brief	Fills a rectangle interior with a custom brush
		void
		fillRect(Rect const& rc, SharedBrush const& brush) const {
			this->fillRect(rc,*brush);
		}

		//!	@brief	Fills a rectangle interior with a custom brush
		void
		fillRect(Rect const& rc, ::HBRUSH brush) const
		{
			if (!::FillRect(this->handle(), rc, brush))
				win::LastError{}.throwAlways();
		}
	
		//!	@brief	Fills a region interior with current brush
		void
		fillRegion(Region const& rgn) const
		{
			if (!::PaintRgn(this->handle(), const_cast<Region&>(rgn)))
				win::LastError{}.throwAlways();
		}
	
		//!	@brief	Fills a region interior with a sytem-coloured brush
		void
		fillRegion(Region const& rgn, SystemColour col) const
		{
			if (!::FillRgn(this->handle(), const_cast<Region&>(rgn), *SystemBrush::get(col).handle()))
				win::LastError{}.throwAlways();
		}
	
		//!	@brief	Fills a region interior with a custom brush
		void
		fillRegion(Region const& rgn, SharedBrush const& brush) const
		{
			if (!::FillRgn(this->handle(), const_cast<Region&>(rgn), *brush))
				win::LastError{}.throwAlways();
		}
	
		//!	@brief	Outlines a region with a system-coloured brush
		void
		frameRegion(Region const& rgn, SystemColour col, Size const thickness) const {
			this->frameRegion(rgn, SystemBrush::get(col).handle(), thickness);
		}
	
		//!	@brief	Outlines a region with a custom brush
		void
		frameRegion(Region const& rgn, SharedBrush const& brush, Size const thickness) const
		{
			if (!::FrameRgn(this->handle(), const_cast<Region&>(rgn), *brush, thickness.Width, thickness.Height))
				win::LastError{}.throwAlways();
		}
		
		//!	@brief	Outlines a region with a custom brush
		void
		frameRegion(Region const& rgn, Brush const& brush, Size const thickness) const {
			this->frameRegion(rgn, brush.handle(), thickness);
		}

		//! @brief    Calculate font height 
		//! @returns  Height in logical pixels
		int32_t 
		measureFont(PointSize points) const
		{
			// Query logical pixel height & convert
			if (auto const logPixels = ::GetDeviceCaps(this->handle(), LOGPIXELSY); !logPixels)
				win::LastError{}.throwAlways();
			else
				return -static_cast<int32_t>(static_cast<int32_t>(points) * logPixels / 72ll); 
		}

		//! @brief	Measure single line of text (rendered in the currently selected font)
		Size
		measureText(std::wstring_view txt) const
		{
			Size sz;

			// Measure text
			if (!::GetTextExtentPoint32W(this->handle(), txt.data(), win::DWord{txt.size()}, sz))
				win::LastError{}.throwAlways();

			return sz;
		}
	
		//! @brief	Measure multiple lines of text displayed within a rectangle (rendered in the currently selected font)
		Size
		measureText(std::wstring_view txt, Size initial) const
		{
			auto const flags = DrawTextFlags::CalcRect|DrawTextFlags::WordBreak;
			Rect rc{Point::Zero, initial};
			// Measure text
			if (!::DrawTextW(this->handle(), txt.data(), win::DWord{txt.size()}, rc, win::DWord{flags}))
				win::LastError{}.throwAlways();

			return rc.size();
		}
	
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		//!	@brief	Add rectangle to clip region
		void
		exclude(Rect const& rc)
		{
			if (!::ExcludeClipRect(this->handle(), rc.Left, rc.Top, rc.Right, rc.Bottom))
				win::LastError{}.throwAlways();
		}

		//!	@brief	Restores the state upon creation
		void 
		restore()
		{
			if (this->Modified.PrevBackColour) 
				this->backColour(*this->Modified.PrevBackColour);
			if (this->Modified.PrevTextColour)
				this->textColour(*this->Modified.PrevTextColour);
			if (this->Modified.PrevDrawingMode)
				this->backMode(*this->Modified.PrevDrawingMode);
			if (this->Modified.PrevBitmap)
				this->setBitmap(*this->Modified.PrevBitmap);
			if (this->Modified.PrevBrush)
				this->setBrush(*this->Modified.PrevBrush);
			if (this->Modified.PrevFont)
				this->setFont(*this->Modified.PrevFont);
			if (this->Modified.PrevPen)
				this->setPen(*this->Modified.PrevPen);

			this->Modified = {};
		}

		void
		setBitmap(::HBITMAP newBitmap)
		{
			if (auto prev = reinterpret_cast<::HBITMAP>(::SelectObject(this->handle(), newBitmap)); !prev) 
				win::LastError{}.throwAlways();
			else if (!this->Modified.PrevBitmap) 
				this->Modified.PrevBitmap = prev;
		}
		
		void
		setBitmap(SharedBitmap const& newBitmap) {
			this->setBitmap(*newBitmap);
		}

		void
		setBitmap(Bitmap const& newBitmap) {
			this->setBitmap(*newBitmap.handle());
		}
		
		void
		setBrush(::HBRUSH newBrush)
		{
			if (auto prev = reinterpret_cast<::HBRUSH>(::SelectObject(this->handle(), newBrush)); !prev) 
				win::LastError{}.throwAlways();
			else if (!this->Modified.PrevBrush) 
				this->Modified.PrevBrush = prev;
		}
		
		void
		setBrush(SharedBrush newBrush) {
			this->setBrush(*newBrush);
		}
		
		void
		setBrush(StockObject stockBrush) {
			this->setBrush(*StockBrush::get(stockBrush).handle());
		}
		
		void
		setBrush(SystemColour newColour) {
			this->setBrush(*SystemBrush::get(newColour).handle());
		}
		
		void
		setBrush(Colour col) {
			this->setBrush(StockObject::InternalBrush);
			if (auto const prev = (Colour)::SetDCBrushColor(this->handle(), static_cast<::COLORREF>(col)); prev == Colour::Invalid)
				win::LastError{}.throwAlways();
		}
		
		void
		setBrush(AnyColour newColour) {
			if (std::holds_alternative<SystemColour>(newColour))
				this->setBrush(std::get<SystemColour>(newColour));
			else if (std::holds_alternative<Colour>(newColour))
				this->setBrush(std::get<Colour>(newColour));
			else
				this->setBrush(StockBrush::Hollow);
		}
		
		void
		setBrush(Brush const& b) {
			this->setBrush(b.handle());
		}
		
		void
		setFont(::HFONT newFont)
		{
			if (auto prev = reinterpret_cast<::HFONT>(::SelectObject(this->handle(), newFont)); !prev) 
				win::LastError{}.throwAlways();
			else if (!this->Modified.PrevFont) 
				this->Modified.PrevFont = prev;
		}
		
		void
		setFont(SharedFont newFont) {
			this->setFont(*newFont);
		}
		
		void
		setFont(StockObject stockFont) {
			this->setFont(*StockFont::get(stockFont).handle());
		}
		
		void
		setFont(Font const& newFont) {
			this->setFont(*newFont.handle());
		}
		
		void
		setPen(::HPEN newPen)
		{
			if (auto prev = reinterpret_cast<::HPEN>(::SelectObject(this->handle(), newPen)); !prev) 
				win::LastError{}.throwAlways();
			else if (!this->Modified.PrevPen) 
				this->Modified.PrevPen = prev;
		}
		
		void
		setPen(SharedPen newPen) {
			this->setPen(*newPen);
		}
		
		void
		setPen(StockObject stockPen) {
			this->setPen(*StockPen::get(stockPen).handle());
		}
		
		void
		setPen(Colour newcolour) {
			this->setPen(StockObject::InternalPen);
			if (auto const prev = (Colour)::SetDCPenColor(this->handle(), static_cast<::COLORREF>(newcolour)); prev == Colour::Invalid)
				win::LastError{}.throwAlways();
		}
		
		void
		setPen(Pen const& newPen) {
			this->setPen(*newPen.handle());
		}
		
		//! @brief	Changes the background drawing mode
		void
		backColour(meta::transparent_t) {
			this->backMode(DrawingMode::Transparent);
		}

		//! @brief	Changes the background drawing mode
		void
		backColour(meta::opaque_t) {
			this->backMode(DrawingMode::Opaque);
		}

		//! @brief	Changes the background colour (used for text backgrounds and non-solid pen backgrounds)
		void
		backColour(Colour newcolour)
		{
			auto const prev = static_cast<Colour>(::SetBkColor(this->handle(), win::DWord{newcolour}));
			if (prev == Colour::Invalid)
				win::LastError{}.throwAlways();
			else if (!this->Modified.PrevBackColour) 
				this->Modified.PrevBackColour = prev;

			this->backMode(DrawingMode::Opaque);
		}

		void
		backColour(SystemColour newcolour) {
			this->backColour(to_colour(newcolour));
		}

		//! @brief	Sets the text colour and transparent background
		//! @throws	Text colour is transparent
		void
		textColour(AnyColour foreground)
		{
			ThrowIf(foreground, std::holds_alternative<meta::transparent_t>(foreground));

			auto const* colour = std::get_if<Colour>(&foreground);
			this->setTextColour(colour ? *colour : to_colour(std::get<SystemColour>(foreground)));
			this->backMode(DrawingMode::Transparent);
		}
		
		//! @brief	Set the text colour
		//! @throws	Text colour is transparent
		void
		textColour(AnyColour foreground, AnyColour background) 
		{
			ThrowIf(foreground, std::holds_alternative<meta::transparent_t>(foreground));

			auto const* colour = std::get_if<Colour>(&foreground);
			this->setTextColour(colour ? *colour : to_colour(std::get<SystemColour>(foreground)));

			if (std::holds_alternative<meta::transparent_t>(background))
				this->backMode(DrawingMode::Transparent);
			else {
				colour = std::get_if<Colour>(&background);
				this->backColour(colour ? *colour : to_colour(std::get<SystemColour>(background)));
				this->backMode(DrawingMode::Opaque);
			}
		}

	protected:
		//! @brief	Changes the background drawing mode
		void
		backMode(DrawingMode mode)
		{
			auto const prev = static_cast<DrawingMode>(::SetBkMode(this->handle(), win::DWord{mode}));
			if (prev == DrawingMode::Invalid)
				win::LastError{}.throwAlways();
			else if (!this->Modified.PrevDrawingMode) 
				this->Modified.PrevDrawingMode = prev;
		}
		
		//! @brief	Changes the current text colour
		void
		setTextColour(Colour col)
		{
			auto const prev = static_cast<Colour>(::SetTextColor(this->handle(), win::DWord{col})); 
			if (prev == Colour::Invalid)
				win::LastError{}.throwAlways();
			else if (!this->Modified.PrevTextColour) 
				this->Modified.PrevTextColour = prev;
		}
	};
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o