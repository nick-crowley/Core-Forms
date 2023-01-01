#pragma once

class Window;
class Dialog;
class ButtonControl;
class CheckBoxControl;
class LabelControl;
class ListBoxControl;
class GroupBoxControl;
class PictureControl;
class RadioButtonControl;
class StaticControl;
class EraseBackgroundEventArgs;
class OwnerDrawEventArgs;
class PaintWindowEventArgs;
class PaintNonClientEventArgs;

struct ILookNFeelProvider
{
	virtual ~ILookNFeelProvider() = default;

	void
	virtual draw(ButtonControl& ctrl, OwnerDrawEventArgs const& args) = 0;
	
	void
	virtual draw(CheckBoxControl& ctrl, OwnerDrawEventArgs const& args) = 0;
	
	void
	virtual draw(LabelControl& ctrl, OwnerDrawEventArgs const& args) = 0;
	
	void
	virtual draw(ListBoxControl& ctrl, OwnerDrawEventArgs const& args) = 0;
	
	void
	virtual erase(ListBoxControl& ctrl, EraseBackgroundEventArgs const& args) = 0;
	
	void
	virtual draw(GroupBoxControl& ctrl, OwnerDrawEventArgs const& args) = 0;
	
	void
	virtual draw(PictureControl& ctrl, OwnerDrawEventArgs const& args) = 0;
	
	void
	virtual draw(RadioButtonControl& ctrl, OwnerDrawEventArgs const& args) = 0;

	void
	virtual draw(StaticControl& ctrl, OwnerDrawEventArgs const& args) = 0;

	void
	virtual draw(Dialog& dlg, PaintWindowEventArgs const& args) = 0;
	
	void
	virtual draw(Window& wnd, PaintNonClientEventArgs const& args) = 0;
};
