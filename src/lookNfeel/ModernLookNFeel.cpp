#include "lookNfeel/ModernLookNFeel.h"
#include "lookNfeel/Nt6LookNFeel.h"
#include "lookNfeel/LookNFeelDecorator.h"
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
	this->Colours.Caption = {SystemColour::ActiveCaption, SystemColour::InactiveCaption};
	this->Colours.Control = Colour::White;
    this->Colours.Highlight = SystemColour::Highlight;
    this->Colours.Primary = Colour::Black;
    this->Colours.Secondary = SystemColour::ButtonFace;
    this->Colours.Tertiary = SystemColour::ButtonShadow;
	this->Colours.Window = Colour::White;
}

Rect
ModernLookNFeel::clientRect(Window& wnd, Rect bounds) const
{	
	Size const Frame{SystemMetric::cxSizeFrame, SystemMetric::cySizeFrame};
	Rect client = bounds - Border{2 * Frame} - Border{0, 1.5f * Measurement{SystemMetric::cyCaption}, 0, 0};
	if (wnd.menu().has_value())
		client.Top += 1.5f * Measurement{SystemMetric::cyMenu};
	return client;
}

bool
ModernLookNFeel::customCaption() const {
	return true;
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
	
	// Define objects used to render ball in colours reflecting enabled state
	auto const enabled = ctrl.enabled();
	Brush ballInterior{enabled ? this->primary() : this->tertiary()};
	Pen   ballOutline{enabled ? this->primary() : this->tertiary(), 2};

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
		args.Graphics.setPen(ballOutline);
		args.Graphics.setBrush(ballInterior);
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
		args.Graphics.setPen(ballOutline);
		args.Graphics.outlinePath();
		
		// Draw dark ball on left
		args.Graphics.setBrush(ballInterior);
		args.Graphics.drawEllipse(Rect{toggle[0].centre(), ballSize, Rect::FromCentre});
	}
	
	// Draw text
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
	Nt6LookNFeel{this->shared_from_this()}.draw(ctrl, args);
}

void
ModernLookNFeel::draw(Dialog& dlg, PaintWindowEventArgs const& args)
{
	// Erase background
	args.Graphics.setBrush(this->window());
	args.Graphics.fillRect(args.Area);

	args.Graphics.restore();
}

Response
ModernLookNFeel::draw(Dialog& dlg, NonClientPaintEventArgs& args) 
{
	ThrowIfNot(args, args.Graphics == nullopt);
	args.beginPaint();

	auto const isActive = args.CaptionState == WindowCaptionState::Active;
	auto const components = this->nonClient(Coords::Window, args.Window.style(), args.Bounds);
	auto const captionColour = isActive ? this->caption().Active : this->caption().Inactive;
	auto const textColour = isActive ? this->primary() : this->tertiary();
	auto const style = dlg.style();
	AnyColour const buttonColours[2] { captionColour, this->secondary() };

	// Draw window caption; colour according to activation-state
	Region const captionArea{Rect{args.Bounds.Left, args.Bounds.Top, args.Bounds.Right, components.MenuBar.Top}};
	args.Graphics->setBrush(captionColour);
	args.Graphics->fillRegion(captionArea);

	// Draw window frame consistently in 'inactive' colours
	Region const frameEdges = args.Area - Region{args.Client} - captionArea;
	args.Graphics->setBrush(this->caption().Inactive);
	args.Graphics->fillRegion(frameEdges);

	// Draw super-thin outline around entire window
	args.Graphics->setPen(StockPen::Black);
	args.Graphics->frameRegion(args.Area, Brush{textColour}, Size{1,1});
	
	// Draw caption text
	args.Graphics->setFont(StockObject::SystemFixedFont);
	args.Graphics->textColour(textColour, transparent);
	args.Graphics->drawText(dlg.text(), components.Title, DrawTextFlags::SimpleLeft);

	// Draw application icon in caption
	if (auto appIcon = dlg.wndcls().LargeIcon; appIcon)
		//! @todo  Draw system-default icon if app doesn't provide an icon
		args.Graphics->drawIcon(*appIcon, components.SysMenuBtn);
	
	// Draw maximize button
	if (style.test(WindowStyle::MaximizeBox)) {
		bool const isPressed = dlg.caption().MaximizeBtn.state() == ButtonState::Pushed;
		auto const pressedOffset = isPressed ? Point{1,1} : Point::Zero;
		args.Graphics->setBrush(buttonColours[isPressed]);
		args.Graphics->fillRect(components.MaximizeBtn);
		args.Graphics->drawText(L"□", components.MaximizeBtn + pressedOffset, DrawTextFlags::SimpleCentre);
	}

	// Draw minimize button
	if (style.test(WindowStyle::MinimizeBox)) {
		bool const isPressed = dlg.caption().MinimizeBtn.state() == ButtonState::Pushed;
		auto const pressedOffset = isPressed ? Point{1,1} : Point::Zero;
		args.Graphics->setBrush(buttonColours[isPressed]);
		args.Graphics->fillRect(components.MinimizeBtn);
		args.Graphics->drawText(L"▬", components.MinimizeBtn + pressedOffset, DrawTextFlags::SimpleCentre);
	}
	
	// Draw close button
	if (style.test(WindowStyle::SysMenu)) {
		bool const isPressed = dlg.caption().CloseBtn.state() == ButtonState::Pushed;
		auto const pressedOffset = isPressed ? Point{1,1} : Point::Zero;
		args.Graphics->setBrush(buttonColours[isPressed]);
		args.Graphics->fillRect(components.CloseBtn);
		args.Graphics->drawText(L"X", components.CloseBtn + pressedOffset, DrawTextFlags::SimpleCentre);
	}

	// Draw menu bar
	if (auto menu = dlg.menuBar(); menu.has_value()) {
		Rect const wndRect = dlg.wndRect();
		Pen const primaryPen{this->primary(), 1};
		Pen const backgroundPen{dlg.backColour(), 1};
		Rect lastItemArea;
		// Draw menu headings (top-level items)
		for (auto last = menu->Items.back(); auto const item : menu->Items) {
			auto const isSelected = item.hover(); 
			auto const customArea = item.area(dlg.handle(), components.MenuBar + wndRect.topLeft()) - wndRect.topLeft();
			args.Graphics->setPen(isSelected ? primaryPen : backgroundPen);
			args.Graphics->setBrush(isSelected ? this->secondary() : dlg.backColour());
			args.Graphics->drawRect(customArea);
			args.Graphics->drawText(item.text(), customArea, DrawTextFlags::SimpleCentre);
			if (item.index() == last.index())
				lastItemArea = customArea;
		}
		// Fill remaining area between right-most item and window's right-hand edge
		args.Graphics->setBrush(dlg.backColour());
		args.Graphics->fillRect(components.MenuBar + Rect{lastItemArea.Right,0,0,0});
	}

	args.Graphics->restore();
	args.endPaint();
	return 0;
}

NonClientLayout
ModernLookNFeel::nonClient(Coords results, nstd::bitset<WindowStyle> style, Rect wnd) const 
{
	ThrowIf(results, results == Coords::Client);
	
	// Base non-client area upon the default
	NonClientLayout bounds = base::nonClient(results, style, wnd);
	
	// Extend height of caption by 50%
	Rect::value_type const CaptionExtension = bounds.Caption.height() / 2;
	bounds.Caption.Bottom += CaptionExtension;
	bounds.MenuBar.translate({0, CaptionExtension});
	bounds.MenuBar.Bottom += Measurement{SystemMetric::cyMenu} / 2;

	// Move the minimize button when the window cannot be maximized
	if (style.test(WindowStyle::MinimizeBox) && !style.test(WindowStyle::MaximizeBox)) {
		bounds.MinimizeBtn = bounds.MaximizeBtn;
		bounds.MaximizeBtn = Rect::Empty;
	}

	return bounds;
}
