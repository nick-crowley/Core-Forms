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
	enum class WindowStyle : uint32_t {
		Overlapped			= WS_OVERLAPPED					, // 0x00000000L
		Popup				= WS_POPUP						, // 0x80000000L
		Child				= WS_CHILD						, // 0x40000000L
		Minimize			= WS_MINIMIZE					, // 0x20000000L
		Visible				= WS_VISIBLE					, // 0x10000000L
		Disabled			= WS_DISABLED					, // 0x08000000L
		ClipSiblings		= WS_CLIPSIBLINGS				, // 0x04000000L
		ClipChildren		= WS_CLIPCHILDREN				, // 0x02000000L
		Maximize			= WS_MAXIMIZE					, // 0x01000000L
		Caption				= WS_CAPTION					, // 0x00C00000L     /* WS_BORDER | WS_DLGFRAME  */
		Border				= WS_BORDER						, // 0x00800000L
		DlgFrame			= WS_DLGFRAME					, // 0x00400000L
		VScroll				= WS_VSCROLL					, // 0x00200000L
		HScroll				= WS_HSCROLL					, // 0x00100000L
		SysMenu				= WS_SYSMENU					, // 0x00080000L
		ThickFrame			= WS_THICKFRAME					, // 0x00040000L
		Group				= WS_GROUP						, // 0x00020000L
		Tabstop				= WS_TABSTOP					, // 0x00010000L
		MinimizeBox			= WS_MINIMIZEBOX				, // 0x00020000L
		MaximizeBox			= WS_MAXIMIZEBOX				, // 0x00010000L

		Tiled				= WS_TILED						, // WS_OVERLAPPED
		Iconic				= WS_ICONIC						, // WS_MINIMIZE
		SizeBox				= WS_SIZEBOX					, // WS_THICKFRAME
		TiledWindow			= WS_TILEDWINDOW				, // WS_OVERLAPPEDWINDOW
		ChildWindow			= Child							, // WS_CHILDWINDOW
		PopupWindow			= Popup|Border|SysMenu			, // WS_POPUPWINDOW
		OverlappedWindow	= Overlapped|Caption|SysMenu|ThickFrame|MinimizeBox|MaximizeBox, // WS_OVERLAPPEDWINDOW

		None				= Overlapped,
	};
}	// namespace core::forms
namespace core::meta
{
	metadata bool Settings<bitwise_enum, forms::WindowStyle> = true;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o