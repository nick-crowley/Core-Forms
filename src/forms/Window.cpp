#include "forms/Window.h"
#include "graphics/SystemBrush.h"
#include "lookNfeel/Nt6LookNFeel.h"
using namespace core;
using namespace forms;

Window const*
Window::BeneathCursor{};

Window::ExistingWindowCollection 
Window::ExistingWindows{};

//Window const
//Window::s_Desktop;

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
