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
#include "forms/EventArgs/NotifyEventArgs.h"
#include "system/SharedHandle.h"
#include "graphics/Graphics.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class FormsExport Window;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{	
	enum class CustomDrawStage : uint32_t {
		PostErase     = CDDS_POSTERASE,     //!< After the erasing cycle is complete.
		PostPaint     = CDDS_POSTPAINT,     //!< After the painting cycle is complete.
		PreErase      = CDDS_PREERASE,      //!< Before the erasing cycle begins.
		PrePaint      = CDDS_PREPAINT,      //!< Before the painting cycle begins.

		Item          = CDDS_ITEM,          //!< Indicates that the dwItemSpec, uItemState, and lItemlParam members are valid.
		ItemPostErase = CDDS_ITEMPOSTERASE, //!< After an item has been erased.
		ItemPostPaint = CDDS_ITEMPOSTPAINT, //!< After an item has been drawn.
		ItemPreErase  = CDDS_ITEMPREERASE,  //!< Before an item is erased.
		ItemPrePaint  = CDDS_ITEMPREPAINT,  //!< Before an item is drawn.
		SubItem       = CDDS_SUBITEM,       //!< Flag combined with CDDS_ITEMPREPAINT or CDDS_ITEMPOSTPAINT if a subitem is being drawn.
	};

	enum class CustomDrawState : uint32_t {
		Checked          = CDIS_CHECKED,              //!< Item is checked.
		Default          = CDIS_DEFAULT,              //!< Item is in its default state.
		Disabled         = CDIS_DISABLED,             //!< Item is disabled.
		Focus            = CDIS_FOCUS,                //!< Item is in focus.
		Grayed           = CDIS_GRAYED,               //!< Item is grayed.
		Hot              = CDIS_HOT,                  //!< Item is currently under the pointer ("hot").
		Indeterminate    = CDIS_INDETERMINATE,        //!< Item is in an indeterminate state.
		Marked           = CDIS_MARKED,               //!< Item is marked.
		Selected         = CDIS_SELECTED,             //!< Item is selected.
		ShowKeyboardCues = CDIS_SHOWKEYBOARDCUES,     //!< [v6.00] Item is showing its keyboard cues.
		Nearhot          = CDIS_NEARHOT,              //!< Item is part of a control that is currently under the mouse pointer ("hot"), but the item is not "hot" itself.
		OtherSideHot     = CDIS_OTHERSIDEHOT,         //!< Item is part of a splitbutton that is currently under the mouse pointer ("hot"), but the item is not "hot" itself.
		DropHighlighted  = CDIS_DROPHILITED,          //!< Item is currently the drop target of a drag-and-drop operation.
	};
	
	enum class CustomDrawResult : uint32_t {
		DoDefault          = CDRF_DODEFAULT,          //!< Control will draw itself.
		DoErase            = CDRF_DOERASE,            //!< Control will only draw the background.
		NewFont            = CDRF_NEWFONT,            //!< Your application specified a new font for the item; the control will use the new font.
		NotifyItemDraw     = CDRF_NOTIFYITEMDRAW,     //!< Control will notify the parent of any item-related drawing operations.
		NotifyPostErase    = CDRF_NOTIFYPOSTERASE,    //!< Control will notify the parent after erasing an item.
		NotifyPostPaint    = CDRF_NOTIFYPOSTPAINT,    //!< Control will send an NM_CUSTOMDRAW notification code when the painting cycle for the entire control is complete.
		NotifySubItemDraw  = CDRF_NOTIFYSUBITEMDRAW,  //!< Your application will receive an NM_CUSTOMDRAW notification code with dwDrawStage set to CDDS_ITEMPREPAINT | CDDS_SUBITEM before each list-view subitem is drawn.
		SkipDefault        = CDRF_SKIPDEFAULT,        //!< Your application drew the item manually.
		SkipPostPaint      = CDRF_SKIPPOSTPAINT,      //!< Control will not draw the focus rectangle around an item.
	};
}
namespace core::meta 
{
	metadata bool Settings<bitwise_enum, forms::CustomDrawStage> = true;
	metadata bool Settings<bitwise_enum, forms::CustomDrawState> = true;
	metadata bool Settings<bitwise_enum, forms::CustomDrawResult> = true;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class CustomDrawEventArgs : NotifyEventArgs {
		using base = NotifyEventArgs;
	public:
		struct ItemData {
			ItemData(::NMCUSTOMDRAW& data) 
			  : Area{data.rc},
			    Index{static_cast<uintptr_t>(data.dwItemSpec)},
			    State{static_cast<CustomDrawState>(data.uItemState)},
			    UserData{static_cast<uintptr_t>(data.lItemlParam)}
			{}

			Rect                          Area;
			uintptr_t                     Index;
			nstd::bitset<CustomDrawState> State;
			uintptr_t                     UserData;

			template <nstd::Class CustomData>
			CustomData*
			data() const noexcept {
				return reinterpret_cast<CustomData*>(this->UserData);
			}
		};

	private:
		::NMCUSTOMDRAW*   Header;

	public:
		DeviceContext                    mutable Graphics;
		ItemData                         Item;
		nstd::bitset<CustomDrawStage>    Stage;
		forms::Window*                   Window;

		explicit
		CustomDrawEventArgs(NotifyEventArgs args) 
		  : Header{args.header<::NMCUSTOMDRAW>()},
		    Item{*this->Header},
		    Graphics{SharedDeviceContext{this->Header->hdc, weakref}},
		    Stage{static_cast<CustomDrawStage>(this->Header->dwDrawStage)},
		    Window{args.Window}
		{}
	};
	
	using CustomDrawDelegate = Delegate<void (Window&,CustomDrawEventArgs)>;
	using CustomDrawEvent = ObservableEvent<CustomDrawDelegate>;

}	// namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o