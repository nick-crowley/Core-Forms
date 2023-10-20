#include "lookNfeel/DarkRetroLookNFeel.h"
using namespace core;
using namespace forms;

std::shared_ptr<ILookNFeelProvider> const
DarkRetroLookNFeel::Instance = std::make_shared<DarkRetroLookNFeel>();

DarkRetroLookNFeel::DarkRetroLookNFeel()
{
	this->Colours.Button = SystemColour::ButtonDkShadow;
    this->Colours.Caption = {Colour::Navy, to_colour(36,36,36)};
	this->Colours.Control = to_colour(36,36,36);
    this->Colours.Primary = Colour::White;
    this->Colours.Highlight = SystemColour::GrayText;
    this->Colours.Secondary = SystemColour::ButtonDkShadow;
    this->Colours.Tertiary = SystemColour::ButtonShadow;
	this->Colours.Window = to_colour(36,36,36);
}
