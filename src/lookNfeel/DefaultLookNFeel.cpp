#include "lookNfeel/ILookNFeelProvider.h"
#include "lookNFeel/DarkModernLookNFeel.h"
#include "lookNFeel/DarkRetroLookNFeel.h"
#include "lookNFeel/Win31LookNFeel.h"
using namespace core;
using namespace forms;

SharedLookNFeelProvider
forms::lookNFeel = std::make_shared<Win31LookNFeel>();

void
forms::setLookNFeel(VisualStyle style, ColourScheme scheme) noexcept {
	// Determine whether to use dark-mode and set visual style
	switch (bool const isDarkMode = (scheme == ColourScheme::Dark); style) {
	case VisualStyle::Modern:
		lookNFeel = isDarkMode ? DarkModernLookNFeel::Instance 
		                       : ModernLookNFeel::Instance; 
		break;
	
	case VisualStyle::Retro:
		lookNFeel = isDarkMode ? DarkRetroLookNFeel::Instance 
		                       : RetroLookNFeel::Instance;  
		break;
	
	case VisualStyle::Vintage: 
		lookNFeel = Win31LookNFeel::Instance;
		break;
	}
}