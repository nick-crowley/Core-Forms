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
#include "forms/WindowStyle.h"
#include "forms/NonClientLayout.h"
#include "forms/Response.h"
#include "looknfeel/IColourScheme.h"
#include "looknfeel/IWindowFrame.h"
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
	//! @brief  Distinguishes colours scheme used by the current look-n-feel
	enum class ColourScheme : uint32_t { Default, Dark, Light };

	//! @brief  Supported looks-n-feels
	enum class VisualStyle : uint32_t { Modern, Retro, Vintage };
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	struct ILookNFeelProvider : IColourScheme, IWindowFrame
	{
		satisfies(ILookNFeelProvider,
			IsInterface
		);
		
		void
		virtual draw(ButtonControl& ctrl, OwnerDrawEventArgs& args) const abstract;
	
		void
		virtual draw(CheckBoxControl& ctrl, OwnerDrawEventArgs& args) const abstract;
	
		void
		virtual draw(ComboBoxControl& ctrl, OwnerDrawEventArgs& args) const abstract;
	
		void
		virtual measure(ComboBoxControl& ctrl, MeasureItemEventArgs& args) const abstract;
	
		void
		virtual draw(LabelControl& ctrl, OwnerDrawEventArgs& args) const abstract;
	
		void
		virtual draw(ListBoxControl& ctrl, OwnerDrawEventArgs& args) const abstract;
	
		void
		virtual erase(ListBoxControl& ctrl, EraseBackgroundEventArgs& args) const abstract;
	
		void
		virtual measure(ListBoxControl& ctrl, MeasureItemEventArgs& args) const abstract;
	
		void
		virtual draw(ListViewControl& ctrl, OwnerDrawEventArgs& args) const abstract;
	
		void
		virtual erase(ListViewControl& ctrl, EraseBackgroundEventArgs& args) const abstract;
	
		void
		virtual measure(ListViewControl& ctrl, MeasureItemEventArgs& args) const abstract;
	
		void
		virtual draw(GroupBoxControl& ctrl, OwnerDrawEventArgs& args) const abstract;
	
		void
		virtual erase(GroupBoxControl& ctrl, EraseBackgroundEventArgs& args) const abstract;
	
		void
		virtual draw(PictureControl& ctrl, OwnerDrawEventArgs& args) const abstract;
	
		void
		virtual draw(RadioButtonControl& ctrl, OwnerDrawEventArgs& args) const abstract;

		void
		virtual draw(StaticControl& ctrl, OwnerDrawEventArgs& args) const abstract;

		void
		virtual draw(Dialog& dlg, PaintWindowEventArgs& args) const abstract;
	
		Response
		virtual draw(Dialog& dlg, NonClientPaintEventArgs& args) const abstract;
		
		void
		virtual draw(Window& wnd, OwnerDrawMenuEventArgs& args) const abstract;
		
		void
		virtual measure(Window& wnd, MeasureMenuEventArgs& args) const abstract;
	};

	using SharedLookNFeelProvider = std::shared_ptr<ILookNFeelProvider const>;

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