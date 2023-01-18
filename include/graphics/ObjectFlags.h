#pragma once
#include "library/core.Forms.h"
#include "support/EnumOperators.h"

namespace core::forms
{
    enum class PenStyle : int32_t;
}

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
  
    //! \enum HatchStyle - Defines brush hatch styles
    enum class HatchStyle : uint32_t
    {
        Horizontal = 0,                     //!<      /* ----- */
        Vertical = 1,                       //!<      /* ||||| */
        ForwardDiagonal = 2,                //!<      /* \\\\\ */
        BackwardDiagonal = 3,               //!<      /* ///// */
        Cross = 4,                          //!<      /* +++++ */
        CrossDiagonal = 5,                  //!<      /* xxxxx */
    };
}

//! \enum PenStyle - Defines cosmetic and geometric pen styles
enum class core::forms::PenStyle : int32_t
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

constdata bool core::meta::Settings<core::meta::bitwise_enum_t, core::forms::PenStyle> = true;
