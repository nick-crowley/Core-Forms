﻿#include "lookNfeel/Nt6LookNFeel.h"
#include "controls/CommonControls.h"
using namespace core;
using namespace forms;
	
std::shared_ptr<ILookNFeelProvider> const
Nt6LookNFeel::Instance = std::make_shared<Nt6LookNFeel>();

Nt6LookNFeel::Nt6LookNFeel()
{
	using namespace forms::literals;
	this->Fonts.Paragraph = Font{L"Segoe UI", DeviceContext::ScreenDC.measureFont(11_pt)};
	this->Fonts.Heading2 = Font{*this->Fonts.Paragraph.handle(), std::nullopt, DeviceContext::ScreenDC.measureFont(12_pt)};
	this->Fonts.Heading1 = Font{*this->Fonts.Paragraph.handle(), std::nullopt, DeviceContext::ScreenDC.measureFont(14_pt)};
	this->Colours.Button = SystemColour::ButtonFace;
	this->Colours.Control = SystemColour::Window;
    this->Colours.Primary = SystemColour::WindowText;
    this->Colours.Secondary = SystemColour::GrayText;
	this->Colours.Window = SystemColour::Dialog;
}

ILookNFeelProvider::FontDescription
Nt6LookNFeel::default() 
{
	using namespace forms::literals;
	return FontDescription{L"Segoe UI", 11_pt};
}

void
Nt6LookNFeel::draw(GroupBoxControl& ctrl, OwnerDrawEventArgs const& args) 
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"GroupBox #{} must be OwnerDraw", args.Ident};

	// Draw frame
	auto const text = ctrl.text();
	args.Graphics.setFont(ctrl.font());
	auto const textSize = args.Graphics.measureText(text);
	auto const frameRect = args.Item.Area - Border{1, textSize.Height/2, 0, 0};
	Pen const  thickPen{SystemColour::ButtonShadow, 2};
	args.Graphics.setPen(thickPen);
	args.Graphics.setBrush(StockBrush::Hollow);
	args.Graphics.drawRoundRect(frameRect, Size{16,16});

	// Draw text
	auto const frameText = L' ' + text + L' ';
	auto const textOffset = Point{SystemMetric::cxSmallIcon,0};
	args.Graphics.textColour(ctrl.textColour(), ctrl.backColour());
	args.Graphics.drawText(frameText, args.Item.Area + textOffset, DrawTextFlags::Top);

	args.Graphics.restore();
}

void
Nt6LookNFeel::draw(Dialog& dlg, PaintWindowEventArgs const& args)
{
	// Erase background
	args.Graphics->setBrush(SystemBrush::Dialog);
	args.Graphics->fillRect(*args.Area);

	// [SIZE-GRIP]
	if (dlg.style().test(WindowStyle::SizeBox)) {
		Rect rc{args.Area->bottomRight(), Size{SystemMetric::cxHThumb, SystemMetric::cyVThumb}, Rect::FromBottomRight};
		args.Graphics->drawControl(rc, DFC_SCROLL, DFCS_SCROLLSIZEGRIP);
	}

	args.Graphics->restore();
}

void
Nt6LookNFeel::draw(Window& wnd, NonClientPaintEventArgs const& args) 
{
	Invariant(args.Graphics);

	auto const activeCaption = args.State == WindowCaptionState::Active;
	auto const components = NonClientComponentBounds{args.Bounds};

	// Draw frame
	args.Graphics->setBrush(StockBrush::LightGrey);
	args.Graphics->setPen(StockPen::Black);
	Rect const insideFrame = args.Bounds - 2*Border{SystemMetric::cxSizeFrame,SystemMetric::cySizeFrame};
	Region const frameEdges = args.Area - Region{insideFrame};
	args.Graphics->fillRegion(frameEdges);
	args.Graphics->frameRegion(frameEdges, StockBrush::DarkGrey, Size{2,2});
	
	// Draw caption background
	args.Graphics->setBrush(activeCaption ? SystemColour::Highlight : SystemColour::ButtonDkShadow);
	//if (args.InvalidArea)
	//	args.Graphics->fillRegion(*args.InvalidArea);
	//else
		args.Graphics->fillRect(components.Caption);
	
	// Draw caption text
	args.Graphics->setFont(StockFont::SystemFixed);
	args.Graphics->textColour(SystemColour::HighlightText, transparent);
	args.Graphics->drawText(wnd.text(), components.Title, DrawTextFlags::SimpleCentre);

	args.Graphics->restore();
}
