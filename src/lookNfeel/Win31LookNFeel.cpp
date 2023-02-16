#include "lookNfeel/Win31LookNFeel.h"
#include "controls/CommonControls.h"
using namespace core;
using namespace forms;

std::shared_ptr<ILookNFeelProvider> const
Win31LookNFeel::Instance = std::make_shared<Win31LookNFeel>();

void
Win31LookNFeel::draw(Dialog& dlg, PaintWindowEventArgs const& args)
{
	args.Graphics->setObj(StockObject::WhiteBrush);
	args.Graphics->fillRect(*args.Area);

	args.Graphics->restore();
}

void
Win31LookNFeel::draw(Window& wnd, PaintNonClientEventArgs const& args) 
{
	auto const activeCaption = args.State == WindowCaptionState::Active;
	auto const components = NonClientComponentBounds{args.Bounds};

	// Window frame
	args.Graphics->setObj(DeviceContext::get<StockObject::LtGreyBrush>());
	args.Graphics->setObj(StockObject::BlackPen);
	Rect const insideFrame = args.Bounds - 2*Border{SystemMetric::cxSizeFrame,SystemMetric::cySizeFrame};
	Region const frameEdges = args.Area - Region{insideFrame};
	args.Graphics->fillRegion(frameEdges);
	args.Graphics->frameRegion(frameEdges, DeviceContext::get<StockObject::DkGreyBrush>(), Size{2,2});
	
	// Caption background
	args.Graphics->setObj(activeCaption ? SystemColour::Highlight : SystemColour::ButtonDkShadow);
	//if (args.InvalidArea)
	//	args.Graphics->fillRegion(*args.InvalidArea);
	//else
		args.Graphics->fillRect(components.Caption);
	
	// Caption text
	args.Graphics->setObj(StockObject::SystemFixedFont);
	args.Graphics->setText(SystemColour::HighlightText);
	args.Graphics->setBack(DrawingMode::Transparent);
	args.Graphics->drawText(wnd.text(), components.Title, DrawTextFlags::SimpleCentre);

	// SysMenu button
	args.Graphics->setObj(StockObject::BlackPen);
	args.Graphics->setObj(SystemColour::ButtonFace);
	args.Graphics->drawRect(components.SysMenuBtn);
	args.Graphics->setObj(StockObject::BlackPen);
	args.Graphics->setObj(SystemColour::Window);
	Rect rcSysMenuBar {components.Caption.topLeft(), components.SysMenuBtn.size()};
	rcSysMenuBar.inflate(-Size{components.SysMenuBtn.width()/6, components.SysMenuBtn.height()/2 - 2});
	rcSysMenuBar.Left--; // Fix: Compensate for manually adjusted SysMenu hit-box to correctly align left-edge
	args.Graphics->drawRect(rcSysMenuBar);

	// Maximize button
	if (wnd.style().test(WindowStyle::MaximizeBox)) {
		args.Graphics->fillRect(components.MaximizeBtn, SystemColour::ButtonFace);
		args.Graphics->drawEdge(components.MaximizeBtn, EdgeFlags::Raised);
		args.Graphics->setText(SystemColour::WindowText);
		args.Graphics->drawText(L"▲", components.MaximizeBtn, DrawTextFlags::SimpleCentre);
	}

	// Minimize button
	if (wnd.style().test(WindowStyle::MinimizeBox)) {
		args.Graphics->fillRect(components.MinimizeBtn, SystemColour::ButtonFace);
		args.Graphics->drawEdge(components.MinimizeBtn, EdgeFlags::Raised);
		args.Graphics->setText(SystemColour::WindowText);
		args.Graphics->drawText(L"▼", components.MinimizeBtn, DrawTextFlags::SimpleCentre);
	}
	
	args.Graphics->restore();
}

void
Win31LookNFeel::initialize(Dialog& dlg, InitDialogEventArgs const& args) 
{
	for (Window* const ctrl : dlg.Children) {
		ctrl->font(DeviceContext::get<StockObject::SystemFixedFont>());
	}
}
