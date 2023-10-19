#include "lookNfeel/ModernLookNFeel.h"
#include "lookNfeel/Nt6LookNFeel.h"
#include "controls/CommonControls.h"
#include "graphics/FontBuilder.h"
using namespace core;
using namespace forms;

std::shared_ptr<ILookNFeelProvider> const
ModernLookNFeel::Instance = std::make_shared<ModernLookNFeel>();

ModernLookNFeel::ModernLookNFeel()
{
	using namespace forms::literals;
	this->Fonts.Paragraph = this->makeDefault();
	this->Fonts.Heading2 = FontBuilder{}.fromExisting(this->Fonts.Paragraph).withSize(18_pt);
	this->Fonts.Heading1 = FontBuilder{}.fromExisting(this->Fonts.Paragraph).withSize(24_pt);
	this->Colours.Button = SystemColour::ButtonFace;
	this->Colours.Control = Colour::White;
    this->Colours.Highlight = SystemColour::Highlight;
    this->Colours.Primary = Colour::Black;
    this->Colours.Secondary = SystemColour::ButtonFace;
    this->Colours.Tertiary = SystemColour::ButtonShadow;
	this->Colours.Window = Colour::White;
}

ILookNFeelProvider::FontDescription
ModernLookNFeel::default() 
{
	using namespace forms::literals;
	return FontDescription{L"Lucida Console", 11_pt};
}

void
ModernLookNFeel::draw(ButtonControl& ctrl, OwnerDrawEventArgs const& args) 
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"Button #{} must be OwnerDraw", args.Ident};
	
	// Draw background
	auto const state = ctrl.state();
	auto const pushed = state.test(ButtonState::Pushed);
	auto const backcolour = !pushed ? this->button() : this->tertiary();
	args.Graphics.setBrush(backcolour);
	args.Graphics.fillRect(args.Item.Area);

	// Draw text
	auto const enabled = ctrl.enabled();
	Rect const content = args.Item.Area - Border{SystemMetric::cxFixedFrame} + (pushed ? Point{1,1} : Point::Zero);
	args.Graphics.setFont(ctrl.font());
	args.Graphics.textColour(enabled ? ctrl.textColour() : this->tertiary(), backcolour);
	args.Graphics.drawText(ctrl.text(), content, forms::drawFlags(ctrl.align()));

	// Draw focus rectangle
	if (auto const focused = state.test(ButtonState::Focus); focused && !pushed) {
		Pen outline{this->tertiary(), 2, PenStyle::InsideFrame};
		args.Graphics.setBrush(StockBrush::Hollow);
		args.Graphics.setPen(outline);
		args.Graphics.drawRect(args.Item.Area);
	}

	args.Graphics.restore();
}

void
ModernLookNFeel::draw(CheckBoxControl& ctrl, OwnerDrawEventArgs const& args) 
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"CheckBox #{} must be OwnerDraw", args.Ident};
	
	// Erase background
	args.Graphics.setBrush(ctrl.backColour());
	args.Graphics.fillRect(args.Item.Area);
	
	// Define toggle area
	Rect const  content = args.Item.Area;
	Size const  smallIcon {SystemMetric::cxSmallIcon,SystemMetric::cySmallIcon};
	Size const  ballSize = smallIcon * .75f;
	Size const  toggleSize {long(smallIcon.Width * 2.5f), long(smallIcon.Height * 1.25f)};
	Point const toggleOffset = content.topLeft() + Point{0,(content.height()-toggleSize.Height)/2} + 2*Point{SystemMetric::cxFixedFrame,0};
	Rect const  toggle[2] {
		Rect{toggleOffset,                               Size{toggleSize.Width/2, toggleSize.Height}},
		Rect{toggleOffset + Point{toggleSize.Width/2,0}, Size{toggleSize.Width/2, toggleSize.Height}}
	};
	
	if (auto const checked = ctrl.checked(); checked) {
		// Fillable path requires a central rectangle
		args.Graphics.beginPath();
		args.Graphics.drawArc(toggle[0], toggle[0].topMiddle(), toggle[0].bottomMiddle());
		args.Graphics.drawRect(toggle[0] + Point{toggle[0].width()/2, 0} + Border{0,0,0,1});
		args.Graphics.drawArc(toggle[1], toggle[1].bottomMiddle(), toggle[1].topMiddle());
		args.Graphics.endPath();
		
		// [CHECKED] Fill extended oval
		Brush interior{SystemColour::Highlight};
		Pen   outline{SystemColour::Highlight, 2};
		args.Graphics.setPen(outline);
		args.Graphics.setBrush(interior);
		args.Graphics.fillPathAndOutline();
		
		// Draw white ball on right
		args.Graphics.setPen(StockPen::White);
		args.Graphics.setBrush(StockBrush::White);
		args.Graphics.drawEllipse(Rect{toggle[1].centre(), ballSize, Rect::FromCentre});
	}
	else {
		// Outline path mustn't contain vertical lines of a rectangle
		args.Graphics.beginPath();
		args.Graphics.drawArc(toggle[0], toggle[0].topMiddle(), toggle[0].bottomMiddle());
		args.Graphics.moveTo(toggle[0].bottomMiddle());
		args.Graphics.drawLine(toggle[1].bottomMiddle());
		args.Graphics.drawArc(toggle[1], toggle[1].bottomMiddle(), toggle[1].topMiddle());
		args.Graphics.moveTo(toggle[1].topMiddle());
		args.Graphics.drawLine(toggle[0].topMiddle());
		args.Graphics.endPath();

		// [UNCHECKED] Draw outline extended oval
		Pen outline{this->primary(), 2};
		args.Graphics.setPen(outline);
		args.Graphics.outlinePath();
		
		// Draw dark ball on left
		Brush interior{this->primary()};
		args.Graphics.setBrush(interior);
		args.Graphics.drawEllipse(Rect{toggle[0].centre(), ballSize, Rect::FromCentre});
	}
	
	// Draw text
	auto const enabled = ctrl.enabled();
	Rect const areaText = content + Rect{toggle[1].Right - content.Left + 3*Measurement{SystemMetric::cxEdge},0,0,0};
	args.Graphics.setFont(ctrl.font());
	args.Graphics.textColour(enabled ? ctrl.textColour() : this->tertiary(), ctrl.backColour());
	args.Graphics.drawText(ctrl.text(), areaText, forms::drawFlags(ctrl.align()));
	
	// Draw focus rectangle
	if (auto const focused = ctrl.state().test(ButtonState::Focus); focused) {
		Pen outline{SystemColour::ButtonShadow, 2, PenStyle::InsideFrame};
		args.Graphics.setBrush(StockBrush::Hollow);
		args.Graphics.setPen(outline);
		args.Graphics.drawRect(args.Item.Area);
	}

	args.Graphics.restore();
}

void
ModernLookNFeel::draw(GroupBoxControl& ctrl, OwnerDrawEventArgs const& args) 
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"GroupBox #{} must be OwnerDraw", args.Ident};

	// Same as NT6
	Nt6LookNFeel::Instance->draw(ctrl, args);
}

void
ModernLookNFeel::draw(Dialog& dlg, PaintWindowEventArgs const& args)
{
	// Erase background
	args.Graphics->setBrush(this->window());
	args.Graphics->fillRect(*args.Area);

	args.Graphics->restore();
}

Response
ModernLookNFeel::draw(Window& wnd, NonClientPaintEventArgs args) 
{
	// Use Windows default
	return Window::Unhandled;
}
