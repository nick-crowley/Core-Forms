#include "lookNfeel/Nt6LookNFeel.h"
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
    this->Colours.Primary = SystemColour::WindowText;
    this->Colours.Secondary = SystemColour::GrayText;
	this->Colours.Window = SystemColour::Dialog;
}

void
Nt6LookNFeel::draw(GroupBoxControl& ctrl, OwnerDrawEventArgs const& args) 
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"GroupBox #{} must be OwnerDraw", args.Ident};

	// Erase background
	args.Graphics.setBrush(ctrl.backColour());
	args.Graphics.fillRect(args.Item.Area);

	// Draw frame
	auto const text = ctrl.text();
	auto const textSize = args.Graphics.measureText(text);
	auto const frameRect = args.Item.Area - Border{1, textSize.Height/2, 0, 0};
	Pen const  thickPen{SystemColour::ButtonShadow, 2};
	args.Graphics.setPen(thickPen);
	args.Graphics.setBrush(StockBrush::Hollow);
	args.Graphics.drawRoundRect(frameRect, Size{16,16});

	// Draw text
	auto const frameText = L' ' + text + L' ';
	auto const textOffset = Point{SystemMetric::cxSmallIcon,0};
	args.Graphics.setFont(ctrl.font());
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

void
Nt6LookNFeel::initialize(Dialog& dlg, InitDialogEventArgs const& args) 
{
	using namespace forms::literals;
	this->WindowFont = Font{L"Segoe UI", dlg.clientDC().measureFont(11_pt)};

	for (Window& ctrl : dlg.Children) {
		switch (ctrl.role())
		{
		case WindowRole::ComboBox:
		case WindowRole::List:
			ctrl.backColour(SystemColour::Window);
			break;

		default:
			ctrl.backColour(SystemColour::Dialog);
			break;
		}
		ctrl.textColour(SystemColour::WindowText);
		ctrl.font(this->WindowFont);
		ctrl.lookNfeel(this->shared_from_this());
	}
}
