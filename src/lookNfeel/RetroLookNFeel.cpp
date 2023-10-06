#include "lookNfeel/RetroLookNFeel.h"
#include "lookNfeel/Win31LookNFeel.h"
#include "controls/CommonControls.h"
using namespace core;
using namespace forms;

std::shared_ptr<ILookNFeelProvider> const
RetroLookNFeel::Instance = std::make_shared<RetroLookNFeel>();

RetroLookNFeel::RetroLookNFeel()
{
	using namespace forms::literals;
	this->Fonts.Paragraph = Font{this->default().Name, DeviceContext::ScreenDC.measureFont(this->default().Height)};
	this->Fonts.Heading2 = Font{*this->Fonts.Paragraph.handle(), std::nullopt, DeviceContext::ScreenDC.measureFont(18_pt)};
	this->Fonts.Heading1 = Font{*this->Fonts.Paragraph.handle(), std::nullopt, DeviceContext::ScreenDC.measureFont(24_pt)};
	this->Colours.Button = SystemColour::ButtonFace;
	this->Colours.Control = Colour::White;
    this->Colours.Primary = Colour::Black;
    this->Colours.Secondary = SystemColour::ButtonFace;
    this->Colours.Tertiary = SystemColour::ButtonShadow;
	this->Colours.Window = Colour::White;
}

ILookNFeelProvider::FontDescription
RetroLookNFeel::default() 
{
	using namespace forms::literals;
	return FontDescription{L"Lucida Console", 11_pt};
}

void
RetroLookNFeel::draw(GroupBoxControl& ctrl, OwnerDrawEventArgs const& args) 
{
	if (!ctrl.ownerDraw())
		throw runtime_error{"GroupBox #{} must be OwnerDraw", args.Ident};

	// Same as Win3.1
	Win31LookNFeel::Instance->draw(ctrl, args);
}

Response
RetroLookNFeel::draw(Window& wnd, NonClientPaintEventArgs args) 
{
	ThrowIfNot(args, args.Graphics == std::nullopt);

	// Same as Win3.1
	return Win31LookNFeel::Instance->draw(wnd, args);
}
