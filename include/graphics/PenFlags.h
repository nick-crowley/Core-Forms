#pragma once
#include "Common.h"
#include "PlatformSdk.h"

namespace core::forms
{
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

constdata bool core::meta::Settings<core::meta::bitwise_enum_t, core::forms::PenStyle> = true;
