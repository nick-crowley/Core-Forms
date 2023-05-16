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
#include "forms/WindowStyle.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	enum class ButtonStyle : uint32_t {
		PushButton			= BS_PUSHBUTTON			, // -|  Creates standard button
		DefPushButton		= BS_DEFPUSHBUTTON		, //  |  Creates standard button as the default control
		CheckBox			= BS_CHECKBOX			, //  |  Creates a checkbox that doesn't automatically change state when the user clicks on it
		AutoCheckBox		= BS_AUTOCHECKBOX		, //  |  Creates a (unchecked) checkbox with text on the right-hand side. [Use LeftText to place it to the left]
		TriState            = BS_3STATE             , //  |  Creates a tri-state checkbox that can be greyed-out that doesn't automatically change state when the user clicks on it
		AutoTriState        = BS_AUTO3STATE         , //  |  Creates a tri-state checkbox that can be greyed-out
		RadioButton			= BS_RADIOBUTTON		, //  |  Creates a small circle with text that doesn't automatically change state when the user clicks it
		AutoRadioButton		= BS_AUTORADIOBUTTON	, //  |  Creates a small circle with text on the right-hand side. [Use LeftText to place it to the left]
		GroupBox			= BS_GROUPBOX			, //  |  Creates a rectangle in which other controls can be grouped. Text is displayed in the rectangle's upper left corner.
													  //  +--> Mutually exclusive control-type style-bits
		UserButton			= BS_USERBUTTON			, //  |  Obselete
		SplitButton         = BS_SPLITBUTTON        , //  |  Creates split button. A split button has a drop down arrow.
		DefSplitButton      = BS_DEFSPLITBUTTON     , //  |  Creates split button as the default control
		CommandLink         = BS_COMMANDLINK        , //  |  Creates command link button with a green arrow on the left pointing to the button text (with optional caption)
		DefCommandLink      = BS_DEFCOMMANDLINK     , //  |  Creates command link button as the default control
		PushBox				= BS_PUSHBOX			, //  |  Undocumented
		OwnerDraw			= BS_OWNERDRAW			, // -|  Parent is responsible for drawing
		TypeMask			= BS_TYPEMASK			, // Mask for the control-type style-bits
		LeftText			= BS_LEFTTEXT			, // [CheckBox/RadioButton] Place text on the left-hand side instead of the default (RHS)
		RightButton			= LeftText				, // Same as above
		Text				= BS_TEXT				, // Specifies that the button displays text.
		Icon				= BS_ICON				, // Specifies that the button displays an icon.
		Bitmap				= BS_BITMAP				, // Specifies that the button displays a bitmap
		Left				= BS_LEFT				, // Left-justify the text
		Right				= BS_RIGHT				, // Right-justify the text
		Centre				= BS_CENTER				, // Centers text horizontally in the button rectangle
		Top					= BS_TOP				, // Places text at the top of the button rectangle.
		Bottom				= BS_BOTTOM				, // Places text at the bottom of the button rectangle
		VCentre				= BS_VCENTER			, // Places text in the middle (vertically) of the button rectangle.
		PushLike			= BS_PUSHLIKE			, // Display CheckBox and RadioButton states as raised/sunken
		MultiLine			= BS_MULTILINE			, // Wraps the button text to multiple lines if the text string is too long to fit on a single line
		Notify				= BS_NOTIFY				, // Sends BN_KILLFOCUS and BN_SETFOCUS notifications (BN_DBLCLK notifications further require RadioButton or OwnerDraw style)
		Flat				= BS_FLAT				, // Display as two-dimensional [do not use the default shading to create a 3-D image]
	
		None				= PushButton,
	};
}
namespace core::meta 
{
	metadata bool Settings<bitwise_enum, forms::ButtonStyle> = true;
	metadata bool Settings<compatible_enum, forms::ButtonStyle, forms::WindowStyle> = true;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o