#include "lookNfeel/DarkModernLookNFeel.h"
#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "uxtheme.lib")
using namespace core;
using namespace forms;

std::shared_ptr<ILookNFeelProvider> const
DarkModernLookNFeel::Instance = std::make_shared<DarkModernLookNFeel>();

DarkModernLookNFeel::DarkModernLookNFeel()
{
	this->Colours.Button = SystemColour::ButtonDkShadow;
	this->Colours.Caption = {to_colour(36,36,36), Colour::Black};
	this->Colours.Control = to_colour(36,36,36);
    this->Colours.Highlight = SystemColour::GrayText;
    this->Colours.Primary = Colour::White;
    this->Colours.Secondary = SystemColour::ButtonDkShadow;
    this->Colours.Tertiary = SystemColour::ButtonShadow;
	this->Colours.Window = to_colour(36,36,36);
}

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

void
DarkModernLookNFeel::onCreated(Window& wnd, CreateWindowEventArgs const& args) {
	
	BOOL constexpr
	static enable = TRUE;

	if (!wnd.parent())
		if (!::DwmSetWindowAttribute(wnd.handle(), DWMWA_USE_IMMERSIVE_DARK_MODE, &enable, sizeof(enable)))
			clog << Warning{"DwmSetWindowAttribute() failed: {}", win::LastError{}.str()};
}
