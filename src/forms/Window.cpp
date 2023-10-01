#include "forms/Window.h"
#include "controls/CommonControls.h"
#include "graphics/SystemBrush.h"
#include "lookNfeel/Nt6LookNFeel.h"
using namespace core;
using namespace forms;

Window const*
Window::BeneathCursor{};

Window::ExistingWindowCollection 
Window::ExistingWindows{};

::BOOL 
CALLBACK Window::HierarchyIterator::onNextChildWindow(::HWND child, ::LPARAM iterator) 
{
	win::Boolean constexpr 
	static ContinueSearch{true};

	auto* const thís = reinterpret_cast<type*>(iterator);

	// [OPTIONAL] Exclude non-direct descendants
	if (thís->Flags == DirectDescendants && ::GetParent(child) != thís->Parent)
		return ContinueSearch;

	// Exclude windows we didn't create
	if (Window::ExistingWindows.contains(child))
		thís->Children.push_back(child);
	
	return ContinueSearch;
}

Window::Window()
  : Children(*this),
    LookNFeel{Nt6LookNFeel::Instance},
    Timers(*this)
{
}

Warning
Window::unrecognisedNotificationLogEntry(CommandEventArgs args)
{	
	CommonControl const controlType = forms::identifyControl(WindowClass::nameFromHandle(args.Notification->Handle));
	std::string messageName{};
	
	switch (controlType){
	case CommonControl::Button:   messageName = ButtonControl::identifyNotification(args.Notification->Code);   break;
	case CommonControl::ComboBox: messageName = ComboBoxControl::identifyNotification(args.Notification->Code); break;
	case CommonControl::Edit:     messageName = EditControl::identifyNotification(args.Notification->Code);     break;
	case CommonControl::ListBox:  messageName = ListBoxControl::identifyNotification(args.Notification->Code);  break;
	case CommonControl::Static:   messageName = StaticControl::identifyNotification(args.Notification->Code);   break;
	default:                      messageName = to_hexString<4>(args.Notification->Code);                       break;
	}
	
	return Warning{"forms::Window::onCommand() received {} from unmanaged {} id={} (handle={:#08x})", 
		messageName, core::to_string(controlType), args.Ident, (uintptr_t)args.Notification->Handle};
}