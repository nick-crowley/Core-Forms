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
#include "lookNfeel/ILookNFeelProvider.h"
#include "nstd/Bitset.h"
#include "controls/ButtonStyle.h"
#include "controls/StaticStyle.h"
#include "graphics/DrawFlags.h"
#include "forms/WindowStyle.h"
#include "forms/ExWindowStyle.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class Size;
	class Rect;
	class DeviceContext;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class FormsExport LookNFeelProvider : public ILookNFeelProvider, 
	                                      public std::enable_shared_from_this<ILookNFeelProvider>
	{
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		SharedColourScheme ColourDecorator;
		SharedWindowFrame  FrameDecorator;

	protected:
		struct {
			Font  Heading1;
			Font  Heading2;
			Font  Heading3;
			Font  Paragraph;
		} Fonts;

		struct { 
			AnyColour  Button;
			AnyColourPair Caption;
			AnyColour  Control;
			AnyColour  Highlight;
			AnyColour  Primary;
			AnyColour  Secondary;
			AnyColour  Tertiary;
			AnyColour  Window;
		} Colours;
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	protected:
		//! @brief  Construct
		LookNFeelProvider();

		//! @brief  Construct decorator with properties from another look'n'feel
		LookNFeelProvider(SharedColourScheme alternateColours, SharedWindowFrame windowFrame);
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	protected:
		Font
		makeDefault() const;
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		Rect
		virtual clientArea(Window& wnd, Rect bounds) const override;

		bool
		virtual customFrame() const override;
		
		NonClientLayout
		virtual nonClientArea(Coords results, nstd::bitset<WindowStyle> style, Rect wnd) const override;

		void
		virtual onWindowCreated(Window&) const override;
		
		void
		virtual draw(ButtonControl& ctrl, OwnerDrawEventArgs& args) const override;
	
		void
		virtual draw(CheckBoxControl& ctrl, OwnerDrawEventArgs& args) const override;
	
		void
		virtual draw(ComboBoxControl& ctrl, OwnerDrawEventArgs& args) const override;
	
		void
		virtual measure(ComboBoxControl& ctrl, MeasureItemEventArgs& args) const override;
	
		void
		virtual erase(GroupBoxControl& ctrl, EraseBackgroundEventArgs& args) const override;
	
		void
		virtual draw(LabelControl& ctrl, OwnerDrawEventArgs& args) const override;
	
		void
		virtual draw(ListBoxControl& ctrl, OwnerDrawEventArgs& args) const override;
	
		void
		virtual erase(ListBoxControl& ctrl, EraseBackgroundEventArgs& args) const override;
	
		void
		virtual measure(ListBoxControl& ctrl, MeasureItemEventArgs& args) const override;
	
		void
		virtual draw(ListViewControl& ctrl, OwnerDrawEventArgs& args) const override;
	
		void
		virtual erase(ListViewControl& ctrl, EraseBackgroundEventArgs& args) const override;
	
		void
		virtual measure(ListViewControl& ctrl, MeasureItemEventArgs& args) const override;
	
		void
		virtual draw(PictureControl& ctrl, OwnerDrawEventArgs& args) const override;
	
		void
		virtual draw(RadioButtonControl& ctrl, OwnerDrawEventArgs& args) const override;

		void
		virtual draw(StaticControl& ctrl, OwnerDrawEventArgs& args) const override;
		
		Response
		virtual draw(Dialog& dlg, NonClientPaintEventArgs& args) const override;

		void
		virtual draw(Window& wnd, OwnerDrawMenuEventArgs& args) const override;
		
		void
		virtual measure(Window& wnd, MeasureMenuEventArgs& args) const override;
		
		AnyColour
		virtual button() const override;
		
		AnyColourPair
		virtual caption() const override;
		
		AnyColour
		virtual control() const override;

		Font
		virtual heading1() const override;

		Font
		virtual heading2() const override;
		
		Font
		virtual heading3() const override;
		
		AnyColour
		virtual highlight() const override;

		Font
		virtual paragraph() const override;

		AnyColour
		virtual primary() const override;

		AnyColour
		virtual secondary() const override;
		
		AnyColour
		virtual tertiary() const override;

		AnyColour
		virtual window() const override;
	};
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	DrawTextFlags
	drawTextFlags(nstd::bitset<Alignment> align) noexcept;

	EdgeFlags
	edgeFlags(nstd::bitset<ExWindowStyle> style) noexcept;

	Size
	measureEdge(nstd::bitset<ExWindowStyle> style) noexcept;

	void
	drawWindowBorder(DeviceContext& graphics, Rect const& client, nstd::bitset<WindowStyle> style, nstd::bitset<ExWindowStyle> exStyle) noexcept;

}	// namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o