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
    //! \enum FontWeight - Defines font weights
    enum class FontWeight : uint32_t
    { 
        DontCare = 0,                     //!< 
        Thin = 100,                       //!< 
        ExtraLight = 200,                 //!< 
        Light = 300,                      //!< 
        Normal = 400,                     //!< 
        Medium = 500,                     //!< 
        SemiBold = 600,                   //!< 
        Bold = 700,                       //!< 
        ExtraBold = 800,                  //!< 
        Heavy = 900,                      //!< 
    
        UltraLight = ExtraLight,          //!< 
        Regular = Normal,                 //!< 
        Demibold = SemiBold,              //!< 
        Ultrabold = ExtraBold,            //!< 
    };

    //! \enum FontCharSet - Defines font character sets
    enum class FontCharSet : uint32_t
    { 
        Ansi = 0,                  //!<
        Default = 1,               //!<
        Symbol = 2,                //!<
        Shiftjis = 128,            //!<
        Hangeul = 129,             //!<
        Hangul = 129,              //!<
        Gb2312 = 134,              //!<
        Chinesebig5 = 136,         //!<
        Oem = 255,                 //!<
        Johab = 130,               //!<
        Hebrew = 177,              //!<
        Arabic = 178,              //!<
        Greek = 161,               //!<
        Turkish = 162,             //!<
        Vietnamese = 163,          //!<
        Thai = 222,                //!<
        Easteurope = 238,          //!<
        Russian = 204,             //!<
        Mac = 77,                  //!<
        Baltic = 186,              //!<
    };
  
    //! \enum FontQuality - Defines font quality
    enum class FontQuality : uint32_t
    {
        Default = 0,               //!<
        Draft = 1,                 //!<
        Proof = 2,                 //!<
        NonAntiAliased = 3,        //!<
        AntiAliased = 4,           //!<
        ClearType = 5,             //!< [Windows 5.01]
        ClearTypeNatural = 6,      //!< [Windows 5.01]
    };
  
    //! \enum FontPrecision - Defines font output precision
    enum class FontPrecision : uint32_t
    {
        Default = 0,               //!< 
        String = 1,                //!< 
        Character = 2,             //!< 
        Stroke = 3,                //!< 
        TT = 4,                    //!< 
        Device = 5,                //!< 
        Raster = 6,                //!< 
        TT_Only = 7,               //!< 
        Outline = 8,               //!< 
        ScreenOutline = 9,         //!< 
        PS_Only = 10,              //!< 
    };
    
    //! \enum FontFamily - Defines font families
    enum class FontFamily : uint32_t
    { 
        Default = 0,               //!< Default pitch
        Fixed = 1,                 //!< Fixed pitch
        Variable = 2,              //!< Variable pitch
        MonoFont = 8,              //!< Default pitch
        DontCare = (0<<4),         //!< Don't care or don't know
        Roman = (1<<4),            //!< Variable stroke width, serifed. Times Roman, Century Schoolbook, etc.
        Swiss = (2<<4),            //!< Variable stroke width, sans-serifed. Helvetica, Swiss, etc. 
        Modern = (3<<4),           //!< Constant stroke width, serifed or sans-serifed. Pica, Elite, Courier, etc. 
        Script = (4<<4),           //!< Cursive, etc. 
        Decorative = (5<<4),       //!< Old English, etc. 
    };
  
    enum class HatchStyle {
		Horizontal = HS_HORIZONTAL,    //!< Horizontal hatch
		Vertical = HS_VERTICAL,        //!< Vertical hatch
		Cross = HS_CROSS,              //!< Horizontal and vertical crosshatch
		NorthEast = HS_BDIAGONAL,      //!< 45-degree upward left-to-right hatch
		NorthWest = HS_FDIAGONAL,      //!< 45-degree downward left-to-right hatch
		DiagonalCross = HS_DIAGCROSS,  //!< 45-degree crosshatch
	};

    //! \enum PenStyle - Defines cosmetic and geometric pen styles
    enum class PenStyle : int32_t
    {
        Solid = 0,                          //!< 
        Dash = 1,                           //!<       /* -------  */
        Dot = 2,                            //!<       /* .......  */
        DashDot = 3,                        //!<       /* _._._._  */
        DashDotDot = 4,                     //!<       /* _.._.._  */
        Null = 5,                           //!< 
        InsideFrame = 6,                    //!< 
        UserStyle = 7,                      //!< 
        Alternate = 8,                      //!< 
        StyleMask = 0x0000000f,             //!< 
    
        EndcapRound = 0x00000000,           //!< 
        EndcapSquare = 0x00000100,          //!< 
        EndcapFlat = 0x00000200,            //!< 
        EndcapMask = 0x00000f00,            //!< 
    
        JoinRound = 0x00000000,             //!< 
        JoinBevel = 0x00001000,             //!< 
        JoinMiter = 0x00002000,             //!< 
        JoinMask = 0x0000f000,              //!< 
    
        Cosmetic = 0x00000000,              //!< 
        Geometric = 0x00010000,             //!< 
        TypeMask  = 0x000f0000,             //!< 
    };
}

metadata bool core::meta::Settings<core::bitwise_enum, core::forms::PenStyle> = true;
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o