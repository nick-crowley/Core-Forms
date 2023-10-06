#include "lookNfeel/ModernLookNFeel.h"
#include "lookNfeel/Nt6LookNFeel.h"
#include "controls/CommonControls.h"
using namespace core;
using namespace forms;

std::shared_ptr<ILookNFeelProvider> const
ModernLookNFeel::Instance = std::make_shared<ModernLookNFeel>();

ModernLookNFeel::ModernLookNFeel()
{
	using namespace forms::literals;
	this->Fonts.Paragraph = Font{this->default().Name, DeviceContext::ScreenDC.measureFont(this->default().Height)};
	this->Fonts.Heading2 = Font{*this->Fonts.Paragraph.handle(), std::nullopt, DeviceContext::ScreenDC.measureFont(18_pt)};
	this->Fonts.Heading1 = Font{*this->Fonts.Paragraph.handle(), std::nullopt, DeviceContext::ScreenDC.measureFont(24_pt)};
	this->Colours.Button = SystemColour::ButtonFace;
	this->Colours.Control = Colour::White;
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
	
	// Erase background
	args.Graphics.setBrush(ctrl.backColour());
	args.Graphics.fillRect(args.Item.Area);

	// Draw background
	auto const state = ctrl.state();
	auto const pushed = state.test(ButtonState::Pushed);
	auto const backcolour = !pushed ? this->button() : this->tertiary();
	args.Graphics.fillRect(args.Item.Area, Brush{backcolour}.handle());

	// Draw text
	auto const enabled = ctrl.enabled();
	Rect const content = args.Item.Area - Border{SystemMetric::cxFixedFrame} + (pushed ? Point{1,1} : Point::Zero);
	args.Graphics.setFont(ctrl.font());
	args.Graphics.textColour(enabled ? ctrl.textColour() : this->tertiary(), backcolour);
	args.Graphics.drawText(ctrl.text(), content, calculateFlags(ctrl.style<ButtonStyle>()));

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
	Rect const  toggle {content.topLeft()+Point{2, (content.height()-(smallIcon*1.25f).Height)/2}, 1.25f*smallIcon};
	Point const bottomMiddle = Point::midPoint(toggle.bottomLeft(), toggle.bottomRight());
	Point const topMiddle = Point::midPoint(toggle.topLeft(), toggle.topRight());
	Rect const  ball {toggle.centre(), toggle.size()*.6f, Rect::FromCentre};
	
	if (auto const checked = ctrl.checked(); checked) {
		// Fillable path requires a central rectangle
		args.Graphics.beginPath();
		args.Graphics.drawArc(toggle, topMiddle, bottomMiddle);
		args.Graphics.drawRect(Rect{topMiddle+Point{1,0}, Size{25,toggle.height()+1}});
		args.Graphics.drawArc(toggle + Point{25,0}, bottomMiddle + Point{25,0}, topMiddle + Point{25,0});
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
		args.Graphics.drawEllipse(ball + Point{25,0});
	}
	else {
		// Outline path mustn't contain vertical lines of a rectangle
		args.Graphics.beginPath();
		args.Graphics.drawArc(toggle, topMiddle, bottomMiddle);
		args.Graphics.moveTo(bottomMiddle);
		args.Graphics.lineTo(bottomMiddle + Point{25,0});
		args.Graphics.drawArc(toggle + Point{25,0}, bottomMiddle + Point{25,0}, topMiddle + Point{25,0});
		args.Graphics.moveTo(topMiddle + Point{25,0});
		args.Graphics.lineTo(topMiddle);
		args.Graphics.endPath();

		// [UNCHECKED] Draw outline extended oval
		Pen outline{this->primary(), 2};
		args.Graphics.setPen(outline);
		args.Graphics.outlinePath();
		
		// Draw dark ball on left
		Brush interior{this->primary()};
		args.Graphics.setBrush(interior);
		args.Graphics.drawEllipse(ball);
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
