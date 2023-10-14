#include "lookNfeel/LookNFeelProvider.h"
#include "controls/CommonControls.h"
#include "graphics/FontBuilder.h"
using namespace core;
using namespace forms;

DrawTextFlags
forms::calculateFlags(nstd::bitset<ButtonStyle> style) noexcept
{
	nstd::bitset<DrawTextFlags> textAlign = style.test(ButtonStyle::Left)  ? DrawTextFlags::Left
	                                      : style.test(ButtonStyle::Right) ? DrawTextFlags::Right
	                                      :                                  DrawTextFlags::Centre;
	
	textAlign |= style.test(ButtonStyle::Top)     ? DrawTextFlags::Top
	           : style.test(ButtonStyle::Bottom)  ? DrawTextFlags::Bottom
	           :                                    DrawTextFlags::VCentre|DrawTextFlags::SingleLine;

	return textAlign;
}

DrawTextFlags
forms::calculateFlags(nstd::bitset<StaticStyle> style) noexcept
{
	auto const typeBits = style & StaticStyle::TypeMask;
	nstd::bitset<DrawTextFlags> textAlign = typeBits == StaticStyle::Right  ? DrawTextFlags::Right
	                                      : typeBits == StaticStyle::Centre ? DrawTextFlags::Centre
	                                      :                                   DrawTextFlags::Left;

	if (style.test(StaticStyle::CentreImage))
		textAlign |= DrawTextFlags::VCentre|DrawTextFlags::SingleLine;
	else
		textAlign |= DrawTextFlags::WordBreak;
	
	return textAlign;
}

EdgeFlags
forms::calculateFlags(nstd::bitset<ExWindowStyle> style) noexcept
{
	if (style.test(ExWindowStyle::ClientEdge))
		return EdgeFlags::SunkenOuter;
	if (style.test(ExWindowStyle::StaticEdge))
		return EdgeFlags::SunkenInner;
	if (style.test(ExWindowStyle::DlgModalFrame))
		return EdgeFlags::RaisedOuter;
	return EdgeFlags::None;
}

Size
forms::measureEdge(nstd::bitset<ExWindowStyle> style) noexcept
{
	if (style.test(ExWindowStyle::ClientEdge|ExWindowStyle::StaticEdge))
		return {SystemMetric::cxBorder, SystemMetric::cyBorder};
	if (style.test(ExWindowStyle::DlgModalFrame))
		return {SystemMetric::cxSizeFrame, SystemMetric::cySizeFrame};
	return Size::Zero;
}

void
forms::drawWindowBorder(DeviceContext& graphics, Rect const& client, nstd::bitset<WindowStyle> style, nstd::bitset<ExWindowStyle> exStyle) noexcept
{
	auto constexpr edgeStyles = ExWindowStyle::StaticEdge|ExWindowStyle::ClientEdge|ExWindowStyle::DlgModalFrame;

	if (exStyle.test(edgeStyles)) 
		graphics.drawEdge(client, calculateFlags(exStyle), BorderFlags::Rect);

	else if (style.test(WindowStyle::Border)) {
		graphics.setPen(StockPen::Black);
		graphics.drawRect(client);
	}
}

LookNFeelProvider::LookNFeelProvider() 
  : Fonts{
    .Heading1{StockFont::DefaultGui},
    .Heading2{StockFont::DefaultGui},
    .Paragraph{StockFont::DefaultGui}
  },
  Colours{
    .Button{SystemColour::ButtonFace},
	.Highlight{SystemColour::Highlight},
    .Primary{SystemColour::WindowText},
    .Secondary{SystemColour::GrayText},
	.Tertiary{SystemColour::GrayText},
    .Window{SystemColour::Window}
  }
{
}

Font
LookNFeelProvider::makeDefault() {
	return FontBuilder{}.withName(this->default().Name).withSize(this->default().Height);
}

void
LookNFeelProvider::draw(ButtonControl& ctrl, OwnerDrawEventArgs const& args) 
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"Button #{} must be OwnerDraw", args.Ident};
	
	// Erase background
	args.Graphics.setBrush(ctrl.backColour());
	args.Graphics.fillRect(args.Item.Area);

	// Draw edges
	auto const state = ctrl.state();
	auto const pushed = state.test(ButtonState::Pushed);
	args.Graphics.drawEdge(args.Item.Area, pushed ? EdgeFlags::Sunken : EdgeFlags::Raised);
#if 0
	//args.Graphics.set(::GetSysColorBrush((int)(enabled ? SystemColour::ButtonFace : SystemColour::GrayText)));
	args.Graphics.fillRect(args.Item.Area, ::GetSysColorBrush((int)(enabled ? SystemColour::ButtonFace : SystemColour::ButtonDkShadow)));
	
	args.Graphics.drawControl(args.Item.Area, DFC_BUTTON, pushed   ? DFCS_BUTTONPUSH|DFCS_PUSHED
	                                                      : !enabled ? DFCS_BUTTONPUSH|DFCS_INACTIVE
	                                                      :            DFCS_BUTTONPUSH);
#endif

	// Draw text
	auto const enabled = ctrl.enabled();
	Rect const content = args.Item.Area - Border{SystemMetric::cxFixedFrame} + (pushed ? Point{1,1} : Point::Zero);
	args.Graphics.setFont(ctrl.font());
	args.Graphics.textColour(enabled ? ctrl.textColour() : SystemColour::GrayText, ctrl.backColour());
	args.Graphics.drawText(ctrl.text(), content, calculateFlags(ctrl.style<ButtonStyle>()));

	// Draw focus rectangle
	auto const focused = state.test(ButtonState::Focus);
	if (focused)
		args.Graphics.drawFocus(content);

	args.Graphics.restore();
}

void
LookNFeelProvider::draw(CheckBoxControl& ctrl, OwnerDrawEventArgs const& args) 
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"CheckBox #{} must be OwnerDraw", args.Ident};
	
	// Erase background
	args.Graphics.setBrush(ctrl.backColour());
	args.Graphics.fillRect(args.Item.Area);
	
	// Draw check
	auto const checked = ctrl.checked();
	Rect const content = args.Item.Area;
	Rect const tick {content.topLeft(), Size{SystemMetric::cxSmallIcon,SystemMetric::cySmallIcon}};
	args.Graphics.drawControl(tick, DFC_BUTTON, DFCS_BUTTONCHECK|(checked?DFCS_CHECKED:0));
	
	// Draw text
	auto const enabled = ctrl.enabled();
	Rect const areaText = content - Border{SystemMetric::cxSmallIcon,0,0,0} - Border{SystemMetric::cxEdge,0,0,0};
	args.Graphics.setFont(ctrl.font());
	args.Graphics.textColour(enabled ? ctrl.textColour() : SystemColour::GrayText, ctrl.backColour());
	args.Graphics.drawText(ctrl.text(), areaText, calculateFlags(ctrl.style<ButtonStyle>()));
	
	// Draw focus rectangle
	auto const focused = ctrl.state().test(ButtonState::Focus);
	if (focused)
		args.Graphics.drawFocus(content);

	args.Graphics.restore();
}

void
LookNFeelProvider::draw(ComboBoxControl& ctrl, OwnerDrawEventArgs const& args) 
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"ComboBox #{} must be OwnerDraw", args.Ident};
	
	Size constexpr  static BigIcon{48,48};
	Size constexpr  static SmallIcon{24,24};

	// Query basic item state
	bool const selected = args.Item.State.test(OwnerDrawState::Selected);
	bool const withinEdit = args.Item.State.test(OwnerDrawState::ComboBoxEdit);
	auto const backColour = selected ? this->highlight() : ctrl.backColour();
	
	// Draw item background
	Rect const rcContent = args.Item.Area - Border{measureEdge(ctrl.exStyle()).Width};
	args.Graphics.setBrush(backColour);
	args.Graphics.fillRect(rcContent);
	final_act(&) noexcept { 
		args.Graphics.restore();
	};
	
	// FIX: Manually erase background of gap beneath bottom item
	//! @todo  Determine whether this gap only exists because measuring code has falling out-of-sync with drawing code for multi-line items with a title
	if (ctrl.dropped() && args.Item.Index == ctrl.Items.size()-1) {
		Rect const rcBottom = {rcContent.Left, rcContent.Top, rcContent.Right, ctrl.info().DroppedItemList.clientRect().Bottom};
		args.Graphics.fillRect(rcBottom);
	}

	// [NO-SELECTED-ITEM] Occurs in DropDownList mode
	if (args.Item.Index == args.Empty && !ctrl.Items.selected()) 
		return;
	
	// Query custom features
	auto const item = ctrl.Items[args.Item.Index];
	auto const selectedTextColour = nstd::make_optional_if<AnyColour>(selected, SystemColour::HighlightText);
	bool const useHeadings = ctrl.features().test(ComboBoxFeature::Headings);
	bool const useIcons = ctrl.features().test(ComboBoxFeature::Icons);
	auto const heading = item.heading(); 
	auto const icon = item.icon();

	// [HEADING] Draw heading and calculate different rectangle for (multi-line) detail text
	Rect rcDetailText = rcContent;
	if (useHeadings) {
		Invariant(heading.has_value());

		// Prefer heading font + selected-text colour; fallback to heading-default then control-default
		args.Graphics.setFont(heading->Font.value_or(ctrl.headingFont().value_or(ctrl.font())));
		args.Graphics.textColour(selectedTextColour.value_or(heading->Colour.value_or(ctrl.textColour())), transparent);
		
		// [HEADING-ICON] Draw icon on left; position both heading and detail text beside it
		Rect rcHeadingText = rcContent;
		if (useIcons) {
			Size const iconSize = withinEdit ? SmallIcon : BigIcon;
			Point const iconPosition = rcContent.topLeft();
			if (icon)  // Invariant(icon.has_value());
				args.Graphics.drawIcon(icon->handle(), iconPosition, iconSize);

			// Offset heading/detail drawing rectangles
			LONG const horzOffset = iconSize.Width + 3*GuiMeasurement{SystemMetric::cxFixedFrame};
			rcHeadingText.Left += horzOffset;
			rcDetailText.Left += horzOffset;
		}

		// Draw heading (position directly above detail)
		auto const titleHeight = args.Graphics.drawText(heading->Text, rcHeadingText, DrawTextFlags::Left);
		
		// [EDIT] Skip drawing the detail
		if (withinEdit) 
			return;

		rcDetailText += Point{0, titleHeight};
	}
	
	// [DETAIL-ICON] Draw on left; position detail text to right
	if (useIcons && !useHeadings) {
		Size const iconSize = SmallIcon;
		if (icon)	//Invariant(icon.has_value());
			args.Graphics.drawIcon(icon->handle(), rcContent.topLeft(), iconSize);
		rcDetailText.Left += iconSize.Width + 3*GuiMeasurement{SystemMetric::cxFixedFrame};
	}
	
	// Prefer detail font + selected-text colour; fallback to control-default
	auto const detail = item.detail();
	args.Graphics.setFont(detail.Font.value_or(ctrl.font()));
	args.Graphics.textColour(selectedTextColour.value_or(detail.Colour.value_or(ctrl.textColour())), transparent);

	// [DETAIL] Detail can be multi-line when drawing an OD-variable ListBox item; but they are
	//              vertically when OD-fixed or item is being drawn within the ComboBox/Edit
	DrawTextFlags constexpr  static MultilineFlags = DrawTextFlags::Left|DrawTextFlags::WordBreak;
	DrawTextFlags constexpr  static SinglelineFlags = DrawTextFlags::SimpleLeft;
	bool const odVariable = ctrl.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable);
	auto const flags = odVariable && !withinEdit ? MultilineFlags : SinglelineFlags;

	args.Graphics.drawText(detail.Text, rcDetailText, flags);
}

void
LookNFeelProvider::measure(ComboBoxControl& ctrl, MeasureItemEventArgs const& args)
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"ComboBox #{} must be OwnerDraw", args.Ident};

	bool const useHeadings = ctrl.features().test(ComboBoxFeature::Headings);
	bool const useIcons = ctrl.features().test(ComboBoxFeature::Icons);

	// [EDIT] Return greater of the edit-font height or icon height
	if (args.Item.Index == MeasureItemEventArgs::EditControl) {
		//! @remarks WM_MEASUREITEM is received extremely early in the process of ComboBox construction
		//!           so it's not even feasible to rely on its window font because the control hasn't yet
		//!           received its first WM_SETFONT
		auto const fontHeight = ctrl.editFont().height();
		args.Height = std::abs(fontHeight) + 2*GuiMeasurement{SystemMetric::cyFixedFrame};
		if (useIcons)
			args.Height = std::max<LONG>(args.Height, useHeadings ? 48 : 24);
		return;
	}

	// [FIXED-HEIGHT] Return greater of the detail height, icon height, or user-requested item height.
	//                 Font measured is 
	if (ctrl.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawFixed)) {
		args.Graphics.setFont(ctrl.font());
		args.Height = args.Graphics.measureText(L"Wjgy").Height;
		if (useIcons)
			args.Height = std::max<LONG>(args.Height, 24);
		args.Height = std::max<LONG>(args.Height, ctrl.Items.height());
	}
	// [VARIABLE-HEIGHT] Calculate per-item height
	else {
		auto const& item = *reinterpret_cast<ComboBoxControl::ItemData*>(args.Item.UserData);
		args.Height = 0;
		
		// [HEADING] Prefer heading font; fallback to heading-default then control-default
		if (useHeadings) {
			auto const heading = item.Heading; 
			Invariant(heading.has_value());
			args.Graphics.setFont(heading->Font.value_or(ctrl.headingFont().value_or(ctrl.font())));
			args.Height += args.Graphics.measureText(heading->Text).Height;
		}

		// Prefer detail font; fallback to control-default
		auto const detail = item.Detail;
		args.Graphics.setFont(detail.Font.value_or(ctrl.font()));
		
		// Its difficult to measure the dropped rectangle width because the scrollbar is optional :/
		Rect rcDetailText = ctrl.droppedRect() - Border{SystemMetric::cxFixedFrame, 0} - Border{0,0,0,SystemMetric::cxHScroll};

		// Detail may be offset by icon
		if (useIcons)
			rcDetailText.Left += (useHeadings ? 48 : 24) + 3*GuiMeasurement{SystemMetric::cxFixedFrame};

		// Measure multiline height
		args.Height += args.Graphics.measureText(detail.Text, Size{rcDetailText.width(),1}).Height;
		
		// [NO-HEADING] Add small gap between items
		if (!useHeadings) 
			args.Height += 2*GuiMeasurement{SystemMetric::cyFixedFrame};

		// Return greater of combined height, icon height, or user-requested height
		if (useIcons)
			args.Height = std::max<LONG>(args.Height, 24);

		//! @bug  Cannot set user-requested height at this moment
		//args.Height = std::max<LONG>(args.Height, item.height());
	}

	args.Graphics.restore();
}

void
LookNFeelProvider::erase(GroupBoxControl& ctrl, EraseBackgroundEventArgs const& args) 
{
	// Erase background
	Rect const rcClient = ctrl.clientRect();
	args.Graphics.setBrush(ctrl.backColour());
	args.Graphics.fillRect(rcClient);

	// Draw window border
	drawWindowBorder(args.Graphics, rcClient, ctrl.style(), ctrl.exStyle());
	
	args.Graphics.restore();
}

void
LookNFeelProvider::draw(LabelControl& ctrl, OwnerDrawEventArgs const& args) 
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"Label #{} must be OwnerDraw", args.Ident};
	
	// Erase background
	args.Graphics.setBrush(ctrl.backColour());
	args.Graphics.fillRect(args.Item.Area);

	// Draw text
	auto const style = (ctrl.style<StaticStyle>() & ~StaticStyle::TypeMask) | ctrl.align();
	args.Graphics.setFont(ctrl.font());
	args.Graphics.textColour(ctrl.textColour(), ctrl.backColour());
	args.Graphics.drawText(ctrl.text(), args.Item.Area, calculateFlags(style));

	args.Graphics.restore();
}

void
LookNFeelProvider::draw(ListBoxControl& ctrl, OwnerDrawEventArgs const& args) 
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"ListBox #{} must be OwnerDraw", args.Ident};

	bool const selected = args.Item.State.test(OwnerDrawState::Selected);
	
	// Draw item background
	Rect const rcItem = args.Item.Area - Border{measureEdge(ctrl.exStyle()).Width, 0};
	auto const backColour = !selected ? ctrl.backColour() : this->highlight();
	args.Graphics.setBrush(backColour);
	args.Graphics.fillRect(rcItem);

	// Draw item text
	if (args.Item.Index != args.Empty) {
		args.Graphics.setFont(ctrl.font());
		args.Graphics.textColour(selected ? SystemColour::HighlightText : ctrl.textColour(),
								 selected ? this->highlight() : ctrl.backColour());
		args.Graphics.drawText(ctrl.Items[args.Item.Index].text(), rcItem);
	}

	args.Graphics.restore();
}

void
LookNFeelProvider::erase(ListBoxControl& ctrl, EraseBackgroundEventArgs const& args) 
{
	// Erase background
	Rect const rcClient = ctrl.clientRect();
	args.Graphics.setBrush(ctrl.background());
	args.Graphics.fillRect(rcClient);

	// Draw window border
	drawWindowBorder(args.Graphics, rcClient, ctrl.style(), ctrl.exStyle());

	args.Graphics.restore();
}

void
LookNFeelProvider::draw(PictureControl& ctrl, OwnerDrawEventArgs const& args) 
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"Picture #{} must be OwnerDraw", args.Ident};
	
	if (auto bitmap = ctrl.image(); bitmap) {
		if (auto* dc = ::CreateCompatibleDC(args.Graphics.handle()); !dc)
			win::LastError{}.throwAlways("Failed to create compatible DC");
		else {
			DeviceContext src{SharedDeviceContext{dc, destroy}};
			src.setBitmap(bitmap->handle());

			Rect rcDest = args.Item.Area;
			if (ctrl.style<StaticStyle>().test(StaticStyle::RealSizeImage)) 
				rcDest = Rect{args.Item.Area.topLeft(), bitmap->size()};	
			args.Graphics.copyBitmap(src.handle(), bitmap->depth(), bitmap->rect(), rcDest);

			src.restore();
		}
	}
	else if (auto icon = ctrl.icon(); icon) {
		Rect rcDest = args.Item.Area;
		if (ctrl.style<StaticStyle>().test(StaticStyle::RealSizeImage)) 
			rcDest = Rect{args.Item.Area.topLeft(), icon->size()};	
		args.Graphics.drawIcon(icon->handle(), rcDest);
	}
}

void
LookNFeelProvider::draw(RadioButtonControl& ctrl, OwnerDrawEventArgs const& args)
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"RadioButton #{} must be OwnerDraw", args.Ident};

	auto const enabled = ctrl.enabled();
	auto const checked = ctrl.checked();
	auto const focused = ctrl.state().test(ButtonState::Focus);
	
	// Draw radio glyph
	Rect const content = args.Item.Area - Border{SystemMetric::cxFixedFrame};
	Rect const radio {content.topLeft(), Size{20,20}};
	args.Graphics.drawControl(radio, DFC_BUTTON, DFCS_BUTTONRADIO|(checked?DFCS_CHECKED:0));

	// Draw text
	Rect const areaText = content - Border{30,0,0,0};
	args.Graphics.setFont(ctrl.font());
	args.Graphics.textColour(enabled ? ctrl.textColour() : SystemColour::GrayText, ctrl.backColour());
	args.Graphics.drawText(ctrl.text(), areaText, calculateFlags(ctrl.style<ButtonStyle>()));

	// Draw focus rectangle
	if (focused)
		args.Graphics.drawFocus(content);

	args.Graphics.restore();
}

void
LookNFeelProvider::draw(StaticControl& ctrl, OwnerDrawEventArgs const& args)
{
	args.Graphics.setBrush(ctrl.backColour());
	args.Graphics.drawText(ctrl.text(), args.Item.Area, calculateFlags(ctrl.style<StaticStyle>()));
	
	args.Graphics.restore();
}

void
LookNFeelProvider::onCreated(Window&, CreateWindowEventArgs args) {
	// nothing
}

AnyColour
LookNFeelProvider::button() {
	return this->Colours.Button;
}

AnyColour
LookNFeelProvider::control() {
	return this->Colours.Control;
}

Font
LookNFeelProvider::heading1() {
	return this->Fonts.Heading1;
}

Font
LookNFeelProvider::heading2() {
	return this->Fonts.Heading2;
}

AnyColour
LookNFeelProvider::highlight() {
	return this->Colours.Highlight;
}

Font
LookNFeelProvider::paragraph() {
	return this->Fonts.Paragraph;
}

AnyColour
LookNFeelProvider::primary() {
	return this->Colours.Primary;
}

AnyColour
LookNFeelProvider::secondary() {
	return this->Colours.Secondary;
}

AnyColour
LookNFeelProvider::tertiary() {
	return this->Colours.Tertiary;
}

AnyColour
LookNFeelProvider::window() {
	return this->Colours.Window;
}
