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
	enum class ExWindowStyle : uint32_t {
		DlgModalFrame       = WS_EX_DLGMODALFRAME			, // 0x00000001L
		NoParentNotify      = WS_EX_NOPARENTNOTIFY			, // 0x00000004L
		TopMost             = WS_EX_TOPMOST					, // 0x00000008L
		AcceptFiles         = WS_EX_ACCEPTFILES				, // 0x00000010L
		Transparent         = WS_EX_TRANSPARENT				, // 0x00000020L
		MdiChild            = WS_EX_MDICHILD				, // 0x00000040L
		ToolWindow          = WS_EX_TOOLWINDOW				, // 0x00000080L
		WindowEdge          = WS_EX_WINDOWEDGE				, // 0x00000100L
		ClientEdge          = WS_EX_CLIENTEDGE				, // 0x00000200L
		ContextHelp         = WS_EX_CONTEXTHELP				, // 0x00000400L
		Right               = WS_EX_RIGHT					, // 0x00001000L
		Left                = WS_EX_LEFT					, // 0x00000000L
		RtlReading          = WS_EX_RTLREADING				, // 0x00002000L
		LtrReading          = WS_EX_LTRREADING				, // 0x00000000L
		LeftScrollbar       = WS_EX_LEFTSCROLLBAR			, // 0x00004000L
		RightScrollbar      = WS_EX_RIGHTSCROLLBAR			, // 0x00000000L
		ControlParent       = WS_EX_CONTROLPARENT			, // 0x00010000L
		StaticEdge          = WS_EX_STATICEDGE				, // 0x00020000L
		AppWindow           = WS_EX_APPWINDOW				, // 0x00040000L
		OverlappedWindow    = WindowEdge|ClientEdge			, // WS_EX_OVERLAPPEDWINDOW	  (WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE)
		PaletteWindow       = WindowEdge|ToolWindow|TopMost , // WS_EX_PALETTEWINDOW	  (WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST)
		Layered             = WS_EX_LAYERED					, // 0x00080000
		NoInheritLayout     = WS_EX_NOINHERITLAYOUT			, // 0x00100000L // Disable inheritence of mirroring by children
		NoRedirectionBitmap = WS_EX_NOREDIRECTIONBITMAP		, // 0x00200000L
		LayoutRtl           = WS_EX_LAYOUTRTL				, // 0x00400000L // Right to left mirroring
		Composited          = WS_EX_COMPOSITED				, // 0x02000000L
		NoActivate          = WS_EX_NOACTIVATE				, // 0x08000000L
		ActiveCaption		= WS_ACTIVECAPTION				, // 0x0001
	
		None				= 0,
	};
}
namespace core::meta
{
	metadata bool Settings<bitwise_enum, forms::ExWindowStyle> = true;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o