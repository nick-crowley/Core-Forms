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
	ThrowIf(data, data.CtlType == ODT_MENU);
}

forms::OwnerDrawEventArgs::OwnerDrawEventArgs([[maybe_unused]] ::WPARAM id, ::LPARAM data) 
  : OwnerDrawEventArgs{*reinterpret_cast<::DRAWITEMSTRUCT*>(data)}
{
}

forms::OwnerDrawEventArgs::OwnerDrawEventArgs(::DRAWITEMSTRUCT& data) 
  : Action{static_cast<OwnerDrawAction>(data.itemAction)},
    Ident{static_cast<int16_t>(data.CtlID)},
	Item{data},
	Graphics{SharedDeviceContext{data.hDC, weakref}},
	Source{static_cast<OwnerDrawControl>(data.CtlType)},
	Window{Window::ExistingWindows.find(data.hwndItem)}
{
}
