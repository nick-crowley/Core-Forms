#include "lookNfeel/LookNFeelProvider.h"
#include "controls/CommonControls.h"
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
    .Primary{SystemColour::WindowText},
    .Secondary{SystemColour::GrayText},
    .Window{SystemColour::Window}
  }
{
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

	bool const variable = ctrl.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable);
	bool const selected = args.Item.State.test(OwnerDrawState::Selected);
	auto const backColour = selected ? SystemColour::Highlight : ctrl.backColour();
	auto const chooseTextColour = [&](std::optional<AnyColour> const& itemColour) -> AnyColour {
		if (selected)
			return SystemColour::HighlightText;
		return itemColour ? *itemColour : ctrl.textColour();
	};
	
	// Draw item background
	Rect const rcItem = args.Item.Area - Border{measureEdge(ctrl.exStyle()).Width};
	args.Graphics.setBrush(backColour);
	args.Graphics.fillRect(rcItem);

	// Setup item
	auto const item = ctrl.Items[args.Item.Index];
	auto const detail = item.detail();
	Rect rcDetail = rcItem;

	// [ALIGNMENT] Item detail may be multi-line when using OD-variable mode; otherwise they're v-centred
	auto const flagsDetail = variable ? DrawTextFlags::Left|DrawTextFlags::WordBreak : DrawTextFlags::SimpleLeft;

	// [TITLE] Draw title and calculate different rectangle for (multi-line) detail text
	auto const title = item.heading(); 
	auto const icon = item.icon();
	if (title)
	{
		args.Graphics.setFont(title->Font.value_or(ctrl.titleFont().value_or(ctrl.font())));
		args.Graphics.textColour(chooseTextColour(title->Colour), backColour);
		
		// [ICON] Draw icon on left; position both title and detail text beside it
		if (icon)
		{
			Size const iconSize{rcItem.height(), rcItem.height()};
			Rect const rcTitle = rcItem + Point{Percentage{115,unconstrained}*iconSize.Width,0};
			args.Graphics.drawIcon(icon->handle(), rcItem.topLeft(), iconSize);
			auto const titleHeight = args.Graphics.drawText(title->Text, rcTitle, DrawTextFlags::Left);
			rcDetail = rcTitle + Point{0, titleHeight};
		}
		// [TITLE-ONLY] Draw title; position detail text directly below
		else {
			auto const titleHeight = args.Graphics.drawText(title->Text, rcItem, DrawTextFlags::Left);
			rcDetail = rcItem + Point{0, titleHeight};
		}
	}

	// [TEXT] Draw using custom font/colour, if any; otherwise use ComboBox colours
	args.Graphics.setFont(detail.Font.value_or(ctrl.font()));
	args.Graphics.textColour(chooseTextColour(detail.Colour), backColour);
	if (icon && !title) {
		Size const iconSize{rcDetail.height(), rcDetail.height()};
		args.Graphics.drawIcon(icon->handle(), rcItem.topLeft(), iconSize);
		rcDetail += Rect{Percentage{115,unconstrained}*iconSize.Width, 0, 0, 0};
	}
	args.Graphics.drawText(detail.Text, rcDetail, flagsDetail);
	
	args.Graphics.restore();
}

void
LookNFeelProvider::measure(ComboBoxControl& ctrl, MeasureItemEventArgs const& args)
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"ComboBox #{} must be OwnerDraw", args.Ident};

	// [EDIT] Measure the 'edit font', if any, provided prior to construction
	if (args.Item.Index == MeasureItemEventArgs::EditControl) {
		//! @remarks WM_MEASUREITEM is received _extremely_ early in the process of ComboBox construction
		//!           so it's not even feasible to rely on its window font because the control hasn't yet
		//!           received its first WM_SETFONT
		auto const fontHeight = ctrl.editFont().height();
		args.Height = std::abs(fontHeight);
		return;
	}

	// [FIXED-HEIGHT] Return user-provided height, if any, otherwise default
	if (ctrl.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawFixed)) {
		args.Graphics.setFont(ctrl.font());
		args.Height = args.Graphics.measureText(L"Wjgy").Height;
	}
	// [VARIABLE-HEIGHT] Calculate per-item height
	else {
		auto const& item = *reinterpret_cast<ComboBoxControl::ComboBoxItemData*>(args.Item.UserData);
		args.Height = 0;

		// [TITLE] Measure title using custom font, if provided; otherwise ComboBox's heading-font
		if (auto const title = item.Heading; title)
		{
			args.Graphics.setFont(title->Font.value_or(ctrl.titleFont().value_or(ctrl.font())));
			args.Height += args.Graphics.measureText(title->Text).Height;
		}

		// Calculate size required for (potentially multi-line) item text. Use custom font, if one
		//  was provided; otherwise ComboBox's text-font
		auto const detail = item.Detail;
		args.Graphics.setFont(detail.Font.value_or(ctrl.font()));
		args.Height += args.Graphics.measureText(detail.Text, Size{ctrl.droppedRect().width(), args.Height}).Height;
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
	Rect const rcItem = args.Item.Area - Border{measureEdge(ctrl.exStyle()).Width};
	args.Graphics.setBrush(selected ? SystemBrush::Highlight : SystemBrush::Window);
	args.Graphics.fillRect(rcItem);

	// Draw item text
	if (args.Item.Index != args.Empty) {
		args.Graphics.setFont(ctrl.font());
		args.Graphics.textColour(selected ? SystemColour::HighlightText : ctrl.textColour(),
								 selected ? SystemColour::Highlight : ctrl.backColour());
		args.Graphics.drawText(ctrl.Items[args.Item.Index].text(), rcItem);
	}

	args.Graphics.restore();
}

void
LookNFeelProvider::erase(ListBoxControl& ctrl, EraseBackgroundEventArgs const& args) 
{
	// Erase background
	Rect const rcClient = ctrl.clientRect();
	args.Graphics.setBrush(SystemBrush::Window);
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

AnyColour
LookNFeelProvider::button() {
	return this->Colours.Button;
}

Font
LookNFeelProvider::heading1() {
	return this->Fonts.Heading1;
}

Font
LookNFeelProvider::heading2() {
	return this->Fonts.Heading2;
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
LookNFeelProvider::window() {
	return this->Colours.Window;
}
