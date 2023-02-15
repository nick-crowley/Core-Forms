#include "lookNfeel/Nt6LookNFeel.h"
#include "controls/CommonControls.h"
using namespace core;
using namespace forms;
	
std::shared_ptr<ILookNFeelProvider> const
Nt6LookNFeel::Instance = std::make_shared<Nt6LookNFeel>();

Nt6LookNFeel::Nt6LookNFeel()
  : SegoeUi{L"Segoe UI",20}
{}

void
Nt6LookNFeel::draw(Dialog& dlg, PaintWindowEventArgs const& args)
{
	args.Graphics->setObj(SystemColour::BtnFace);
	args.Graphics->fillRect(*args.Area);

	args.Graphics->restore();
}

void
Nt6LookNFeel::draw(Window& wnd, PaintNonClientEventArgs const& args) 
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
	args.Graphics->setObj(activeCaption ? SystemColour::Highlight : SystemColour::BtnDkShadow);
	//if (args.InvalidArea)
	//	args.Graphics->fillRegion(*args.InvalidArea);
	//else
		args.Graphics->fillRect(components.Caption);
	
	// Caption text
	args.Graphics->setObj(StockObject::SystemFixedFont);
	args.Graphics->setText(SystemColour::HighlightText);
	args.Graphics->setBack(DrawingMode::Transparent);
	args.Graphics->drawText(wnd.text(), components.Title, DrawTextFlags::SimpleCentre);

	args.Graphics->restore();
}

void
Nt6LookNFeel::initialize(Dialog& dlg, InitDialogEventArgs const& args) 
{
	for (Window* const ctrl : dlg.Children) {
		ctrl->font(this->SegoeUi.handle());
	}
}
