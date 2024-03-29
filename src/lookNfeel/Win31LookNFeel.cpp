﻿#include "lookNfeel/Win31LookNFeel.h"
#include "controls/CommonControls.h"
#include "graphics/FontBuilder.h"
using namespace core;
using namespace forms;

std::shared_ptr<ILookNFeelProvider> const
Win31LookNFeel::Instance = std::make_shared<Win31LookNFeel>();

Win31LookNFeel::Win31LookNFeel()
{
	using namespace forms::literals;
	this->Fonts.Paragraph = this->makeDefault();
	this->Fonts.Heading3 = FontBuilder{}.fromExisting(this->Fonts.Paragraph).withSize(12_pt);
	this->Fonts.Heading2 = FontBuilder{}.fromExisting(this->Fonts.Paragraph).withSize(16_pt);
	this->Fonts.Heading1 = FontBuilder{}.fromExisting(this->Fonts.Paragraph).withSize(24_pt);
	this->Colours.Button = SystemColour::ButtonFace;
    this->Colours.Caption = {SystemColour::Highlight, SystemColour::ButtonDkShadow};
	this->Colours.Control = Colour::White;
    this->Colours.Primary = Colour::Black;
    this->Colours.Highlight = SystemColour::Highlight;
    this->Colours.Secondary = SystemColour::GrayText;
    this->Colours.Tertiary = SystemColour::GrayText;
	this->Colours.Window = Colour::White;
}

Win31LookNFeel::Win31LookNFeel(SharedColourScheme alternateColours, SharedWindowFrame windowFrame)
  : base{alternateColours,windowFrame}
{
}

bool
Win31LookNFeel::customFrame() const {
	return true;
}

NonClientLayout
Win31LookNFeel::nonClientArea(Coords results, nstd::bitset<WindowStyle> style, Rect wnd) const 
{
	ThrowIf(results, results == Coords::Client);

	// Base non-client area upon the default
	NonClientLayout bounds = base::nonClientArea(results, style, wnd);

	// Shift Minimize/Maximize buttons right by 1 button
	bounds.MinimizeBtn = bounds.MaximizeBtn;
	bounds.MaximizeBtn = bounds.CloseBtn;
	bounds.CloseBtn = Rect::Empty;

	return bounds;
}

void
Win31LookNFeel::draw(GroupBoxControl& ctrl, OwnerDrawEventArgs& args) const 
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"GroupBox #{} must be OwnerDraw", args.Ident};

	// Draw frame
	auto const text = ctrl.text();
	args.Graphics.setFont(ctrl.font());
	auto const textSize = args.Graphics.measureText(text);
	auto const frameRect = args.Item.Area - Border{1, textSize.Height/2, 0, 0};
	Pen const  thickPen{Colour::Black, 2};
	args.Graphics.setPen(thickPen);
	args.Graphics.setBrush(StockBrush::Hollow);
	args.Graphics.drawRect(frameRect);

	// Draw text
	auto const frameText = L' ' + text + L' ';
	auto const textOffset = Point{SystemMetric::cxSmallIcon,0};
	args.Graphics.textColour(ctrl.textColour(), ctrl.backColour());
	args.Graphics.drawText(frameText, args.Item.Area + textOffset, DrawTextFlags::Top);

	args.Graphics.restore();
}

void
Win31LookNFeel::draw(Dialog& dlg, PaintWindowEventArgs& args) const
{
	// Erase background
	args.Graphics.setBrush(this->window());
	args.Graphics.fillRect(args.Area);

	args.Graphics.restore();
}

Response
Win31LookNFeel::draw(Dialog& dlg, NonClientPaintEventArgs& args) const 
{
	ThrowIfNot(args, args.Graphics == nullopt);
	args.beginPaint();

	auto const activeCaption = args.CaptionState == WindowCaptionState::Active;
	auto const components = this->nonClientArea(Coords::Window, args.Window.style(), args.Bounds);
	auto const style = dlg.style();
	auto const& caption = dlg.caption();

	// Draw window frame
	args.Graphics->setBrush(StockBrush::LightGrey);
	args.Graphics->setPen(StockPen::Black);
	Region const frameEdges = args.Area - Region{args.Client};
	args.Graphics->fillRegion(frameEdges);
	args.Graphics->frameRegion(frameEdges, StockBrush::DarkGrey, Size{2,2});
	
	// Draw caption background
	args.Graphics->setBrush(activeCaption ? this->caption().Active : this->caption().Inactive);
	//if (args.InvalidArea)
	//	args.Graphics->fillRegion(*args.InvalidArea);
	//else
		args.Graphics->fillRect(components.Caption);
	
	// Draw caption text
	args.Graphics->setFont(StockObject::SystemFixedFont);
	args.Graphics->textColour(SystemColour::HighlightText);
	args.Graphics->backColour(transparent);
	args.Graphics->drawText(dlg.text(), components.Title, DrawTextFlags::SimpleCentre);

	// Draw SysMenu button
	args.Graphics->setPen(StockPen::Black);
	args.Graphics->setBrush(SystemBrush::Dialog);
	args.Graphics->drawRect(components.SysMenuBtn);
	args.Graphics->setPen(StockObject::BlackPen);
	args.Graphics->setBrush(SystemBrush::Window);
	auto const sysMenuBorder = Border{Size{components.SysMenuBtn.width()/6, components.SysMenuBtn.height()/2 - 2}};
	auto const sysMenuBtn = Rect{components.Caption.topLeft(), components.SysMenuBtn.size()} - sysMenuBorder;
	args.Graphics->drawRect(sysMenuBtn);

	// Draw maximize button
	if (style.test(WindowStyle::MaximizeBox)) {
		bool const pressed = caption.MaximizeBtn.state() == ButtonState::Pushed;
		args.Graphics->fillRect(components.MaximizeBtn, SystemColour::ButtonFace);
		args.Graphics->drawEdge(components.MaximizeBtn, pressed ? EdgeFlags::Sunken : EdgeFlags::Raised);
		args.Graphics->textColour(activeCaption ? SystemColour::WindowText : SystemColour::GrayText);
		auto const btnRect = components.MaximizeBtn + (pressed ? Point{1,1} : Point::Zero);
		args.Graphics->drawText(!dlg.maximized() ? L"▲" : L"⌂", btnRect, DrawTextFlags::SimpleCentre);
	}

	// Draw minimize button
	if (style.test(WindowStyle::MinimizeBox)) {
		bool const pressed = caption.MinimizeBtn.state() == ButtonState::Pushed;
		args.Graphics->fillRect(components.MinimizeBtn, SystemColour::ButtonFace);
		args.Graphics->drawEdge(components.MinimizeBtn, pressed ? EdgeFlags::Sunken : EdgeFlags::Raised);
		args.Graphics->textColour(activeCaption ? SystemColour::WindowText : SystemColour::GrayText);
		auto const btnRect = components.MinimizeBtn + (pressed ? Point{1,1} : Point::Zero);
		args.Graphics->drawText(L"▼", btnRect, DrawTextFlags::SimpleCentre);
	}
	
	// Draw window menu bar background
	if (components.Caption.Bottom < args.Client.Top) {
		args.Graphics->setBrush(dlg.backColour());
		args.Graphics->fillRect(components.MenuBar);
	}

	args.Graphics->restore();
	args.endPaint();
	return 0;
}

Win31LookNFeel::FontDescription
Win31LookNFeel::default() const
{
	using namespace forms::literals;
	return FontDescription{L"FixedSys Excelsior", 12_pt};
}
