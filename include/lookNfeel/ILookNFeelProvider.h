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
#include "graphics/Colours.h"
#include "graphics/Font.h"
#include "forms/WindowStyle.h"
#include "forms/NonClientLayout.h"
#include "forms/Response.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class Window;
	class Dialog;
	class Menu;
	class ButtonControl;
	class CheckBoxControl;
	class ComboBoxControl;
	class LabelControl;
	class ListBoxControl;
	class ListViewControl;
	class GroupBoxControl;
	class PictureControl;
	class RadioButtonControl;
	class StaticControl;
	class CreateWindowEventArgs;
	class EraseBackgroundEventArgs;
	class InitDialogEventArgs;
	class MeasureItemEventArgs;
	class MeasureMenuEventArgs;
	class OwnerDrawEventArgs;
	class OwnerDrawMenuEventArgs;
	class PaintWindowEventArgs;
	class NonClientPaintEventArgs;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	//! @brief  Distinguishes colours used by the current look-n-feel
	enum class ColourScheme : uint32_t { Default, Dark, Light };
	
	//! @brief  Supported looks-n-feels
	enum class VisualStyle : uint32_t { Modern, Retro, Vintage };
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	struct ILookNFeelProvider
	{
		struct FontDescription 
		{
			std::wstring  Name;
			PointSize     Height;
		};

		struct AnyColourPair
		{
			AnyColour  Active, Inactive;
		};

		satisfies(ILookNFeelProvider,
			IsInterface
		);
		
		Rect
		virtual clientRect(Window& wnd, Rect bounds) const abstract;

		bool
		virtual customCaption() const abstract;

		void
		virtual draw(ButtonControl& ctrl, OwnerDrawEventArgs const& args) abstract;
	
		void
		virtual draw(CheckBoxControl& ctrl, OwnerDrawEventArgs const& args) abstract;
	
		void
		virtual draw(ComboBoxControl& ctrl, OwnerDrawEventArgs const& args) abstract;
	
		void
		virtual measure(ComboBoxControl& ctrl, MeasureItemEventArgs const& args) abstract;
	
		void
		virtual draw(LabelControl& ctrl, OwnerDrawEventArgs const& args) abstract;
	
		void
		virtual draw(ListBoxControl& ctrl, OwnerDrawEventArgs const& args) abstract;
	
		void
		virtual erase(ListBoxControl& ctrl, EraseBackgroundEventArgs const& args) abstract;
	
		void
		virtual measure(ListBoxControl& ctrl, MeasureItemEventArgs const& args) abstract;
	
		void
		virtual draw(ListViewControl& ctrl, OwnerDrawEventArgs const& args) abstract;
	
		void
		virtual erase(ListViewControl& ctrl, EraseBackgroundEventArgs const& args) abstract;
	
		void
		virtual measure(ListViewControl& ctrl, MeasureItemEventArgs const& args) abstract;
	
		void
		virtual draw(GroupBoxControl& ctrl, OwnerDrawEventArgs const& args) abstract;
	
		void
		virtual erase(GroupBoxControl& ctrl, EraseBackgroundEventArgs const& args) abstract;
	
		void
		virtual draw(PictureControl& ctrl, OwnerDrawEventArgs const& args) abstract;
	
		void
		virtual draw(RadioButtonControl& ctrl, OwnerDrawEventArgs const& args) abstract;

		void
		virtual draw(StaticControl& ctrl, OwnerDrawEventArgs const& args) abstract;

		void
		virtual draw(Dialog& dlg, PaintWindowEventArgs const& args) abstract;
	
		Response
		virtual draw(Dialog& dlg, NonClientPaintEventArgs& args) abstract;
		
		void
		virtual draw(Window& wnd, OwnerDrawMenuEventArgs& args) abstract;
		
		void
		virtual measure(Window& wnd, MeasureMenuEventArgs& args) abstract;
		
		//! @brief  Retrieve non-client area component bounds
		//! @param  client  Client area (in screen co-ordinates)
		NonClientLayout
		virtual nonClient(Coords results, nstd::bitset<WindowStyle> style, Rect wnd) const abstract;

		void
		virtual onCreated(Window&, CreateWindowEventArgs const& args) abstract;
		
		AnyColour
		virtual button() abstract;
		
		AnyColourPair
		virtual caption() abstract;
		
		AnyColour
		virtual control() abstract;
		
		FontDescription
		virtual default() abstract;

		Font
		virtual heading1() abstract;

		Font
		virtual heading2() abstract;
		
		Font
		virtual heading3() abstract;
		
		AnyColour
		virtual highlight() abstract;
		
		Font
		virtual paragraph() abstract;

		AnyColour
		virtual primary() abstract;
		
		AnyColour
		virtual secondary() abstract;
		
		AnyColour
		virtual tertiary() abstract;

		AnyColour
		virtual window() abstract;
	};

	using SharedLookNFeelProvider = std::shared_ptr<ILookNFeelProvider>;

}	// namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms {
	//! @brief  Look'n'feel used when creating windows
	SharedLookNFeelProvider 
	extern FormsExport lookNFeel;

	//! @brief  Change the default look-n-feel used when creating new windows
	void
	setLookNFeel(VisualStyle style, ColourScheme scheme) noexcept;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o