#include "windows/ChildWindowIterator.h"
using namespace core;
using namespace forms;

const ConstChildWindowIterator  
ConstChildWindowIterator::npos;

::BOOL 
CALLBACK ConstChildWindowIterator::on_next_child_window(::HWND child, ::LPARAM iterator) 
{
	static auto constexpr CONTINUE = TRUE;

	auto pThis = reinterpret_cast<type*>(iterator);

	// Exclude non-direct descendants
	if (::GetParent(child) != pThis->Parent)
		return CONTINUE;

	// Exclude windows we didn't create
	if (!Window::s_ExistingWindows.contains(child))
		return CONTINUE;
	
	pThis->Children.push_back(child);
	return CONTINUE;
}
