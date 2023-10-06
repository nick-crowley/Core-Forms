#include "lookNfeel/Win31LookNFeel.h"
#include "controls/CommonControls.h"
using namespace core;
using namespace forms;

std::shared_ptr<ILookNFeelProvider> const
Win31LookNFeel::Instance = std::make_shared<Win31LookNFeel>();

Win31LookNFeel::Win31LookNFeel()
{
	using namespace forms::literals;
	this->Fonts.Paragraph = Font{this->default().Name, DeviceContext::ScreenDC.measureFont(this->default().Height)};
	this->Fonts.Heading2 = Font{*this->Fonts.Paragraph.handle(), std::nullopt, DeviceContext::ScreenDC.measureFont(12_pt)};
	this->Fonts.Heading1 = Font{*this->Fonts.Paragraph.handle(), std::nullopt, DeviceContext::ScreenDC.measureFont(14_pt)};
	this->Colours.Button = SystemColour::ButtonFace;
	this->Colours.Control = Colour::White;
    this->Colours.Primary = Colour::Black;
    this->Colours.Secondary = SystemColour::GrayText;
    this->Colours.Tertiary = SystemColour::GrayText;
	this->Colours.Window = Colour::White;
}

ILookNFeelProvider::FontDescription
Win31LookNFeel::default() 
{
	using namespace forms::literals;
	return FontDescription{L"FixedSys", 11_pt};
}

void
Win31LookNFeel::draw(GroupBoxControl& ctrl, OwnerDrawEventArgs const& args) 
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
Win31LookNFeel::draw(Dialog& dlg, PaintWindowEventArgs const& args)
{
	// Erase background
	args.Graphics->setBrush(this->window());
	args.Graphics->fillRect(*args.Area);

	args.Graphics->restore();
}

Response
Win31LookNFeel::draw(Window& wnd, NonClientPaintEventArgs args) 
{
	ThrowIfNot(args, args.Graphics == std::nullopt);
	args.beginPaint();

	auto const activeCaption = args.CaptionState == WindowCaptionState::Active;
	auto const components = NonClientComponentBounds{args.Window.style(), args.Bounds, Coords::Window};

	// Draw window frame
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
	args.Graphics->setFont(StockObject::SystemFixedFont);
	args.Graphics->textColour(SystemColour::HighlightText);
	args.Graphics->backColour(transparent);
	args.Graphics->drawText(wnd.text(), components.Title, DrawTextFlags::SimpleCentre);

	// Draw SysMenu button
	args.Graphics->setPen(StockPen::Black);
	args.Graphics->setBrush(SystemBrush::Dialog);
	args.Graphics->drawRect(components.SysMenuBtn);
	args.Graphics->setPen(StockObject::BlackPen);
	args.Graphics->setBrush(SystemBrush::Window);
	auto const sysMenuBorder = Border{Size{components.SysMenuBtn.width()/6, components.SysMenuBtn.height()/2 - 2}};
	auto const sysMenuBtn = Rect{components.Caption.topLeft(), components.SysMenuBtn.size()} - sysMenuBorder - Point{1,0};
	args.Graphics->drawRect(sysMenuBtn);

	// Draw maximize button
	if (wnd.style().test(WindowStyle::MaximizeBox)) {
		bool const pressed = args.CaptionButtons.MaximizeBtn == ButtonState::Pushed;
		args.Graphics->fillRect(components.MaximizeBtn, SystemColour::ButtonFace);
		args.Graphics->drawEdge(components.MaximizeBtn, pressed ? EdgeFlags::Sunken : EdgeFlags::Raised);
		args.Graphics->textColour(activeCaption ? SystemColour::WindowText : SystemColour::GrayText);
		auto const btnRect = components.MaximizeBtn + (pressed ? Point{1,1} : Point::Zero);
		args.Graphics->drawText(L"▲", btnRect, DrawTextFlags::SimpleCentre);
	}

	// Draw minimize button
	if (wnd.style().test(WindowStyle::MinimizeBox)) {
		bool const pressed = args.CaptionButtons.MinimizeBtn == ButtonState::Pushed;
		args.Graphics->fillRect(components.MinimizeBtn, SystemColour::ButtonFace);
		args.Graphics->drawEdge(components.MinimizeBtn, pressed ? EdgeFlags::Sunken : EdgeFlags::Raised);
		args.Graphics->textColour(activeCaption ? SystemColour::WindowText : SystemColour::GrayText);
		auto const btnRect = components.MinimizeBtn + (pressed ? Point{1,1} : Point::Zero);
		args.Graphics->drawText(L"▼", btnRect, DrawTextFlags::SimpleCentre);
	}
	
	args.Graphics->restore();
	args.endPaint();
	return 0;
}
