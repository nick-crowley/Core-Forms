#include "forms/EventArgs/MeasureMenuEventArgs.h"
#include "forms/Window.h"
using namespace core;
using namespace forms;

MeasureMenuEventArgs::MeasureMenuEventArgs(forms::Window& wnd, ::MEASUREITEMSTRUCT& data) 
  : Item{data},
	Menu{*ThrowIfEmpty(wnd.menu())},
	Graphics{wnd.clientDC()}
{
}
