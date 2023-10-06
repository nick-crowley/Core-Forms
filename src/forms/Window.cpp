#include "forms/Window.h"
#include "controls/CommonControls.h"
#include "graphics/SystemBrush.h"
using namespace core;
using namespace forms;

Window const*
Window::BeneathCursor{};

Window::ExistingWindowCollection 
Window::ExistingWindows{};

Window::HierarchyIterator::HierarchyIterator(::HWND parent, SearchBehaviour descendants) noexcept 
  : Flags{descendants}, Parent{parent}, Index{0}
{
	ThrowIf(descendants, (descendants & (Managed|Unmanaged)) == 0);
	std::ignore = ::EnumChildWindows(parent, &type::onNextChildWindow, (::LPARAM)(uintptr_t)this);
	if (this->Results.empty()) 
		*this = type{};
}

::BOOL 
CALLBACK Window::HierarchyIterator::onNextChildWindow(::HWND child, ::LPARAM iterator) 
{
	win::Boolean constexpr 
	static ContinueSearch{true};

	auto* const thís = reinterpret_cast<type*>(iterator);

	// [CHILDREN] Exclude non-direct descendants
	if (thís->Flags.test(SearchBehaviour::Children) && ::GetParent(child) != thís->Parent)
		return ContinueSearch;

	// [MANAGED] Exclude windows we didn't create
	bool const isManaged = Window::ExistingWindows.contains(child);
	if ((thís->Flags.test(SearchBehaviour::Managed) && isManaged)
	 || (thís->Flags.test(SearchBehaviour::Unmanaged) && !isManaged))
		thís->Results.push_back(child);
	
	return ContinueSearch;
}

Window::Window()
  : Children(*this),
    LookNFeel{forms::lookNFeel},
    Timers(*this)
{
	Invariant(forms::lookNFeel != nullptr);
	this->FontChanged += {*this, &Window::this_FontChanged}; 
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