/* o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o */ /*!
* @copyright	Copyright (c) 2023, Nick Crowley. All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted
* provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions
*    and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of
*    conditions and the following disclaimer in the documentation and/or other materials provided
*    with the distribution.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
* FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
* WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
* The views and conclusions contained in the software and documentation are those of the author 
* and should not be interpreted as representing official policies, either expressed or implied, of
* the projects which contain it.
*/
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Preprocessor Directives o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#pragma once
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Header Files o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#include "library/core.Forms.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
    //! \enum Colour - Defines common RGB colours
    enum class Colour : uint32_t
    {
        Black = RGB(0x00,0x00,0x00),      //!< 
        Grey = RGB(0x80,0x80,0x80),       //!< 
        DarkGrey = RGB(0x24,0x24,0x24),   //!< 
    
        Blue = RGB(0x00,0x4B,0xff),       //!< 
        CadetBlue = RGB(0x5f,0x93,0xa0),  //!< 
        CornflowerBlue = RGB(0x64,0x95,0xed),
        DarkBlue = RGB(0x00,0x05,0xff),   //!< 
        SkyBlue = RGB(0x00,0x63,0xff),    //!< 
        Navy = RGB(0x00,0x00,0x80),       //!< 
    
        Cyan = RGB(0x00,0xfa,0xff),       //!< 
        Teal = RGB(0x00,0xff,0x7d),       //!< 
        Lime = RGB(0x00,0xff,0x00),       //!< 
        Green = RGB(0x32,0xcd,0x32),      //!< 
        Leaves = RGB(0x22,0x8b,0x22),     //!< 
        Forest = RGB(0x00,0x64,0x00),     //!< 
        DarkSeaGreen = RGB(0x8f,0xbc,0x8f),

        Yellow = RGB(0xbe,0xff,0x00),     //!< 
        Gold = RGB(0xff,0xff,0x00),       //!< 
        Orange = RGB(0xff,0xc3,0x00),     //!< 
        Honey = RGB(0xff,0x4b,0x00),      //!< 
        Brown = RGB(0x66,0x33,0x00),      //!< 

        Red = RGB(0xff,0x00,0x05),        //!< 
        Rose = RGB(0xff,0x00,0x73),       //!< 
        Pink = RGB(0xff,0x00,0xcd),       //!< 
        Purple = RGB(0xff,0x00,0xff),     //!< 
        Magenta = RGB(0x64,0x00,0xff),    //!< 
        Plum = RGB(0xdd,0xa0,0xdd),       //!< 
        
        PaleGoldenRod = RGB(0xee,0xe8,0xaa),
        Bisque = RGB(0xff,0xe4,0xc4),     //!< 
        Beige = RGB(0xf5,0xf5,0xdc),      //!< 
        Wheat = RGB(0xf5,0xde,0xb3),      //!< 
        Snow = RGB(0xff,0xfa,0xfa),       //!< 
        White = RGB(0xff,0xff,0xff),      //!< 

        Invalid = CLR_INVALID,            //!< Sentinel value for invalid colour
    };
  
    
	enum class ColourDepth
	{
		bpp1 = 1,
		bpp4 = 4,
		bpp8 = 8,
		bpp16 = 16,
		bpp24 = 24,
		bpp32 = 32,
	};

    //! \enum SystemColour - Defines system colours
    enum class SystemColour
    {
        ActiveCaption = 2,                  //!< Active window title bar. 
        InactiveCaption = 3,                //!< Inactive window caption. 
        CaptionText = 9,                    //!< Text in caption, size box, and scroll bar arrow box.
        InactiveCaptionText = 19,           //!< Color of text in an inactive caption.
        GradientActiveCaption = 27,         //!< [windows 5.00] Right side color in the color gradient of an active window's title bar. 
        GradientInactiveCaption = 28,       //!< [windows 5.00] Right side color in the color gradient of an inactive window's title bar

        Menu = 4,                           //!< Menu background.
        MenuText = 7,                       //!< Text in menus.
        MenuHighlight = 29,                 //!< [windows 5.01] The color used to highlight menu items when the menu appears as a flat menu
        MenuBar = 30,                       //!< [windows 5.01] The background color for the menu bar when menus appear as flat menus 
        
        AppWorkspace = 12,                  //!< Background color of multiple document interface (MDI) applications.
        Desktop = 1,                        //!< Desktop
        Dialog = 15,                        //!< Dialog box background
        Window = 5,                         //!< Window background.
        WindowFrame = 6,                    //!< Window frame.
        WindowText = 8,                     //!< Text in windows.

        ActiveBorder = 10,                  //!< Active window border.
        InactiveBorder = 11,                //!< Inactive window border.

        Highlight = 13,                     //!< Item(s) selected in a control.
        HighlightText = 14,                 //!< Text of item(s) selected in a control.
        GrayText = 17,                      //!< Grayed (disabled) text.
        
        ButtonFace = Dialog,                //!< Face color for three-dimensional display elements and for dialog box backgrounds.
        ButtonHighlight = 20,               //!< Highlight color for three-dimensional display elements (for edges facing the light source.)
        ButtonShadow = 16,                  //!< Shadow color for three-dimensional display elements (for edges facing away from the light source).
        ButtonText = 18,                    //!< Text on push buttons.
        ButtonDkShadow = 21,                //!< Dark shadow for three-dimensional display elements.
        ButtonLight = 22,                   //!< Light color for three-dimensional display elements (for edges facing the light source.)
        
        InfoText = 23,                      //!< Text color for tooltip controls.
        Info = 24,                          //!< Background color for tooltip controls.
    
        Hotlight = 26,                      //!< [windows 5.00] Color for a hyperlink or hot-tracked item.
        
        ScrollBar = 0,                      //!< Scroll bar gray area.
     };
  
    //! \enum ThemeColour - Defines themed system colours 
    enum class ThemeColour : int32_t
    { 
        ActiveBorder            = TMT_ACTIVEBORDER,             //!< color of the border around an active window.
        ActiveCaption           = TMT_ACTIVECAPTION,            //!< color of the caption area on an active window.
        AppWorkspace            = TMT_APPWORKSPACE,             //!< color of the application workspace.
        Background              = TMT_BACKGROUND,               //!< color of the background.
        ButtonFace              = TMT_BTNFACE,                  //!< color of a button face.
        ButtonHighlight         = TMT_BTNHIGHLIGHT,             //!< color of the highlight around a button.
        ButtonShadow            = TMT_BTNSHADOW,                //!< color of the shadow underneath a button.
        ButtonText              = TMT_BTNTEXT,                  //!< color of text contained within a button.
        ButtonAlternateFace     = TMT_BUTTONALTERNATEFACE,      //!< color of the alternate face of a button.
        CaptionText             = TMT_CAPTIONTEXT,              //!< color of text drawn in the caption area of an active window.
        DarkShadow3D            = TMT_DKSHADOW3D,               //!< color of three-dimensional dark shadows.
        GradientActiveCaption   = TMT_GRADIENTACTIVECAPTION,    //!< gradient color applied to the caption area of an active window.
        GradientInActiveCaption = TMT_GRADIENTINACTIVECAPTION,  //!< gradient color applied to the caption area of an inactive window.
        GrayText                = TMT_GRAYTEXT,                 //!< color of dimmed text.
        Highlight               = TMT_HIGHLIGHT,                //!< color of a highlight.
        HighlightText           = TMT_HIGHLIGHTTEXT,            //!< color of highlighted text.
        HotTracking             = TMT_HOTTRACKING,              //!< color of highlight applied when a user moves the mouse over a control.
        InactiveBorder          = TMT_INACTIVEBORDER,           //!< color of the border around an inactive window.
        InactiveCaption         = TMT_INACTIVECAPTION,          //!< color of the caption area on an inactive window.
        InactiveCaptionText     = TMT_INACTIVECAPTIONTEXT,      //!< color of the text in the caption area of an inactive window.
        InfoText                = TMT_INFOTEXT,                 //!< color of informational text.
        InfoBackground          = TMT_INFOBK,                   //!< color of the background behind informational text.
        Light3D                 = TMT_LIGHT3D,                  //!< color of three-dimensional light areas.
        MenuBar                 = TMT_MENUBAR,                  //!< color of the menu bar.
        MenuHighlight           = TMT_MENUHILIGHT,              //!< color of highlight drawn on a menu item when the user moves the mouse over it.
        MenuText                = TMT_MENUTEXT,                 //!< color of text drawn on a menu.
        ScrollBar               = TMT_SCROLLBAR,                //!< color of scroll bars.
        Window                  = TMT_WINDOW,                   //!< color of a window.
        WindowFrame             = TMT_WINDOWFRAME,              //!< color of the frame around a window.
        WindowText              = TMT_WINDOWTEXT,               //!< color of text drawn in a window.
    };
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::meta
{
	struct opaque_t {} constexpr  
	inline opaque;
	
	struct transparent_t {} constexpr 
	inline transparent;
}

namespace core
{
	auto constexpr 
	inline opaque = meta::opaque;

	auto constexpr 
	inline transparent = meta::transparent;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	using AnyColour = std::variant<Colour,SystemColour,meta::transparent_t>;
    
    char unsigned constexpr
    inline getRed(Colour c) noexcept {
        return static_cast<char unsigned>((std::to_underlying(c) >> 0) & 0xff);
    }

    char unsigned constexpr
    inline getGreen(Colour c) noexcept {
        return static_cast<char unsigned>((std::to_underlying(c) >> 8) & 0xff);
    }
    
    char unsigned constexpr
    inline getBlue(Colour c) noexcept {
        return static_cast<char unsigned>((std::to_underlying(c) >> 16) & 0xff);
    }
    
    char unsigned constexpr
    inline getAlpha(Colour c) noexcept {
        return static_cast<char unsigned>((std::to_underlying(c) >> 24) & 0xff);
    }

	Colour
	inline to_colour(AnyColour c) 
	{
        ThrowIf(c, std::holds_alternative<meta::transparent_t>(c));
        
        if (auto const* rgb = std::get_if<Colour>(&c); rgb)
            return *rgb;
		return static_cast<Colour>(::GetSysColor(win::DWord{std::get<SystemColour>(c)}));
	}

    template <std::integral Red, std::integral Green, std::integral Blue>
    Colour
	inline to_colour(Red r, Green g, Blue b) {
        return static_cast<Colour>(RGB(r,g,b));
    }

    inline namespace literals
    {
        inline namespace colour_depth_literals
        {
	        ColourDepth consteval
	        operator""_bpp(uint64_t n) noexcept {
		        return static_cast<ColourDepth>(n);
	        }
        }
    }
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o