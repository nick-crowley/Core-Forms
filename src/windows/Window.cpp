#include "windows/Window.h"
#include "windows/Win31LookNFeel.h"

Window::ExistingWindowCollection 
Window::s_ExistingWindows{};

WindowMessageDictionary 
Window::s_MessageDatabase{};

Window::Window()
  : Children(*this),
    LookNFeel{std::make_shared<Win31LookNFeel>()},
    Timers(*this)
{
}
