#include "lookNfeel/LookNFeelProvider.h"
#include "controls/CommonControls.h"
#include "graphics/FontBuilder.h"
using namespace core;
using namespace forms;

DrawTextFlags
forms::drawFlags(nstd::bitset<Alignment> align) noexcept
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

LookNFeelProvider::LookNFeelProvider(SharedLookNFeelProvider impl) 
	: LookNFeelProvider{}
{
	this->CustomImpl = impl;
}

bool
LookNFeelProvider::customCaption() const {
	if (CustomImpl)
		return (*CustomImpl)->customCaption();

	return false;
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
	args.Graphics.drawText(ctrl.text(), content, forms::drawFlags(ctrl.align()));

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
	args.Graphics.drawText(ctrl.text(), areaText, forms::drawFlags(alignment));
	
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
	
	//! @fix  Manually erase background of gap beneath bottom item
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
			if (icon)
				args.Graphics.drawIcon(icon->handle(), iconPosition, iconSize);

			// Offset heading/detail drawing rectangles
			LONG const horzOffset = iconSize.Width + 3*Measurement{SystemMetric::cxFixedFrame};
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
		if (icon)
			args.Graphics.drawIcon(icon->handle(), rcContent.topLeft(), iconSize);
		rcDetailText.Left += iconSize.Width + 3*Measurement{SystemMetric::cxFixedFrame};
	}
	
	// Prefer detail font + selected-text colour; fallback to control-default
	auto const detail = item.detail();
	args.Graphics.setFont(detail.Font.value_or(ctrl.font()));
	args.Graphics.textColour(selectedTextColour.value_or(detail.Colour.value_or(ctrl.textColour())), transparent);

	// [DETAIL] Detail can be multi-line when drawing an OD-variable ListBox item; but they are
	//              vertically centred when OD-fixed (or item is being drawn within the ComboBox/Edit)
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
	args.Graphics.setFont(ctrl.font());
	args.Graphics.textColour(ctrl.textColour(), ctrl.backColour());
	args.Graphics.drawText(ctrl.text(), args.Item.Area, forms::drawFlags(ctrl.align()));

	args.Graphics.restore();
}

void
LookNFeelProvider::draw(ListBoxControl& ctrl, OwnerDrawEventArgs const& args) 
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"ListBox #{} must be OwnerDraw", args.Ident};

	Size constexpr  static BigIcon{48,48};
	Size constexpr  static SmallIcon{24,24};

	// Query basic item state
	bool const selected = args.Item.State.test(OwnerDrawState::Selected);
	auto const backColour = selected ? this->highlight() : ctrl.backColour();
	
	// Draw item background
	Rect const rcContent = args.Item.Area - Border{measureEdge(ctrl.exStyle()).Width};
	args.Graphics.setBrush(backColour);
	args.Graphics.fillRect(rcContent);
	final_act(&) noexcept { 
		args.Graphics.restore();
	};
	
	// [NO-ITEMS] Return early
	if (args.Item.Index == args.Empty) 
		return;
	
	// Query custom features
	auto const item = ctrl.Items[args.Item.Index];
	auto const selectedTextColour = nstd::make_optional_if<AnyColour>(selected, SystemColour::HighlightText);
	bool const useHeadings = ctrl.features().test(ComboBoxFeature::Headings);
	bool const useBigIcons = ctrl.features().test(ListBoxFeature::BigIcons);
	bool const useIcons = useBigIcons || ctrl.features().test(ComboBoxFeature::Icons);
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
			Size const iconSize = BigIcon;
			Point const iconPosition = rcContent.topLeft();
			if (icon)
				args.Graphics.drawIcon(icon->handle(), iconPosition, iconSize);

			// Offset heading/detail drawing rectangles
			LONG const horzOffset = iconSize.Width + 3*Measurement{SystemMetric::cxFixedFrame};
			rcHeadingText.Left += horzOffset;
			rcDetailText.Left += horzOffset;
		}

		// Draw heading and offset detail below
		auto const titleHeight = args.Graphics.drawText(heading->Text, rcHeadingText, DrawTextFlags::Left);
		rcDetailText += Point{0, titleHeight};
	}
	
	// [DETAIL-ICON] Draw on left; position detail text to right
	if (useIcons && !useHeadings) {
		Size const iconSize = (useBigIcons ? BigIcon : SmallIcon);
		if (icon)
			args.Graphics.drawIcon(icon->handle(), rcContent.topLeft(), iconSize);
		rcDetailText.Left += iconSize.Width + 3*Measurement{SystemMetric::cxFixedFrame};
	}
	
	// Prefer detail font + selected-text colour; fallback to control-default
	auto const detail = item.detail();
	args.Graphics.setFont(detail.Font.value_or(ctrl.font()));
	args.Graphics.textColour(selectedTextColour.value_or(detail.Colour.value_or(ctrl.textColour())), transparent);

	// [DETAIL] Detail can be multi-line when drawing an OD-variable ListBox item; but they are
	//              vertically centred when OD-fixed
	DrawTextFlags constexpr  static MultilineFlags = DrawTextFlags::Left|DrawTextFlags::WordBreak;
	DrawTextFlags constexpr  static SinglelineFlags = DrawTextFlags::SimpleLeft;
	bool const odVariable = ctrl.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable);
	auto const flags = odVariable ? MultilineFlags : SinglelineFlags;

	args.Graphics.drawText(detail.Text, rcDetailText, flags);
}

void
LookNFeelProvider::measure(ListBoxControl& ctrl, MeasureItemEventArgs const& args)
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
LookNFeelProvider::draw(ListViewControl& ctrl, OwnerDrawEventArgs const& args) 
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
			args.Graphics.drawIcon(icon->handle(), rcLabel.topLeft(), Size{24,24});

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
LookNFeelProvider::measure(ListViewControl& ctrl, MeasureItemEventArgs const& args)
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
LookNFeelProvider::erase(ListViewControl& ctrl, EraseBackgroundEventArgs const& args) 
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
	args.Graphics.drawText(ctrl.text(), areaText, forms::drawFlags(ctrl.align()));

	// Draw focus rectangle
	if (focused)
		args.Graphics.drawFocus(content);

	args.Graphics.restore();
}

void
LookNFeelProvider::draw(StaticControl& ctrl, OwnerDrawEventArgs const& args)
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"Static #{} must be OwnerDraw", args.Ident};

	args.Graphics.setBrush(ctrl.backColour());
	args.Graphics.drawText(ctrl.text(), args.Item.Area, forms::drawFlags(ctrl.align()));
	
	args.Graphics.restore();
}

Response
LookNFeelProvider::draw(Window& wnd, NonClientPaintEventArgs& args) 
{
	// Use Windows default
	return Window::Unhandled;
}

void
LookNFeelProvider::draw(Window& wnd, OwnerDrawMenuEventArgs& args)
{
	Invariant(args.Item.data<Menu::ItemData>() != nullptr);
	auto const& detail = args.Item.data<Menu::ItemData>()->Detail;
	auto const isSelected = args.Item.State.test(OwnerDrawState::Selected);
	auto const isSeparator = detail.Text.empty();
	auto const backColour = isSelected && !isSeparator ? this->highlight() : wnd.backColour();
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
		args.Graphics.textColour(selectedTextColour.value_or(detail.Colour.value_or(wnd.textColour())));
		Size constexpr SmallIcons{24,24};
		args.Graphics.drawText(detail.Text, args.Item.Area + Rect{SmallIcons.Width + 3*Measurement{SystemMetric::cxEdge},0,0,0});
	}

	args.Graphics.restore();
}

void
LookNFeelProvider::measure(Window& wnd, MeasureMenuEventArgs& args)
{
	Invariant(args.Item.data<Menu::ItemData>() != nullptr);
	auto const& detail = args.Item.data<Menu::ItemData>()->Detail;
	
	// Use fixed-height for separator items
	if (detail.Text.empty()) {
		Size constexpr Separator{win::Unused<LONG>, 8};
		args.Item.Height = Separator.Height;
	}
	else {
		// Prefer item font; fallback to window font
		args.Graphics.setFont(detail.Font.value_or(wnd.font()));
		auto const size = args.Graphics.measureText(detail.Text);

		// Account for icon gutter and add significant gap on right-hand side
		Size constexpr SmallIcons{24,24};
		args.Item.Height = std::max<uint32_t>(SmallIcons.Height, size.Height);
		args.Item.Width = 5*Measurement{SystemMetric::cxEdge} + SmallIcons.Width + 2*size.Width;
	}

	args.Graphics.restore();
}

void
LookNFeelProvider::onCreated(Window&, CreateWindowEventArgs const& args) {
	// nothing
}

AnyColour
LookNFeelProvider::button() {
	if (CustomImpl)
		return (*CustomImpl)->button();

	return this->Colours.Button;
}

LookNFeelProvider::AnyColourPair
LookNFeelProvider::caption() {
	if (CustomImpl)
		return (*CustomImpl)->caption();

	return this->Colours.Caption;
}

AnyColour
LookNFeelProvider::control() {
	if (CustomImpl)
		return (*CustomImpl)->control();

	return this->Colours.Control;
}

Font
LookNFeelProvider::heading1() {
	if (CustomImpl)
		return (*CustomImpl)->heading1();

	return this->Fonts.Heading1;
}

Font
LookNFeelProvider::heading2() {
	if (CustomImpl)
		return (*CustomImpl)->heading2();

	return this->Fonts.Heading2;
}

AnyColour
LookNFeelProvider::highlight() {
	if (CustomImpl)
		return (*CustomImpl)->highlight();

	return this->Colours.Highlight;
}

Font
LookNFeelProvider::paragraph() {
	if (CustomImpl)
		return (*CustomImpl)->paragraph();

	return this->Fonts.Paragraph;
}

AnyColour
LookNFeelProvider::primary() {
	if (CustomImpl)
		return (*CustomImpl)->primary();

	return this->Colours.Primary;
}

AnyColour
LookNFeelProvider::secondary() {
	if (CustomImpl)
		return (*CustomImpl)->secondary();

	return this->Colours.Secondary;
}

AnyColour
LookNFeelProvider::tertiary() {
	if (CustomImpl)
		return (*CustomImpl)->tertiary();

	return this->Colours.Tertiary;
}

AnyColour
LookNFeelProvider::window() {
	if (CustomImpl)
		return (*CustomImpl)->window();

	return this->Colours.Window;
}
