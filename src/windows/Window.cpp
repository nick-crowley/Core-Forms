#include "windows/Window.h"
#include "graphics/SystemBrush.h"
#include "lookNfeel/Win31LookNFeel.h"
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
    Background{SystemBrush::Dialog.handle()},
    LookNFeel{Win31LookNFeel::Instance},
    Timers(*this)
{
}
