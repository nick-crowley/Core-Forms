#include "forms/EventArgs/CustomDrawEventArgs.h"
#include "forms/Window.h"
using namespace core;
using namespace forms;

forms::CustomDrawEventArgs::CustomDrawEventArgs(NotifyEventArgs args) 
  : base{args},
    Data{reinterpret_cast<::NMCUSTOMDRAW*>(args.Header)},
    Item{*this->Data},
    Graphics{SharedDeviceContext{this->Data->hdc, weakref}},
    Stage{static_cast<CustomDrawStage>(this->Data->dwDrawStage)},
    Window{Window::ExistingWindows.find(args.Header->hwndFrom)}
{
}
