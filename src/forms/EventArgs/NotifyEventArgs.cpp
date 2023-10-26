#include "forms/EventArgs/NotifyEventArgs.h"
#include "forms/Window.h"
using namespace core;
using namespace forms;

forms::NotifyEventArgs::NotifyEventArgs(::WPARAM, ::LPARAM l)
  : Header{reinterpret_cast<::NMHDR*>(l)},
    Source{static_cast<uint16_t>(this->Header->idFrom), this->Header->hwndFrom},
    Code{static_cast<uint16_t>(this->Header->code)},
    Window{Window::ExistingWindows.find(this->Header->hwndFrom)}
{
    static_assert(sizeof(::LPARAM) == sizeof(::NMHDR*));
}