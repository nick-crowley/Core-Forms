#include "forms/ChildWindowIterator.h"
#include "forms/Window.h"
using namespace core;
using namespace forms;

ConstChildWindowIterator const
ConstChildWindowIterator::npos;

::BOOL 
CALLBACK ConstChildWindowIterator::onNextChildWindow(::HWND child, ::LPARAM iterator) 
{
	static auto constexpr CONTINUE = TRUE;

	auto pThis = reinterpret_cast<type*>(iterator);

	// Exclude non-direct descendants
	if (::GetParent(child) != pThis->Parent)
		return CONTINUE;

	// Exclude windows we didn't create
	if (!Window::ExistingWindows.contains(child))
		return CONTINUE;
	
	pThis->Children.push_back(child);
	return CONTINUE;
}
