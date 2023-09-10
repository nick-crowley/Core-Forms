#include "forms/EventArgs/OwnerDrawMenuEventArgs.h"
#include "forms/Window.h"
using namespace core;
using namespace forms;

forms::OwnerDrawMenuEventArgs::ItemData::ItemData(::DRAWITEMSTRUCT& data) 
  : Area{data.rcItem},
	Ident{win::ResourceId{static_cast<uint16_t>(data.itemID)}},
    State{static_cast<OwnerDrawState>(data.itemState)},
    UserData{data.itemData}
{
	ThrowIf(data, data.CtlType != ODT_MENU);
}

forms::OwnerDrawMenuEventArgs::OwnerDrawMenuEventArgs(::WPARAM [[maybe_unused]] id, ::LPARAM data) 
  : OwnerDrawMenuEventArgs{*reinterpret_cast<::DRAWITEMSTRUCT*>(data)}
{
}

forms::OwnerDrawMenuEventArgs::OwnerDrawMenuEventArgs(::DRAWITEMSTRUCT& data)
  : Action{static_cast<OwnerDrawAction>(data.itemAction)},
	Item{data},
	Menu{reinterpret_cast<::HMENU>(data.hwndItem)},
	Graphics{SharedDeviceContext{data.hDC, weakref}}
{
}
