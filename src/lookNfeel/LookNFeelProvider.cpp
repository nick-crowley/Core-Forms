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
	Rect const content = args.Item.Area - Border{SystemMetric::cxFixedFrame};
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
LookNFeelProvider::draw(LabelControl& ctrl, OwnerDrawEventArgs const& args) 
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"Label #{} must be OwnerDraw", args.Ident};
	
	// Erase background
	args.Graphics.setBrush(ctrl.backColour());
	args.Graphics.fillRect(args.Item.Area);

	std::optional<Font> customFont;
	if (auto textHeight = ctrl.height(); textHeight == PointSize::Default) 
		args.Graphics.setFont(ctrl.font());
	else {
		customFont = Font{*StockFont::OemFixed.handle(), std::nullopt, args.Graphics.measureFont(ctrl.height())};
		args.Graphics.setFont(customFont->handle());
	}

	// Draw text
	auto const style = (ctrl.style<StaticStyle>() & ~StaticStyle::TypeMask) | ctrl.align();
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
	args.Graphics.setFont(ctrl.font());
	args.Graphics.textColour(selected ? SystemColour::HighlightText : ctrl.textColour(),
	                         selected ? SystemColour::Highlight : ctrl.backColour());
	args.Graphics.drawText(ctrl.Items[std::get<uint32_t>(args.Item.Ident)].text(), rcItem);

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
		DeviceContext src{::CreateCompatibleDC(args.Graphics.handle()), ctrl.handle()};
		src.setBitmap(bitmap->handle());

		Rect rcDest = args.Item.Area;
		if (ctrl.style<StaticStyle>().test(StaticStyle::RealSizeImage)) 
			rcDest = Rect{args.Item.Area.topLeft(), bitmap->size()};	
		args.Graphics.copyBitmap(src.handle(), bitmap->depth(), bitmap->rect(), rcDest);

		src.restore();
		::DeleteDC(src.handle());
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
	args.Graphics.setBrush(ctrl.background());
	args.Graphics.drawText(ctrl.text(), args.Item.Area, calculateFlags(ctrl.style<StaticStyle>()));
	
	args.Graphics.restore();
}
