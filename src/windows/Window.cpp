#include "windows/Window.h"
#include "windows/Win31LookNFeel.h"
using namespace core;
using namespace forms;

Window::const_pointer
Window::s_BeneathCursor{};

Window::ExistingWindowCollection 
Window::s_ExistingWindows{};

WindowMessageDictionary 
Window::s_MessageDatabase{};

//Window const
//Window::s_Desktop;
	
Window::Window()
  : Children(*this),
    LookNFeel{std::make_shared<Win31LookNFeel>()},
    Timers(*this)
{
}
