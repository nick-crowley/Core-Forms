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
	// Erase background
	args.Graphics->setBrush(SystemBrush::Dialog);
	args.Graphics->fillRect(*args.Area);

	args.Graphics->restore();
}

void
Nt6LookNFeel::draw(Window& wnd, PaintNonClientEventArgs const& args) 
{
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
	for (Window* const ctrl : dlg.Children) {
		if (ctrl->role() != WindowRole::PushButton)
			ctrl->backColour(SystemColour::Dialog);
		ctrl->textColour(SystemColour::WindowText);
		ctrl->font(this->SegoeUi);
		ctrl->lookNfeel(this->shared_from_this());
	}
}
