#include "lookNfeel/RetroLookNFeel.h"
#include "controls/CommonControls.h"
using namespace core;
using namespace forms;

std::shared_ptr<ILookNFeelProvider> const
RetroLookNFeel::Instance = std::make_shared<RetroLookNFeel>();

RetroLookNFeel::RetroLookNFeel()
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
RetroLookNFeel::default() 
{
	using namespace forms::literals;
	return FontDescription{L"Lucida Console", 11_pt};
}

void
RetroLookNFeel::draw(ButtonControl& ctrl, OwnerDrawEventArgs const& args) 
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
	auto const focused = state.test(ButtonState::Focus);
	if (focused && !pushed) {
		Pen outline{this->tertiary(), 2, PenStyle::InsideFrame};
		args.Graphics.setBrush(StockBrush::Hollow);
		args.Graphics.setPen(outline);
		args.Graphics.drawRect(args.Item.Area);
	}

	args.Graphics.restore();
}

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
	auto const focused = ctrl.state().test(ButtonState::Focus);
	if (focused) {
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
RetroLookNFeel::draw(Dialog& dlg, PaintWindowEventArgs const& args)
{
	// Erase background
	args.Graphics->setBrush(this->window());
	args.Graphics->fillRect(*args.Area);

	args.Graphics->restore();
}

void
RetroLookNFeel::draw(Window& wnd, NonClientPaintEventArgs const& args) 
{
	ThrowIf(args, !args.Graphics);

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
}
