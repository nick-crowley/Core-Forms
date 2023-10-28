#include "forms/EventArgs/MeasureMenuEventArgs.h"
#include "forms/Window.h"
using namespace core;
using namespace forms;

MeasureMenuEventArgs::MeasureMenuEventArgs(forms::Window& wnd, ::MEASUREITEMSTRUCT& data) 
  : Ident{static_cast<uint16_t>(data.CtlID)},
	Item{data},
	Menu{*ThrowIfEmpty(wnd.menu())},
	Graphics{wnd.clientDC()}
{
}
