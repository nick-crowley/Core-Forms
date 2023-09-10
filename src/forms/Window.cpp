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

	auto pThis = reinterpret_cast<type*>(iterator);

	// [OPTIONAL] Exclude non-direct descendants
	if (pThis->Flags == DirectDescendants && ::GetParent(child) != pThis->Parent)
		return ContinueSearch;

	// Exclude windows we didn't create
	if (Window::ExistingWindows.contains(child))
		pThis->Children.push_back(child);
	
	return ContinueSearch;
}

Window::Window()
  : Children(*this),
    Background{SystemBrush::Dialog.handle()},
    LookNFeel{Nt6LookNFeel::Instance},
    Timers(*this)
{
}

Warning
Window::unrecognisedNotificationLogEntry(CommandEventArgs args)
{
	wchar_t buffer[64] {};
	auto const nameLength = ::GetClassNameW(args.Notification->Handle, buffer, lengthof(buffer));
	if (!nameLength)
		return Warning{"forms::Window::onCommand() received {:#06x} from unmanaged child-window id={} (handle={:#08x})", 
			args.Notification->Code, args.Ident, (uintptr_t)args.Notification->Handle};
	
	std::wstring_view const className{&buffer[0], &buffer[nameLength]};
	gsl::czstring controlType{};
	std::string messageName{};

	if (className == WC_BUTTON) {
		messageName = ButtonControl::identifyNotification(args.Notification->Code);
		controlType = "button";
	}
	else if (className == WC_COMBOBOX) {
		messageName = ComboBoxControl::identifyNotification(args.Notification->Code);
		controlType = "combobox";
	}
	else if (className == WC_EDIT) {
		messageName = EditControl::identifyNotification(args.Notification->Code);
		controlType = "edit";
	}
	else if (className == WC_LISTBOX || className == L"ComboLBox") {
		messageName = ListBoxControl::identifyNotification(args.Notification->Code);
		controlType = "listbox";
	}
	else if (className == WC_STATIC) {
		messageName = StaticControl::identifyNotification(args.Notification->Code);
		controlType = "static";
	}
	else {
		messageName = to_hexString<4>(args.Notification->Code);
		controlType = "control";
	}
	return Warning{"forms::Window::onCommand() received {} from unmanaged {} id={} (handle={:#08x})", 
		messageName, controlType, args.Ident, (uintptr_t)args.Notification->Handle};
}
