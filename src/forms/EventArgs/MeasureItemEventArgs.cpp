#include "forms/EventArgs/MeasureItemEventArgs.h"
#include "forms/Window.h"
using namespace core;
using namespace forms;

MeasureItemEventArgs::MeasureItemEventArgs(forms::Window& wnd, ::MEASUREITEMSTRUCT& data) 
  : Ident{static_cast<uint16_t>(data.CtlID)},
	Item{data},
	Graphics{wnd.clientDC()},
	Height{data.itemHeight},
	Source{static_cast<OwnerDrawControl>(data.CtlType)},
	Width{data.itemWidth},
	Window{&wnd}
{
}
