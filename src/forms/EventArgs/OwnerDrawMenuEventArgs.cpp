#include "forms/EventArgs/OwnerDrawMenuEventArgs.h"
#include "forms/Window.h"
using namespace core;
using namespace forms;

forms::OwnerDrawMenuEventArgs::OwnerDrawMenuEventArgs(::WPARAM id, ::LPARAM data) 
  : OwnerDrawMenuEventArgs{*reinterpret_cast<::DRAWITEMSTRUCT*>(data)}
{
	Expects(id == 0);
}

forms::OwnerDrawMenuEventArgs::OwnerDrawMenuEventArgs(::DRAWITEMSTRUCT& data)
  : Action{static_cast<OwnerDrawAction>(data.itemAction)},
	Item{data},
	Menu{reinterpret_cast<::HMENU>(data.hwndItem)},
	Graphics{SharedDeviceContext{data.hDC, weakref}}
{
}
