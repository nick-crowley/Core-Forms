#include "lookNfeel/Win31LookNFeel.h"
#include "controls/CommonControls.h"
using namespace core;
using namespace forms;

std::shared_ptr<ILookNFeelProvider> const
Win31LookNFeel::Instance = std::make_shared<Win31LookNFeel>();

Win31LookNFeel::Win31LookNFeel()
{}

void
Win31LookNFeel::draw(GroupBoxControl& ctrl, OwnerDrawEventArgs const& args) 
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
	Pen const  thickPen{Colour::Black, 2};
	args.Graphics.setPen(thickPen);
	args.Graphics.setBrush(StockBrush::Hollow);
	args.Graphics.drawRect(frameRect);

	// Draw text
	auto const frameText = L' ' + text + L' ';
	auto const textOffset = Point{SystemMetric::cxSmallIcon,0};
	args.Graphics.setFont(ctrl.font());
	args.Graphics.textColour(ctrl.textColour(), ctrl.backColour());
	args.Graphics.drawText(frameText, args.Item.Area + textOffset, DrawTextFlags::Top);

	args.Graphics.restore();
}

void
Win31LookNFeel::draw(Dialog& dlg, PaintWindowEventArgs const& args)
{
	// Erase background
	args.Graphics->setBrush(StockBrush::White);
	args.Graphics->fillRect(*args.Area);

	args.Graphics->restore();
}

void
Win31LookNFeel::draw(Window& wnd, NonClientPaintEventArgs const& args) 
{
	auto const activeCaption = args.State == WindowCaptionState::Active;
	auto const components = NonClientComponentBounds{args.Bounds};

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
		args.Graphics->fillRect(components.MaximizeBtn, SystemColour::ButtonFace);
		args.Graphics->drawEdge(components.MaximizeBtn, EdgeFlags::Raised);
		args.Graphics->textColour(SystemColour::WindowText);
		args.Graphics->drawText(L"▲", components.MaximizeBtn, DrawTextFlags::SimpleCentre);
	}

	// Draw minimize button
	if (wnd.style().test(WindowStyle::MinimizeBox)) {
		args.Graphics->fillRect(components.MinimizeBtn, SystemColour::ButtonFace);
		args.Graphics->drawEdge(components.MinimizeBtn, EdgeFlags::Raised);
		args.Graphics->textColour(SystemColour::WindowText);
		args.Graphics->drawText(L"▼", components.MinimizeBtn, DrawTextFlags::SimpleCentre);
	}
	
	args.Graphics->restore();
}

void
Win31LookNFeel::initialize(Dialog& dlg, InitDialogEventArgs const& args) 
{
	for (Window& ctrl : dlg.Children) {
		if (ctrl.role() != WindowRole::PushButton)
			ctrl.backColour(SystemColour::Window);
		ctrl.textColour(SystemColour::WindowText);
		ctrl.font(this->WindowFont);
		ctrl.lookNfeel(this->shared_from_this());
	}
}
