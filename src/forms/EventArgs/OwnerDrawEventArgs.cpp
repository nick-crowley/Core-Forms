#include "forms/EventArgs/OwnerDrawEventArgs.h"
#include "forms/Window.h"
using namespace core;
using namespace forms;

forms::OwnerDrawEventArgs::ItemData::ItemData(::DRAWITEMSTRUCT& data) 
  : Area{data.rcItem},
	Index{static_cast<ItemIndex>(data.itemID)},
    State{static_cast<OwnerDrawState>(data.itemState)},
    UserData{data.itemData}
{
	ThrowIf(data, data.CtlType != ODT_MENU);
}

forms::OwnerDrawEventArgs::OwnerDrawEventArgs(::WPARAM [[maybe_unused]] id, ::LPARAM data) 
  : OwnerDrawEventArgs{*reinterpret_cast<::DRAWITEMSTRUCT*>(data)}
{
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
