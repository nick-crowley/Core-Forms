#include "forms/EventArgs/MeasureItemEventArgs.h"
#include "forms/Window.h"
using namespace core;
using namespace forms;

MeasureItemEventArgs::MeasureItemEventArgs(forms::Window& wnd, ::MEASUREITEMSTRUCT& data) 
  : Ident{static_cast<uint16_t>(data.CtlID)},
	Item{data},
	Graphics{wnd.clientDC()},
	Source{static_cast<OwnerDrawControl>(data.CtlType)},
	Window{&wnd}
{
}
