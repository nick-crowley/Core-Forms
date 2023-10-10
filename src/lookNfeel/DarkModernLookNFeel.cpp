#include "lookNfeel/DarkModernLookNFeel.h"
using namespace core;
using namespace forms;

std::shared_ptr<ILookNFeelProvider> const
DarkModernLookNFeel::Instance = std::make_shared<DarkModernLookNFeel>();

DarkModernLookNFeel::DarkModernLookNFeel()
{
	this->Colours.Button = SystemColour::ButtonDkShadow;
	this->Colours.Control = Colour::Black;
    this->Colours.Primary = Colour::White;
    this->Colours.Secondary = SystemColour::ButtonDkShadow;
    this->Colours.Tertiary = SystemColour::ButtonShadow;
	this->Colours.Window = Colour::Black;
}
