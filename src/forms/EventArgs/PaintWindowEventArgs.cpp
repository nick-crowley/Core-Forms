#include "forms/WindowEventArgs.h"
#include "forms/Window.h"
using namespace core;
using namespace forms;

PaintWindowEventArgs::PaintWindowEventArgs(forms::Window& wnd) 
  : PaintWindowEventArgs{wnd, ThrowIfNull(::BeginPaint(wnd.handle(), &this->Data))}
{}

PaintWindowEventArgs::PaintWindowEventArgs(forms::Window& wnd, ::HDC dc) 
  : Area{this->Data.rcPaint},
    Erase{this->Data.fErase != FALSE},
    Restore{this->Data.fRestore != FALSE},
    Update{this->Data.fIncUpdate != FALSE},
    Window(wnd),
    Graphics{SharedDeviceContext{
    	dc, [this](::HDC) { 
            ::EndPaint(this->Window.handle(), &this->Data); 
        }
    }}
{}
