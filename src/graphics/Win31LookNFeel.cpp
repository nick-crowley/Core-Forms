#include "windows/Win31LookNFeel.h"
#include "controls/ButtonControl.h"
#include "controls/CheckBoxControl.h"
#include "controls/ListBoxControl.h"
#include "controls/LabelControl.h"
#include "controls/GroupBoxControl.h"
#include "controls/PictureControl.h"
#include "controls/StaticControl.h"
#include "controls/RadioButtonControl.h"
#include "Resource.h"

DrawTextFlags
calculateFlags(EnumBitset<ButtonStyle> style) noexcept
{
	EnumBitset<DrawTextFlags> textAlign = style.test(ButtonStyle::Left)  ? DrawTextFlags::Left
	                                    : style.test(ButtonStyle::Right) ? DrawTextFlags::Right
	                                    :                                  DrawTextFlags::Centre;
	
	textAlign |= style.test(ButtonStyle::Top)     ? DrawTextFlags::Top
	           : style.test(ButtonStyle::Bottom)  ? DrawTextFlags::Bottom
	           :                                    DrawTextFlags::VCentre|DrawTextFlags::SingleLine;

	return textAlign;
}

DrawTextFlags
calculateFlags(EnumBitset<StaticStyle> style) noexcept
{
	EnumBitset<DrawTextFlags> textAlign = style.test(StaticStyle::Centre) ? DrawTextFlags::Centre
	                                    : style.test(StaticStyle::Right)  ? DrawTextFlags::Right
	                                    :                                   DrawTextFlags::Left;

	if (style.test(StaticStyle::CentreImage))
		textAlign = textAlign|DrawTextFlags::VCentre|DrawTextFlags::SingleLine;

	return textAlign;
}

EdgeFlags
calculateFlags(EnumBitset<ExWindowStyle> style) noexcept
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
measureEdge(EnumBitset<ExWindowStyle> style) noexcept
{
	if (style.test(ExWindowStyle::ClientEdge|ExWindowStyle::StaticEdge))
		return {SystemMetric::cxBorder, SystemMetric::cyBorder};
	if (style.test(ExWindowStyle::DlgModalFrame))
		return {SystemMetric::cxSizeFrame, SystemMetric::cySizeFrame};
	return Size::Zero;
}

void
drawWindowBorder(DeviceContext& graphics, Rect const& client, EnumBitset<WindowStyle> style, EnumBitset<ExWindowStyle> exStyle) noexcept
{
	auto constexpr edgeStyles = ExWindowStyle::StaticEdge|ExWindowStyle::ClientEdge|ExWindowStyle::DlgModalFrame;

	if (exStyle.test(edgeStyles)) 
		graphics.drawEdge(client, calculateFlags(exStyle), BorderFlags::Rect);

	else if (style.test(WindowStyle::Border)) {
		graphics.setObj(StockObject::BlackPen);
		graphics.drawRect(client);
	}
}

void
Win31LookNFeel::draw(ButtonControl& ctrl, OwnerDrawEventArgs const& args) 
{
	auto const style = ctrl.style<ButtonStyle>();
	if ((style & ButtonStyle::TypeMask) != ButtonStyle::OwnerDraw)
		throw std::runtime_error("Button #" + std::to_string(*args.Ident) + " must be OwnerDraw");

	auto const enabled = ctrl.enabled();
	auto const state = ctrl.state();
	auto const pushed = state.test(ButtonState::Pushed);
	auto const focused = state.test(ButtonState::Focus);
	
#if 0
	//args.Graphics.set(::GetSysColorBrush((int)(enabled ? SystemColour::BtnFace : SystemColour::GrayText)));
	args.Graphics.fillRect(args.Item.Area, ::GetSysColorBrush((int)(enabled ? SystemColour::BtnFace : SystemColour::BtnDkShadow)));
	
	args.Graphics.drawFrameCtrl(args.Item.Area, DFC_BUTTON, pushed   ? DFCS_BUTTONPUSH|DFCS_PUSHED
	                                                      : !enabled ? DFCS_BUTTONPUSH|DFCS_INACTIVE
	                                                      :            DFCS_BUTTONPUSH);
#endif
	
	args.Graphics.drawEdge(args.Item.Area, pushed ? EdgeFlags::Sunken : EdgeFlags::Raised);

	Rect content = args.Item.Area - Border{SystemMetric::cxFixedFrame};
	if (pushed)
		content += Point{1,1};

	args.Graphics.setText(enabled ? SystemColour::WindowText : SystemColour::GrayText);
	args.Graphics.setObj(StockObject::SystemFixedFont);
	args.Graphics.drawText(ctrl.text(), content, calculateFlags(style));

	if (focused)
		args.Graphics.drawFocus(content);

	args.Graphics.restore();
}

void
Win31LookNFeel::draw(CheckBoxControl& ctrl, OwnerDrawEventArgs const& args) 
{
	auto const style = ctrl.style<ButtonStyle>();
	if ((style & ButtonStyle::TypeMask) != ButtonStyle::OwnerDraw)
		throw std::runtime_error("CheckBox #" + std::to_string(*args.Ident) + " must be OwnerDraw");

	auto const enabled = ctrl.enabled();
	auto const checked = ctrl.checked();
	auto const focused = ctrl.state().test(ButtonState::Focus);
	
	Rect const content = args.Item.Area - Border{SystemMetric::cxFixedFrame};
	Rect tick {content.topLeft(), Size{20,20}};
	args.Graphics.drawFrameCtrl(tick, DFC_BUTTON, DFCS_BUTTONCHECK|(checked?DFCS_CHECKED:0));

	Rect areaText = content;
	areaText.Left += 30;
	args.Graphics.setText(enabled ? SystemColour::WindowText : SystemColour::GrayText);
	args.Graphics.setObj(StockObject::SystemFixedFont);
	args.Graphics.drawText(ctrl.text(), areaText, calculateFlags(style));

	if (focused)
		args.Graphics.drawFocus(content);

	args.Graphics.restore();
}

void
Win31LookNFeel::draw(LabelControl& ctrl, OwnerDrawEventArgs const& args) 
{
	auto const style = ctrl.style<StaticStyle>();
	if ((style & StaticStyle::TypeMask) != StaticStyle::OwnerDraw)
		throw std::runtime_error("Label #" + std::to_string(*args.Ident) + " must be OwnerDraw");

	args.Graphics.setObj(StockObject::WhiteBrush);
	args.Graphics.setObj(StockObject::SystemFixedFont);
	args.Graphics.setBack(DrawingMode::Transparent);
	args.Graphics.setText(ctrl.colour());
	args.Graphics.drawText(ctrl.text(), args.Item.Area, calculateFlags(style));

	args.Graphics.restore();
}

void
Win31LookNFeel::draw(ListBoxControl& ctrl, OwnerDrawEventArgs const& args) 
{
	if (!ctrl.style<ListBoxStyle>().test(ListBoxStyle::OwnerDrawFixed))
		throw std::runtime_error("Listbox #" + std::to_string(*args.Ident) + " must be OwnerDraw");

	bool const selected = args.Item.State.test(OwnerDrawState::Selected);
	
	args.Graphics.setObj(selected ? SystemColour::Highlight : SystemColour::Window);
	args.Graphics.setText(selected ? SystemColour::HighlightText : SystemColour::WindowText);
	Rect const rcItem = args.Item.Area - Border{measureEdge(ctrl.exStyle()).Width};
	args.Graphics.fillRect(rcItem);

	args.Graphics.setObj(StockObject::SystemFixedFont);
	args.Graphics.setBack(DrawingMode::Transparent);
	args.Graphics.drawText(ctrl.Items[args.Item.Id].text(), rcItem);

	args.Graphics.restore();
}

void
Win31LookNFeel::erase(ListBoxControl& ctrl, EraseBackgroundEventArgs const& args) 
{
	Rect const rcClient = ctrl.clientRect();
	args.Graphics.setObj(SystemColour::Window);
	args.Graphics.fillRect(rcClient);

	drawWindowBorder(args.Graphics, rcClient, ctrl.style(), ctrl.exStyle());

	args.Graphics.restore();
}

void
Win31LookNFeel::draw(GroupBoxControl& ctrl, OwnerDrawEventArgs const& args) 
{
	auto const style = ctrl.style<ButtonStyle>();
	if ((style & ButtonStyle::TypeMask) != ButtonStyle::OwnerDraw)
		throw std::runtime_error("GroupBox #" + std::to_string(*args.Ident) + " must be OwnerDraw");

	auto const text = ctrl.text();
	auto const textSize = args.Graphics.measureText(text);
	auto frameRect = args.Item.Area;
	frameRect.Top += textSize.Height/2;
	frameRect.Left++;

	args.Graphics.setObj(SystemColour::Window);
	args.Graphics.fillRect(args.Item.Area);

	auto const thickPen = ::CreatePen(PS_SOLID, 2, (COLORREF)Colour::Black);
	args.Graphics.setObj(thickPen);
	args.Graphics.setObj(StockObject::NullBrush);
	args.Graphics.drawRect(frameRect);

	args.Graphics.setBack(DrawingMode::Opaque);
	args.Graphics.setBack(SystemColour::Window);
	args.Graphics.setObj(StockObject::SystemFixedFont);
	args.Graphics.setText(ctrl.colour());

	auto const frameText = L' ' + text + L' ';
	auto const textOffset = Point{SystemMetric::cxSmallIcon,0};
	args.Graphics.drawText(frameText, args.Item.Area + textOffset, DrawTextFlags::Top);

	args.Graphics.restore();
	::DeleteObject(thickPen);
}

void
Win31LookNFeel::draw(PictureControl& ctrl, OwnerDrawEventArgs const& args) 
{
	auto const style = ctrl.style<StaticStyle>();
	if ((style & StaticStyle::TypeMask) != StaticStyle::OwnerDraw)
		throw std::runtime_error("Picture #" + std::to_string(*args.Ident) + " must be OwnerDraw");
	
	if (auto bitmap = ctrl.image(); bitmap) {
		DeviceContext src{::CreateCompatibleDC(args.Graphics.handle()), ctrl.handle()};
		src.setObj(bitmap->handle());

		Rect rcDest = args.Item.Area;
		if (style.test(StaticStyle::RealSizeImage)) 
			rcDest = Rect{args.Item.Area.topLeft(), bitmap->size()};	
		args.Graphics.copyBitmap(src.handle(), bitmap->depth(), bitmap->rect(), rcDest);

		src.restore();
		::DeleteDC(src.handle());
	}
	else if (auto icon = ctrl.icon(); icon) {
		Rect rcDest = args.Item.Area;
		if (style.test(StaticStyle::RealSizeImage)) 
			rcDest = Rect{args.Item.Area.topLeft(), icon->size()};	
		args.Graphics.drawIcon(icon->handle(), rcDest);
	}
}

void
Win31LookNFeel::draw(RadioButtonControl& ctrl, OwnerDrawEventArgs const& args)
{
	auto const style = ctrl.style<ButtonStyle>();
	if ((style & ButtonStyle::TypeMask) != ButtonStyle::OwnerDraw)
		throw std::runtime_error("RadioButton #" + std::to_string(*args.Ident) + " must be OwnerDraw");

	auto const enabled = ctrl.enabled();
	auto const checked = ctrl.checked();
	auto const focused = ctrl.state().test(ButtonState::Focus);
	
	Rect const content = args.Item.Area - Border{SystemMetric::cxFixedFrame};
	Rect const radio {content.topLeft(), Size{20,20}};
	args.Graphics.drawFrameCtrl(radio, DFC_BUTTON, DFCS_BUTTONRADIO|(checked?DFCS_CHECKED:0));

	Rect areaText = content;
	areaText.Left += 30;
	args.Graphics.setText(enabled ? SystemColour::WindowText : SystemColour::GrayText);
	args.Graphics.setObj(StockObject::SystemFixedFont);
	args.Graphics.drawText(ctrl.text(), areaText, calculateFlags(style));

	if (focused)
		args.Graphics.drawFocus(content);

	args.Graphics.restore();
}

void
Win31LookNFeel::draw(StaticControl& ctrl, OwnerDrawEventArgs const& args)
{
	auto const style = ctrl.style<StaticStyle>();

	args.Graphics.setBack(SystemColour::Window);
	args.Graphics.drawText(ctrl.text(), args.Item.Area, calculateFlags(style));
	
	args.Graphics.restore();
}

void
Win31LookNFeel::draw(Dialog& dlg, PaintWindowEventArgs const& args)
{
	args.Graphics->setObj(StockObject::WhiteBrush);
	args.Graphics->fillRect(*args.Area);

	args.Graphics->restore();
}

void
Win31LookNFeel::draw(Window& wnd, PaintNonClientEventArgs const& args) 
{
	auto const activeCaption = args.State == ActivateNonClientEventArgs::Active;
	auto const components = NonClientComponentBounds{args.Bounds};

	// Window frame
	args.Graphics->setObj(SystemColour::BtnShadow);
	args.Graphics->setObj(StockObject::BlackPen);
	auto const insideFrame = args.Bounds - 2*Border{SystemMetric::cxSizeFrame,SystemMetric::cySizeFrame};
	auto const frameEdges = args.Area - Region{insideFrame};
	args.Graphics->fillRegion(frameEdges);
	args.Graphics->frameRegion(frameEdges, DeviceContext::get<StockObject::BlackBrush>(), Size{2,2});
	
	// Caption background
	args.Graphics->setObj(activeCaption ? SystemColour::Highlight : SystemColour::BtnDkShadow);
	//if (args.InvalidArea)
	//	args.Graphics->fillRegion(*args.InvalidArea);
	//else
		args.Graphics->fillRect(components.Caption);
	
	// Caption text
	args.Graphics->setObj(StockObject::SystemFixedFont);
	args.Graphics->setText(SystemColour::HighlightText);
	args.Graphics->setBack(DrawingMode::Transparent);
	args.Graphics->drawText(wnd.text(), components.Title, DrawTextFlags::SimpleCentre);

	// SysMenu button
	args.Graphics->setObj(StockObject::BlackPen);
	args.Graphics->setObj(SystemColour::BtnFace);
	args.Graphics->drawRect(components.SysMenuBtn);
	args.Graphics->setObj(StockObject::BlackPen);
	args.Graphics->setObj(SystemColour::Window);
	Rect rcSysMenuBar {components.Caption.topLeft(), components.SysMenuBtn.size()};
	rcSysMenuBar.inflate(-Size{components.SysMenuBtn.width()/6, components.SysMenuBtn.height()/2 - 2});
	rcSysMenuBar.Left--; // Fix: Compensate for manually adjusted SysMenu hit-box to correctly align left-edge
	args.Graphics->drawRect(rcSysMenuBar);

	// Maximize button
	args.Graphics->fillRect(components.MaximizeBtn, SystemColour::BtnFace);
	args.Graphics->drawEdge(components.MaximizeBtn, EdgeFlags::Raised);
	args.Graphics->setText(SystemColour::WindowText);
	args.Graphics->drawText(L"▲", components.MaximizeBtn, DrawTextFlags::SimpleCentre);

	// Minimize button
	args.Graphics->fillRect(components.MinimizeBtn, SystemColour::BtnFace);
	args.Graphics->drawEdge(components.MinimizeBtn, EdgeFlags::Raised);
	args.Graphics->setText(SystemColour::WindowText);
	args.Graphics->drawText(L"▼", components.MinimizeBtn, DrawTextFlags::SimpleCentre);
	
	args.Graphics->restore();
}
