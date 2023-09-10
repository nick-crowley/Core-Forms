#include "forms/EventArgs/OwnerDrawEventArgs.h"
#include "forms/Window.h"
using namespace core;
using namespace forms;

forms::OwnerDrawEventArgs::ItemData::ItemData(::DRAWITEMSTRUCT& data) 
  : Area{data.rcItem},
    Data{data.itemData},
    State{static_cast<OwnerDrawState>(data.itemState)}
{
	if (data.CtlType != ODT_MENU)
		this->Ident = static_cast<uint32_t>(data.itemID);
	else
		this->Ident = win::ResourceId{static_cast<uint16_t>(data.itemID)};
}

forms::OwnerDrawEventArgs::OwnerDrawEventArgs(::WPARAM id, ::LPARAM data) 
  : OwnerDrawEventArgs{*reinterpret_cast<::DRAWITEMSTRUCT*>(data)}
{
	Expects(id != 0);		// FIXME: Replace this @c Expects() with @c Invariant()
}

forms::OwnerDrawEventArgs::OwnerDrawEventArgs(::DRAWITEMSTRUCT& data) 
  : Action{static_cast<OwnerDrawAction>(data.itemAction)},
    Ident{static_cast<uint16_t>(data.CtlID)},
	Item{data},
	Graphics{SharedDeviceContext{data.hDC, weakref}},
	Type{static_cast<OwnerDrawControl>(data.CtlType)},
	Window{Window::ExistingWindows.find(data.hwndItem)}
{
}
