﻿#include "lookNfeel/LookNFeelProvider.h"
#include "controls/CommonControls.h"
#include "graphics/FontBuilder.h"
using namespace core;
using namespace forms;

DrawTextFlags
forms::drawTextFlags(nstd::bitset<Alignment> align) noexcept
{
	nstd::bitset<DrawTextFlags> flags = align.test(Alignment::Left)   ? DrawTextFlags::Left
	                                  : align.test(Alignment::Right)  ? DrawTextFlags::Right
	                                  : align.test(Alignment::Centre) ? DrawTextFlags::Centre
	                                                                  : DrawTextFlags::None;
	
	flags |= align.test(Alignment::Top)     ? DrawTextFlags::Top
	       : align.test(Alignment::Bottom)  ? DrawTextFlags::Bottom
	       : align.test(Alignment::VCentre) ? DrawTextFlags::VCentre|DrawTextFlags::SingleLine
	       :                                  DrawTextFlags::None;

	return flags;
}

EdgeFlags
forms::edgeFlags(nstd::bitset<ExWindowStyle> style) noexcept
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
		graphics.drawEdge(client, forms::edgeFlags(exStyle), BorderFlags::Rect);

	else if (style.test(WindowStyle::Border)) {
		graphics.setPen(StockPen::Black);
		graphics.drawRect(client);
	}
}

LookNFeelProvider::LookNFeelProvider() 
  : Fonts{
    .Heading1{StockFont::DefaultGui},
    .Heading2{StockFont::DefaultGui},
    .Heading3{StockFont::DefaultGui},
    .Paragraph{StockFont::DefaultGui}
  },
  Colours{
    .Button{SystemColour::ButtonFace},
	.Caption{SystemColour::ActiveCaption, SystemColour::InactiveCaption},
	.Highlight{SystemColour::Highlight},
    .Primary{SystemColour::WindowText},
    .Secondary{SystemColour::GrayText},
	.Tertiary{SystemColour::GrayText},
    .Window{SystemColour::Window}
  }
{
}

LookNFeelProvider::LookNFeelProvider(SharedColourScheme alternateColours, SharedWindowFrame windowFrame) 
  : LookNFeelProvider{}
{
	this->ColourDecorator = alternateColours;
	this->FrameDecorator = windowFrame;
}

Font
LookNFeelProvider::makeDefault() const {
	return FontBuilder{}.withName(this->default().Name).withSize(this->default().Height);
}

Rect
LookNFeelProvider::clientArea(Window& wnd, Rect bounds) const
{	
	// Delegate if decorated
	if (this->FrameDecorator)
		return this->FrameDecorator->clientArea(wnd, bounds);

	Size const Frame{SystemMetric::cxSizeFrame, SystemMetric::cySizeFrame};
	Rect client = bounds - Border{2 * Frame} - Border{0, Measurement{SystemMetric::cyCaption}, 0, 0};
	if (wnd.menu().has_value())
		client.Top += Measurement{SystemMetric::cyMenu};
	return client;
}

bool
LookNFeelProvider::customFrame() const 
{
	// Delegate if decorated
	if (this->FrameDecorator)
		return this->FrameDecorator->customFrame();

	return false;
}

NonClientLayout
LookNFeelProvider::nonClientArea(Coords results, nstd::bitset<WindowStyle> style, Rect wnd) const 
{
	ThrowIf(results, results == Coords::Client);
	
	// Delegate if decorated
	if (this->FrameDecorator)
		return this->FrameDecorator->nonClientArea(results, style, wnd);

	// Caption
	NonClientLayout bounds{wnd};
	Size const Frame{SystemMetric::cxSizeFrame, SystemMetric::cySizeFrame};
	bounds.Caption = Rect{Point::Zero, Size{wnd.width(), Measurement{SystemMetric::cyCaption}}} 
	               - Border{2 * Frame.Width, win::Unused<LONG>}
	               + Point{win::Unused<LONG>, 2 * Frame.Height};
	
	// System-menu button
	Size const rcIcon {bounds.Caption.height(), bounds.Caption.height()};
	bounds.SysMenuBtn = Rect{bounds.Caption.topLeft(), rcIcon};

	// Close/Maximize/minimize buttons
	using enum WindowStyle;
	Rect const rightButtonCoords{bounds.Caption.topRight(), rcIcon, Rect::FromTopRight};
	Rect const middleButtonCoords = rightButtonCoords - Point{rcIcon.Width,0};
	Rect const leftButtonCoords = middleButtonCoords - Point{rcIcon.Width,0};
	bounds.CloseBtn = rightButtonCoords;
	bounds.MaximizeBtn = middleButtonCoords;
	bounds.MinimizeBtn = leftButtonCoords;
	
	// Title
	bounds.Title = bounds.Caption;
	bounds.Title.Left = bounds.SysMenuBtn.Right + (2 * Frame.Width);
	bounds.Title.Right = bounds.MinimizeBtn.Left - (2 * Frame.Width);

	// MenuBar
	bounds.MenuBar = Rect{bounds.Caption.Left, bounds.Caption.Bottom, bounds.Caption.Right, bounds.Caption.Bottom + Measurement{SystemMetric::cyMenu}};

	// [SCREEN] Translate all generated co-ordinates
	if (results == Coords::Screen) {
		bounds.Caption += wnd.topLeft();
		bounds.Title += wnd.topLeft();
		bounds.CloseBtn += wnd.topLeft();
		bounds.MenuBar += wnd.topLeft();
		bounds.SysMenuBtn += wnd.topLeft();
		bounds.MaximizeBtn += wnd.topLeft();
		bounds.MinimizeBtn += wnd.topLeft();
	}

	return bounds;
}

void
LookNFeelProvider::onWindowCreated(Window& wnd) const {
	// Delegate if decorated
	if (this->FrameDecorator)
		return this->FrameDecorator->onWindowCreated(wnd);
	
	// (Noop by default)
}

void
LookNFeelProvider::draw(ButtonControl& ctrl, OwnerDrawEventArgs& args) const
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
	args.Graphics.drawText(ctrl.text(), content, forms::drawTextFlags(ctrl.align()));

	// Draw focus rectangle
	auto const focused = state.test(ButtonState::Focus);
	if (focused)
		args.Graphics.drawFocus(content);

	args.Graphics.restore();
}

void
LookNFeelProvider::draw(CheckBoxControl& ctrl, OwnerDrawEventArgs& args) const
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"CheckBox #{} must be OwnerDraw", args.Ident};
	
	// Erase background
	args.Graphics.setBrush(ctrl.backColour());
	args.Graphics.fillRect(args.Item.Area);
	
	// Draw check
	auto const alignment = ctrl.align();
	auto const checked = ctrl.checked();
	Rect const content = args.Item.Area;
	scoped {
		Rect tick {content.topLeft(), Size{SystemMetric::cxSmallIcon,SystemMetric::cySmallIcon}};
		if (alignment.test(Alignment::VCentre))
			tick.translate(Point{0, (content.height() - tick.height()) / 2});
		args.Graphics.drawControl(tick, DFC_BUTTON, DFCS_BUTTONCHECK|(checked?DFCS_CHECKED:0));
	}
	
	// Draw text
	auto const enabled = ctrl.enabled();
	Rect const areaText = content - Border{SystemMetric::cxSmallIcon,0,0,0} - Border{SystemMetric::cxEdge,0,0,0};
	args.Graphics.setFont(ctrl.font());
	args.Graphics.textColour(enabled ? ctrl.textColour() : SystemColour::GrayText, ctrl.backColour());
	args.Graphics.drawText(ctrl.text(), areaText, forms::drawTextFlags(alignment));
	
	// Draw focus rectangle
	auto const focused = ctrl.state().test(ButtonState::Focus);
	if (focused)
		args.Graphics.drawFocus(content);

	args.Graphics.restore();
}

void
LookNFeelProvider::draw(ComboBoxControl& ctrl, OwnerDrawEventArgs& args) const
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
	Rect const rcBackground = args.Item.Area - Border{measureEdge(ctrl.exStyle()).Width};
	args.Graphics.setBrush(backColour);
	args.Graphics.fillRect(rcBackground);
	final_act(&) noexcept { 
		args.Graphics.restore();
	};
	
	//! @fix  Manually erase background of gap beneath bottom item
	if (ctrl.dropped() && args.Item.Index == ctrl.Items.size()-1) {
		Rect const rcBottom = {rcBackground.Left, rcBackground.Top, rcBackground.Right, ctrl.info().DroppedItemList.clientRect().Bottom};
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
	auto const detail = item.detail();
	Rect const rcContent = rcBackground - Border{2*Measurement{SystemMetric::cxEdge}, win::Unused<::LONG>};

	// [HEADING] Draw heading and calculate different rectangle for (multi-line) detail text
	Rect rcDetailText = rcContent;
	if (useHeadings) {
		Invariant(heading.has_value());
		
		// [HEADING-ICON] Draw icon on left; position both heading and detail text beside it
		Rect rcHeadingText = rcContent;
		if (useIcons) {
			Size const iconSize = withinEdit ? SmallIcon : BigIcon;
			Point const iconPosition = rcContent.topLeft();
			if (icon) {
				Point const iconOffset{win::Unused<::LONG>, (rcContent.height() - iconSize.Height) / 2};
				args.Graphics.drawIcon(*icon->handle(), iconPosition + iconOffset, iconSize);
			}

			// Offset heading/detail drawing rectangles
			LONG const horzOffset = iconSize.Width + 3*Measurement{SystemMetric::cxFixedFrame};
			rcHeadingText.Left += horzOffset;
			rcDetailText.Left += horzOffset;
		}
		
		// [EDIT] Draw edit contents using heading style. Forget the detail
		if (withinEdit) {
			// Prefer heading font + selected-text colour; fallback to heading-default then control-default
			args.Graphics.setFont(heading->Font.value_or(ctrl.headingFont().value_or(ctrl.font())));
			args.Graphics.textColour(selectedTextColour.value_or(heading->Colour.value_or(ctrl.textColour())), transparent);
			
			// [HEADING] Draw heading and abort
			args.Graphics.drawText(heading->Text, rcHeadingText, DrawTextFlags::SimpleLeft);
			return;
		}

		// Pre-calculate the detail size in order to calculate the heading offset
		Rect detailRequired = rcDetailText;
		args.Graphics.setFont(detail.Font.value_or(ctrl.font()));
		args.Graphics.textColour(selectedTextColour.value_or(detail.Colour.value_or(ctrl.textColour())), transparent);
		args.Graphics.calcRect(detail.Text, detailRequired, DrawTextFlags::Left|DrawTextFlags::WordBreak);

		// Prefer heading font + selected-text colour; fallback to heading-default then control-default
		args.Graphics.setFont(heading->Font.value_or(ctrl.headingFont().value_or(ctrl.font())));
		args.Graphics.textColour(selectedTextColour.value_or(heading->Colour.value_or(ctrl.textColour())), transparent);

		// Calculate the heading offset
		Rect headingRequired = rcDetailText;
		args.Graphics.calcRect(heading->Text, headingRequired, DrawTextFlags::Left);
		Size const headingOffset{win::Unused<LONG>, (rcContent.height() - headingRequired.height() - detailRequired.height()) / 2};
		rcHeadingText.Top += headingOffset.Height;
		
		// [HEADING] Draw heading (position directly above detail)
		auto const titleHeight = args.Graphics.drawText(heading->Text, rcHeadingText, DrawTextFlags::Left);
		rcDetailText.Top += headingOffset.Height + titleHeight;
	}
	
	// [DETAIL-ICON] Draw on left; position detail text to right
	if (useIcons && !useHeadings) {
		Size const iconSize = SmallIcon;
		if (icon) {
			Point const iconOffset{win::Unused<::LONG>, (rcContent.height() - iconSize.Height) / 2};
			args.Graphics.drawIcon(*icon->handle(), rcContent.topLeft() + iconOffset, iconSize);
		}
		rcDetailText.Left += iconSize.Width + 3*Measurement{SystemMetric::cxFixedFrame};
	}
	
	// Prefer detail font + selected-text colour; fallback to control-default
	args.Graphics.setFont(detail.Font.value_or(ctrl.font()));
	args.Graphics.textColour(selectedTextColour.value_or(detail.Colour.value_or(ctrl.textColour())), transparent);

	// [DETAIL] Detail can be multi-line when drawing an OD-variable ListBox item; but they are vertically centred when OD-fixed
	if (bool const variableHeight = ctrl.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable); !variableHeight)
		args.Graphics.drawText(detail.Text, rcDetailText, DrawTextFlags::SimpleLeft);
	else {
		Rect required = rcDetailText;
		args.Graphics.calcRect(detail.Text, required, DrawTextFlags::Left|DrawTextFlags::WordBreak);
		rcDetailText.Top += (rcDetailText.height() - required.height()) / 2;
		args.Graphics.drawText(detail.Text, rcDetailText, DrawTextFlags::Left|DrawTextFlags::WordBreak);
	}
}

void
LookNFeelProvider::measure(ComboBoxControl& ctrl, MeasureItemEventArgs& args) const
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"ComboBox #{} must be OwnerDraw", args.Ident};
	
	Size constexpr  static BigIcon{48,48};
	Size constexpr  static SmallIcon{24,24};

	bool const useHeadings = ctrl.features().test(ComboBoxFeature::Headings);
	bool const useIcons = ctrl.features().test(ComboBoxFeature::Icons);

	// [EDIT] Return greater of the edit-font height or icon height
	if (args.Item.Index == MeasureItemEventArgs::EditControl) {
		//! @remarks WM_MEASUREITEM is received extremely early in the process of ComboBox construction
		//!           so it's not even feasible to rely on its window font because the control hasn't yet
		//!           received its first WM_SETFONT
		auto const fontHeight = ctrl.editFont().height();
		args.Item.Height = std::abs(fontHeight) + 2*Measurement{SystemMetric::cyFixedFrame};
		if (useIcons)
			args.Item.Height = std::max<LONG>(args.Item.Height, useHeadings ? BigIcon.Height : SmallIcon.Height);
		return;
	}

	// [FIXED-HEIGHT] Return greater of the detail height, icon height, or user-requested item height.
	//                 Font measured is 
	if (ctrl.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawFixed)) {
		args.Graphics.setFont(ctrl.font());
		args.Item.Height = args.Graphics.measureText(L"Wjgy").Height;
		if (useIcons)
			args.Item.Height = std::max<LONG>(args.Item.Height, SmallIcon.Height);
		args.Item.Height = std::max<LONG>(args.Item.Height, ctrl.Items.height());
	}
	// [VARIABLE-HEIGHT] Calculate per-item height
	else {
		Invariant(args.Item.UserData != NULL);
		auto const& item = *reinterpret_cast<ComboBoxControl::TemporaryMeasureItemData*>(args.Item.UserData);
		args.Item.Height = 0;
		
		// [HEADING] Prefer heading font; fallback to heading-default then control-default
		if (useHeadings) {
			auto const heading = item.Heading; 
			Invariant(heading.has_value());
			args.Graphics.setFont(heading->Font.value_or(ctrl.headingFont().value_or(ctrl.font())));
			args.Item.Height += args.Graphics.measureText(heading->Text).Height;
		}

		// Prefer detail font; fallback to control-default
		auto const detail = item.Detail;
		args.Graphics.setFont(detail.Font.value_or(ctrl.font()));
		
		// Its difficult to measure the dropped rectangle width because the scrollbar is optional :/
		Rect rcDetailText = ctrl.droppedRect() - Border{SystemMetric::cxFixedFrame, 0} - Border{0,0,0,SystemMetric::cxHScroll};

		// Detail may be offset by icon
		if (useIcons)
			rcDetailText.Left += (useHeadings ? BigIcon : SmallIcon).Width + 3*Measurement{SystemMetric::cxFixedFrame};

		// Measure multiline height
		args.Item.Height += args.Graphics.measureText(detail.Text, Size{rcDetailText.width(),1}).Height;
		
		// [NO-HEADING] Add small gap between items
		if (!useHeadings) 
			args.Item.Height += 2*Measurement{SystemMetric::cyFixedFrame};

		// Return greater of combined height or icon height
		if (useIcons)
			args.Item.Height = std::max<LONG>(args.Item.Height, (useHeadings ? BigIcon : SmallIcon).Height);
	}

	args.Graphics.restore();
}

void
LookNFeelProvider::erase(GroupBoxControl& ctrl, EraseBackgroundEventArgs& args) const
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
LookNFeelProvider::draw(LabelControl& ctrl, OwnerDrawEventArgs& args) const
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"Label #{} must be OwnerDraw", args.Ident};
	
	// Erase background
	args.Graphics.setBrush(ctrl.backColour());
	args.Graphics.fillRect(args.Item.Area);

	// Draw text
	args.Graphics.setFont(ctrl.font());
	args.Graphics.textColour(ctrl.textColour(), ctrl.backColour());
	nstd::bitset<DrawTextFlags> flags = forms::drawTextFlags(ctrl.align());
	if (flags.test(DrawTextFlags::Bottom)) 
		flags.set(DrawTextFlags::SingleLine, true);
	else if (flags.test(DrawTextFlags::VCentre)) {
		flags.set(DrawTextFlags::WordBreak, true);
		flags.set(DrawTextFlags::SingleLine, false);
	}
	args.Graphics.drawText(ctrl.text(), args.Item.Area, flags);

	args.Graphics.restore();
}

void
LookNFeelProvider::draw(ListBoxControl& ctrl, OwnerDrawEventArgs& args) const
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"ListBox #{} must be OwnerDraw", args.Ident};

	Size constexpr  static BigIcon{48,48};
	Size constexpr  static SmallIcon{24,24};

	// Query basic item state
	bool const selected = args.Item.State.test(OwnerDrawState::Selected);
	auto const backColour = selected ? this->highlight() : ctrl.backColour();
	
	// Draw item background
	Rect const rcBackground = args.Item.Area - Border{measureEdge(ctrl.exStyle()).Width};
	args.Graphics.setBrush(backColour);
	args.Graphics.fillRect(rcBackground);
	final_act(&) noexcept { 
		args.Graphics.restore();
	};
	
	// [NO-ITEMS] Return early
	if (args.Item.Index == args.Empty) 
		return;
	
	// Query custom features
	auto const item = ctrl.Items[args.Item.Index];
	auto const selectedTextColour = nstd::make_optional_if<AnyColour>(selected, SystemColour::HighlightText);
	bool const useHeadings = ctrl.features().test(ListBoxFeature::Headings);
	bool const useBigIcons = ctrl.features().test(ListBoxFeature::BigIcons);
	bool const useIcons = useBigIcons || ctrl.features().test(ListBoxFeature::Icons);
	auto const heading = item.heading(); 
	auto const icon = item.icon();
	auto const detail = item.detail();
	Rect const rcContent = rcBackground - Border{2*Measurement{SystemMetric::cxEdge}, win::Unused<::LONG>};

	// [HEADING] Draw heading and calculate different rectangle for (multi-line) detail text
	Rect rcDetailText = rcContent;
	if (useHeadings) {
		Invariant(heading.has_value());

		// [HEADING-ICON] Draw icon on left; position both heading and detail text beside it
		Rect rcHeadingText = rcContent;
		if (useIcons) {
			Size const iconSize = BigIcon;
			Point const iconPosition = rcContent.topLeft();
			if (icon) {
				Point const iconOffset{win::Unused<::LONG>, (rcContent.height() - iconSize.Height) / 2};
				args.Graphics.drawIcon(*icon->handle(), iconPosition + iconOffset, iconSize);
			}

			// Offset heading/detail drawing rectangles
			LONG const horzOffset = iconSize.Width + 3*Measurement{SystemMetric::cxFixedFrame};
			rcHeadingText.Left += horzOffset;
			rcDetailText.Left += horzOffset;
		}
		
		// Pre-calculate the detail size in order to calculate the heading offset
		Rect detailRequired = rcDetailText;
		args.Graphics.setFont(detail.Font.value_or(ctrl.font()));
		args.Graphics.calcRect(detail.Text, detailRequired, DrawTextFlags::Left|DrawTextFlags::WordBreak);
		
		// Prefer heading font + selected-text colour; fallback to heading-default then control-default
		args.Graphics.setFont(heading->Font.value_or(ctrl.headingFont().value_or(ctrl.font())));
		args.Graphics.textColour(selectedTextColour.value_or(heading->Colour.value_or(ctrl.textColour())), transparent);
		
		// Calculate the heading offset
		Rect headingRequired = rcDetailText;
		args.Graphics.calcRect(heading->Text, headingRequired, DrawTextFlags::Left);
		Size const headingOffset{win::Unused<LONG>, (rcContent.height() - headingRequired.height() - detailRequired.height()) / 2};
		rcHeadingText.Top += headingOffset.Height;
		
		// Draw heading and offset detail below
		auto const titleHeight = args.Graphics.drawText(heading->Text, rcHeadingText, DrawTextFlags::Left);
		rcDetailText.Top += titleHeight;
	}
	
	// [DETAIL-ICON] Draw on left; position detail text to right
	if (useIcons && !useHeadings) {
		Size const iconSize = (useBigIcons ? BigIcon : SmallIcon);
		if (icon) {
			Point const iconOffset{win::Unused<::LONG>, (rcContent.height() - iconSize.Height) / 2};
			args.Graphics.drawIcon(*icon->handle(), rcContent.topLeft() + iconOffset, iconSize);
		}
		rcDetailText.Left += iconSize.Width + 3*Measurement{SystemMetric::cxFixedFrame};
	}
	
	// Prefer detail font + selected-text colour; fallback to control-default
	args.Graphics.setFont(detail.Font.value_or(ctrl.font()));
	args.Graphics.textColour(selectedTextColour.value_or(detail.Colour.value_or(ctrl.textColour())), transparent);

	// [DETAIL] Detail can be multi-line when drawing an OD-variable ListBox item; but they are vertically centred when OD-fixed
	if (bool const variableHeight = ctrl.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable); !variableHeight)
		args.Graphics.drawText(detail.Text, rcDetailText, DrawTextFlags::SimpleLeft);
	else {
		Rect required = rcDetailText;
		args.Graphics.calcRect(detail.Text, required, DrawTextFlags::Left|DrawTextFlags::WordBreak);
		rcDetailText.Top += (rcDetailText.height() - required.height()) / 2;
		args.Graphics.drawText(detail.Text, rcDetailText, DrawTextFlags::Left|DrawTextFlags::WordBreak);
	}
}

void
LookNFeelProvider::measure(ListBoxControl& ctrl, MeasureItemEventArgs& args) const
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"ListBox #{} must be OwnerDraw", args.Ident};
	
	Size constexpr  static BigIcon{48,48};
	Size constexpr  static SmallIcon{24,24};

	bool const useHeadings = ctrl.features().test(ListBoxFeature::Headings);
	bool const useBigIcons = ctrl.features().test(ListBoxFeature::BigIcons);
	bool const useIcons = useBigIcons || ctrl.features().test(ListBoxFeature::Icons);

	// [FIXED-HEIGHT] Return greater of the detail height, icon height, or user-requested item height.
	//                 Font measured is 
	if (ctrl.style<ListBoxStyle>().test(ListBoxStyle::OwnerDrawFixed)) {
		args.Graphics.setFont(ctrl.font());
		args.Item.Height = args.Graphics.measureText(L"Wjgy").Height;
		if (useIcons)
			args.Item.Height = std::max<LONG>(args.Item.Height, (useBigIcons ? BigIcon : SmallIcon).Height);
		args.Item.Height = std::max<LONG>(args.Item.Height, ctrl.Items.height());
	}
	// [VARIABLE-HEIGHT] Calculate per-item height
	else {
		Invariant(args.Item.UserData != NULL);
		auto const& item = *reinterpret_cast<ListBoxControl::TemporaryMeasureItemData*>(args.Item.UserData);
		args.Item.Height = 0;
		
		// [HEADING] Prefer heading font; fallback to heading-default then control-default
		if (useHeadings) {
			auto const heading = item.Heading; 
			Invariant(heading.has_value());
			args.Graphics.setFont(heading->Font.value_or(ctrl.headingFont().value_or(ctrl.font())));
			args.Item.Height += args.Graphics.measureText(heading->Text).Height;
		}

		// Prefer detail font; fallback to control-default
		auto const detail = item.Detail;
		args.Graphics.setFont(detail.Font.value_or(ctrl.font()));
		
		// Its difficult to measure the dropped rectangle width because the scrollbar is optional :/
		Rect rcDetailText = ctrl.clientRect() - Border{SystemMetric::cxFixedFrame, 0} - Border{0,0,0,SystemMetric::cxHScroll};

		// Detail may be offset by icon
		if (useIcons)
			rcDetailText.Left += (useHeadings || useBigIcons ? BigIcon : SmallIcon).Width + 3*Measurement{SystemMetric::cxFixedFrame};

		// Measure multiline height
		args.Item.Height += args.Graphics.measureText(detail.Text, Size{rcDetailText.width(),1}).Height;
		
		// [NO-HEADING] Add small gap between items
		if (!useHeadings) 
			args.Item.Height += 2*Measurement{SystemMetric::cyFixedFrame};

		// Return greater of combined height or icon height
		if (useIcons)
			args.Item.Height = std::max<LONG>(args.Item.Height, (useBigIcons ? BigIcon : SmallIcon).Height);
	}

	args.Graphics.restore();
}

void
LookNFeelProvider::erase(ListBoxControl& ctrl, EraseBackgroundEventArgs& args) const
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
LookNFeelProvider::draw(ListViewControl& ctrl, OwnerDrawEventArgs& args) const
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"ListView #{} must be OwnerDraw", args.Ident};
	
	// Query basic item state
	bool const selected = args.Item.State.test(OwnerDrawState::Selected);
	auto const selectedTextColour = nstd::make_optional_if<AnyColour>(selected, SystemColour::HighlightText);
	
	// Prefer item background colour (unless selected); fallback to control-default
	auto const item = ctrl.Items[args.Item.Index];
	auto const backColour = selected ? this->highlight() : item.backColour().value_or(ctrl.backColour());

	// Draw item background
	Rect const rcLine = args.Item.Area - Border{measureEdge(ctrl.exStyle()).Width};
	args.Graphics.setBrush(backColour);
	args.Graphics.fillRect(rcLine);
	final_act(&) noexcept { 
		args.Graphics.restore();
	};
	
	// [ICON]
	Rect rcLabel = item.area();
	if (ctrl.features().test(ListViewFeature::Icons)) {
		if (std::optional<Icon> icon = item.icon(); icon.has_value()) 
			args.Graphics.drawIcon(*icon->handle(), rcLabel.topLeft(), Size{24,24});

		rcLabel.Left += 24 + 3*Measurement{SystemMetric::cxEdge};
	}

	// [ITEM] Prefer item font + selected-text colour; fallback to control-default
	RichText const detail = item.detail();
	args.Graphics.setFont(detail.Font.value_or(ctrl.font()));
	args.Graphics.textColour(selectedTextColour.value_or(detail.Colour.value_or(ctrl.textColour())), transparent);
	args.Graphics.drawText(detail.Text, rcLabel);

	// [SUB-ITEMS] Prefer subitem font + selected-text colour; item font/colour; fallback to control-default
	for (auto idx = 0, subItemCount = item.SubItems.size(); idx < subItemCount; ++idx) {
		auto const subitem = item.SubItems[idx];
		RichText const subDetail = subitem.detail();
		args.Graphics.setFont(subDetail.Font.value_or(detail.Font.value_or(ctrl.font())));
		args.Graphics.textColour(selectedTextColour.value_or(subDetail.Colour.value_or(detail.Colour.value_or(ctrl.textColour()))), transparent);
		args.Graphics.drawText(subDetail.Text, subitem.area(), DrawTextFlags::SimpleLeft|DrawTextFlags::WordElipsis);
	}
}

void
LookNFeelProvider::measure(ListViewControl& ctrl, MeasureItemEventArgs& args) const
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"ListView #{} must be OwnerDraw", args.Ident};

	args.Graphics.setFont(ctrl.font());
	args.Item.Height = args.Graphics.measureText(L"Ajy").Height;

	if (ctrl.features().test(ListViewFeature::Icons))
		args.Item.Height = std::max<uint32_t>(args.Item.Height, 24);
	
	args.Graphics.restore();
}

void
LookNFeelProvider::erase(ListViewControl& ctrl, EraseBackgroundEventArgs& args) const
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
LookNFeelProvider::draw(PictureControl& ctrl, OwnerDrawEventArgs& args) const
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"Picture #{} must be OwnerDraw", args.Ident};
	
	// Query properties
	bool const DontResize = ctrl.style<StaticStyle>().test(StaticStyle::RealSizeImage);

	// Picture can be either bitmap or icon
	if (auto bitmap = ctrl.image(); bitmap) {
		DeviceContext memory = DeviceContext::create(args.Graphics.handle());
		memory.setBitmap(bitmap->handle());

		// Optionally stretch the image into the client area
		Rect const rcDest = !DontResize ? args.Item.Area : Rect{args.Item.Area.topLeft(), bitmap->size()};
		args.Graphics.copyBitmap(memory.handle(), bitmap->depth(), bitmap->rect(), rcDest, ctrl.opacity());

		memory.restore();
	}
	else if (auto icon = ctrl.icon(); icon) {
		// Optionally stretch the icon into the client area
		Rect const rcDest = !DontResize ? args.Item.Area : Rect{args.Item.Area.topLeft(), icon->size()};
		args.Graphics.drawIcon(*icon->handle(), rcDest);
	}
}

void
LookNFeelProvider::draw(RadioButtonControl& ctrl, OwnerDrawEventArgs& args) const
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
	args.Graphics.drawText(ctrl.text(), areaText, forms::drawTextFlags(ctrl.align()));

	// Draw focus rectangle
	if (focused)
		args.Graphics.drawFocus(content);

	args.Graphics.restore();
}

void
LookNFeelProvider::draw(StaticControl& ctrl, OwnerDrawEventArgs& args) const
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"Static #{} must be OwnerDraw", args.Ident};

	args.Graphics.setBrush(ctrl.backColour());
	args.Graphics.drawText(ctrl.text(), args.Item.Area, forms::drawTextFlags(ctrl.align()));
	
	args.Graphics.restore();
}

Response
LookNFeelProvider::draw(Dialog& dlg, NonClientPaintEventArgs& args) const
{
	// Use Windows default
	return Window::Unhandled;
}

void
LookNFeelProvider::draw(Window& wnd, OwnerDrawMenuEventArgs& args) const
{
	Invariant(args.Item.data<Menu::ItemData>() != nullptr);
	auto const& item = *args.Item.data<Menu::ItemData>();
	
	// When using a custom non-client area, the menu-bar is drawn as part of the non-client paint cycle
	if (this->customFrame() && item.IsTopLevel) 
		return;

	auto const& detail = item.Detail;
	auto const isSelected = args.Item.State.test(OwnerDrawState::Selected);
	auto const isDisabled = args.Item.State.test(OwnerDrawState::Disabled|OwnerDrawState::Grayed);
	auto const isSeparator = detail.Text.empty();
	auto const backColour = isSelected && !isSeparator && !isDisabled ? this->highlight() : wnd.backColour();
	auto const disabledTextColour = nstd::make_optional_if<AnyColour>(isDisabled, this->tertiary());
	auto const selectedTextColour = nstd::make_optional_if<AnyColour>(isSelected, wnd.backColour());

	// Background
	args.Graphics.setBrush(backColour);
	args.Graphics.fillRect(args.Item.Area);

	// Prefer item font/colour; fallback to window font/colour
	if (isSeparator) {
		Size constexpr HugeBorder{16,win::Unused<LONG>};
		args.Graphics.drawEdge(args.Item.Area - Border{HugeBorder.Width, 0, HugeBorder.Width, args.Item.Area.height() / 2}, EdgeFlags::Sunken, BorderFlags::Bottom);
	}
	else {
		args.Graphics.setFont(detail.Font.value_or(wnd.font()));
		args.Graphics.textColour(disabledTextColour.value_or(selectedTextColour.value_or(detail.Colour.value_or(wnd.textColour()))));
		
		// [TOP-LEVEL]
		if (item.IsTopLevel)
			args.Graphics.drawText(detail.Text, args.Item.Area, DrawTextFlags::SimpleCentre);
		else {
			Size constexpr SmallIcons{24,24};
			Rect const GutterOffset{SmallIcons.Width + 3*Measurement{SystemMetric::cxEdge},0,0,0};
			args.Graphics.drawText(detail.Text, args.Item.Area + GutterOffset, DrawTextFlags::SimpleLeft|DrawTextFlags::ExpandTabs);
		}
	}

	args.Graphics.restore();
}

void
LookNFeelProvider::measure(Window& wnd, MeasureMenuEventArgs& args) const
{
	Invariant(args.Item.data<Menu::ItemData>() != nullptr);
	auto const& item = *args.Item.data<Menu::ItemData>();
	auto const& detail = item.Detail;
	auto const isSeparator = detail.Text.empty();
	
	// Use fixed-height for separator items
	if (isSeparator) {
		Size constexpr Separator{win::Unused<LONG>, 8};
		args.Item.Height = Separator.Height;
	}
	else {
		// Prefer item font; fallback to window font
		args.Graphics.setFont(detail.Font.value_or(wnd.font()));
		auto size = args.Graphics.measureText(boost::replace_all_copy(detail.Text, L"\t", L"        "));
		
		//! @fix  This message doesn't seem to be sent for the majority of system-menu items so the measurements
		//!       are incorrect overall. Compensate for this by falsely reporting the widest menu element when
		//!       we detect any system-menu item is being measured
		if (args.Item.Ident == SC_MOVE || args.Item.Ident == SC_SIZE) {
			auto const minSize = args.Graphics.measureText(L"Close    Alt+F4");
			size.Width = std::max(size.Width, minSize.Width);
		}

		// [TOP-LEVEL] Cannot contain icons or shortcuts
		if (item.IsTopLevel) {
			args.Item.Height = std::max<uint32_t>(args.Item.Height, size.Height);
			args.Item.Width = 4*Measurement{SystemMetric::cxEdge} + size.Width;
		}
		// [SUB-MENU] Account for both edges, icon gutter, divider gap, and text
		else {
			Size constexpr SmallIcons{24,24};
			args.Item.Height = std::max<uint32_t>(SmallIcons.Height, size.Height);
			args.Item.Width = 6*Measurement{SystemMetric::cxEdge} + SmallIcons.Width + size.Width;
		}
	}

	args.Graphics.restore();
}

AnyColour
LookNFeelProvider::button() const {
	if (this->ColourDecorator)
		return this->ColourDecorator->button();

	return this->Colours.Button;
}

LookNFeelProvider::AnyColourPair
LookNFeelProvider::caption() const {
	if (this->ColourDecorator)
		return this->ColourDecorator->caption();

	return this->Colours.Caption;
}

AnyColour
LookNFeelProvider::control() const {
	if (this->ColourDecorator)
		return this->ColourDecorator->control();

	return this->Colours.Control;
}

Font
LookNFeelProvider::heading1() const {
	if (this->ColourDecorator)
		return this->ColourDecorator->heading1();

	return this->Fonts.Heading1;
}

Font
LookNFeelProvider::heading2() const {
	if (this->ColourDecorator)
		return this->ColourDecorator->heading2();

	return this->Fonts.Heading2;
}

Font
LookNFeelProvider::heading3() const {
	if (this->ColourDecorator)
		return this->ColourDecorator->heading3();

	return this->Fonts.Heading3;
}

AnyColour
LookNFeelProvider::highlight() const {
	if (this->ColourDecorator)
		return this->ColourDecorator->highlight();

	return this->Colours.Highlight;
}

Font
LookNFeelProvider::paragraph() const {
	if (this->ColourDecorator)
		return this->ColourDecorator->paragraph();

	return this->Fonts.Paragraph;
}

AnyColour
LookNFeelProvider::primary() const {
	if (this->ColourDecorator)
		return this->ColourDecorator->primary();

	return this->Colours.Primary;
}

AnyColour
LookNFeelProvider::secondary() const {
	if (this->ColourDecorator)
		return this->ColourDecorator->secondary();

	return this->Colours.Secondary;
}

AnyColour
LookNFeelProvider::tertiary() const {
	if (this->ColourDecorator)
		return this->ColourDecorator->tertiary();

	return this->Colours.Tertiary;
}

AnyColour
LookNFeelProvider::window() const {
	if (this->ColourDecorator)
		return this->ColourDecorator->window();

	return this->Colours.Window;
}
