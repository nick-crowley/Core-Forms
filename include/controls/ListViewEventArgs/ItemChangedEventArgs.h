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
#include "graphics/SizePoint.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	enum class ListViewItemState : uint32_t {
		Activating     = LVIS_ACTIVATING,        //!< Not currently supported.
		Cut            = LVIS_CUT,               //!< The item is marked for a cut-and-paste operation.
		DropHighlited  = LVIS_DROPHILITED,       //!< The item is highlighted as a drag-and-drop target.
		Focused        = LVIS_FOCUSED,           //!< The item has the focus, so it is surrounded by a standard focus rectangle. Although more than one item may be selected, only one item can have the focus.
		OverlayMask    = LVIS_OVERLAYMASK,       //!< The item's overlay image index is retrieved by a mask.
		Selected       = LVIS_SELECTED,          //!< The item is selected. The appearance of a selected item depends on whether it has the focus and also on the system colors used for selection.
		StateImageMask = LVIS_STATEIMAGEMASK,    //!< The item's state image index is retrieved by a mask.
	};

	enum class ListViewItemFlag : uint32_t {
		Alignment   = LVIF_COLFMT,        //!< [Vista+] The piColFmt member is valid or must be set. If this flag is used, the cColumns member is valid or must be set.
		Columns     = LVIF_COLUMNS,       //!< The cColumns member is valid or must be set.
		Permanent   = LVIF_DI_SETITEM,    //!< The operating system should store the requested list item information and not ask for it again.
		GroupId     = LVIF_GROUPID,       //!< The iGroupId member is valid or must be set.
		Image       = LVIF_IMAGE,         //!< The iImage member is valid or must be set.
		Indent      = LVIF_INDENT,        //!< The iIndent member is valid or must be set.
		NoRecompute = LVIF_NORECOMPUTE,   //!< The control will not generate LVN_GETDISPINFO to retrieve text information if it receives an LVM_GETITEM message.
		Param       = LVIF_PARAM,         //!< The lParam member is valid or must be set.
		State       = LVIF_STATE,         //!< The state member is valid or must be set.
		Text        = LVIF_TEXT,          //!< The pszText member is valid or must be set.
	};
}
namespace core::meta 
{
	metadata bool Settings<bitwise_enum, forms::ListViewItemState> = true;
	metadata bool Settings<bitwise_enum, forms::ListViewItemFlag> = true;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class ItemChangedEventArgs : NotifyEventArgs {
		using base = NotifyEventArgs;

	private:
		::NMLISTVIEW*      Data;

	public:
		int32_t            Item;
		int32_t            SubItem;
		ListViewItemState  OldState;
		ListViewItemState  NewState;
		ListViewItemFlag   Changed;
		Point              Position;
		void*              UserData = nullptr;

		explicit
		ItemChangedEventArgs(NotifyEventArgs args) 
		  : base{args},
			Data{reinterpret_cast<::NMLISTVIEW*>(args.Header)},
			Item{this->Data->iItem},
			SubItem{this->Data->iSubItem},
			OldState{static_cast<ListViewItemState>(this->Data->uOldState)},
			NewState{static_cast<ListViewItemState>(this->Data->uNewState)},
			Changed{static_cast<ListViewItemFlag>(this->Data->uChanged)},
			Position{this->Data->ptAction},
			UserData{std::bit_cast<void*>(this->Data->lParam)}
		{
		}
	};
	
	using ItemChangedDelegate = Delegate<void (Window&,ItemChangedEventArgs)>;
	using ItemChangedEvent = ObservableEvent<ItemChangedDelegate>;

}	// namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o