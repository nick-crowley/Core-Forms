#include "lookNfeel/RetroLookNFeel.h"
#include "lookNfeel/Win31LookNFeel.h"
#include "controls/CommonControls.h"
#include "graphics/FontBuilder.h"
using namespace core;
using namespace forms;

std::shared_ptr<ILookNFeelProvider> const
RetroLookNFeel::Instance = std::make_shared<RetroLookNFeel>();

RetroLookNFeel::RetroLookNFeel()
{
	using namespace forms::literals;
	this->Fonts.Paragraph = this->makeDefault();
	this->Fonts.Heading2 = FontBuilder{}.fromExisting(this->Fonts.Paragraph).withSize(18_pt).build();
	this->Fonts.Heading1 = FontBuilder{}.fromExisting(this->Fonts.Paragraph).withSize(24_pt).build();
	this->Colours.Button = SystemColour::ButtonFace;
	this->Colours.Control = Colour::White;
    this->Colours.Primary = Colour::Black;
    this->Colours.Secondary = SystemColour::ButtonFace;
    this->Colours.Tertiary = SystemColour::ButtonShadow;
	this->Colours.Window = Colour::White;
}

ILookNFeelProvider::FontDescription
RetroLookNFeel::default() 
{
	using namespace forms::literals;
	return FontDescription{L"Eight Bit Dragon", 11_pt};
}

//! @brief  Generates the points of an octagon
class Octagon {
	Rect              Bounds;
	Rect::value_type  Dx;   // Length from midpoint of rectangle edge to octagon corner
	Rect::value_type  Dy;   // Length from midpoint of rectangle edge to octagon corner

public:
	Octagon(Rect const& bounds) 
	  : Bounds{bounds}, 
	    Dx{Bounds.width() / 4}, 
	    Dy{Bounds.height() / 4}
	{
		this->Dx = this->Dy = std::min(this->Dx, this->Dy);
	}

	std::vector<Point>
	points() const {
		return {
			this->Bounds.topLeft()     + Point{0,          this->Dy},
			this->Bounds.topLeft()     + Point{this->Dx,   0},
			this->Bounds.topRight()    - Point{this->Dx+1, 0},
			this->Bounds.topRight()    + Point{0,          this->Dy},
			this->Bounds.bottomRight() - Point{0,          this->Dy},
			this->Bounds.bottomRight() - Point{this->Dx+1, 0},
			this->Bounds.bottomLeft()  + Point{this->Dx,   0},
			this->Bounds.bottomLeft()  - Point{0,          this->Dy},
			this->Bounds.topLeft()     + Point{0,          this->Dy},  // Start point [Closed figure]
		};
	}
};

void
RetroLookNFeel::draw(CheckBoxControl& ctrl, OwnerDrawEventArgs const& args) 
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
		// Define octagon shape
		args.Graphics.beginPath();
		args.Graphics.drawShape(Octagon{Rect{toggleOffset, toggleSize}}.points());
		args.Graphics.endPath();
		
		// [CHECKED] Fill stretched octagon
		Brush interior{SystemColour::Highlight};
		Pen   outline{SystemColour::Highlight, 2};
		args.Graphics.setPen(outline);
		args.Graphics.setBrush(interior);
		args.Graphics.fillPathAndOutline();
		
		// Draw small white octagon (on the right)
		args.Graphics.setPen(StockPen::White);
		args.Graphics.setBrush(StockBrush::White);
		args.Graphics.drawShape(Octagon{Rect{toggle[1].centre(), ballSize, Rect::FromCentre}}.points());
	}
	else {
		// Define octagon outline
		args.Graphics.beginPath();
		args.Graphics.drawLines(Octagon{Rect{toggleOffset, toggleSize}}.points());
		args.Graphics.endPath();

		// [UNCHECKED] Outline stretched octagon
		Pen outline{this->primary(), 2};
		args.Graphics.setPen(outline);
		args.Graphics.outlinePath();
		
		// Draw small dark octagon (on the left)
		Brush interior{this->primary()};
		args.Graphics.setBrush(interior);
		args.Graphics.drawShape(Octagon{Rect{toggle[0].centre(), ballSize, Rect::FromCentre}}.points());
	}
	
	// Draw text
	auto const enabled = ctrl.enabled();
	Rect const areaText = content - Border{smallIcon.Width,0,0,0} - Border{SystemMetric::cxEdge,0,0,0};
	args.Graphics.setFont(ctrl.font());
	args.Graphics.textColour(enabled ? ctrl.textColour() : this->tertiary(), ctrl.backColour());
	args.Graphics.drawText(ctrl.text(), areaText, calculateFlags(ctrl.style<ButtonStyle>()));
	
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
RetroLookNFeel::draw(GroupBoxControl& ctrl, OwnerDrawEventArgs const& args) 
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"GroupBox #{} must be OwnerDraw", args.Ident};

	// Same as Win3.1
	Win31LookNFeel::Instance->draw(ctrl, args);
}

Response
RetroLookNFeel::draw(Window& wnd, NonClientPaintEventArgs args) 
{
	ThrowIfNot(args, args.Graphics == nullopt);

	// Same as Win3.1
	return Win31LookNFeel::Instance->draw(wnd, args);
}
