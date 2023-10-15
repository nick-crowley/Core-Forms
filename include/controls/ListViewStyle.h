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
	enum class ListViewStyle {
		None = 0,
		AlignLeft = LVS_ALIGNLEFT,              //!< Items are left-aligned in icon and small icon view.
		AlignMask = LVS_ALIGNMASK,              //!< The control's current alignment.
		AlignTop = LVS_ALIGNTOP,                //!< Items are aligned with the top of the list-view control in icon and small icon view.
		AutoArrange = LVS_AUTOARRANGE,          //!< Icons are automatically kept arranged in icon and small icon view.
		EditLabels = LVS_EDITLABELS,            //!< Item text can be edited in place.
		Icon = LVS_ICON,                        //!< This style specifies icon view.
		List = LVS_LIST,                        //!< This style specifies list view.
		NoColumnHeader = LVS_NOCOLUMNHEADER,    //!< Column headers are not displayed in report view.
		NoLabelWrap = LVS_NOLABELWRAP,          //!< Item text is displayed on a single line in icon view.
		NoScroll = LVS_NOSCROLL,                //!< Scrolling is disabled. All items must be within the client area.
		NoSortHeader = LVS_NOSORTHEADER,        //!< Column headers do not work like buttons.
		OwnerData = LVS_OWNERDATA,              //!< Version 4.70. This style specifies a virtual list-view control.
		OwnerDrawFixed = LVS_OWNERDRAWFIXED,    //!< The owner window can paint items in report view.
		Report = LVS_REPORT,                    //!< This style specifies report view.
		ShareImageLists = LVS_SHAREIMAGELISTS,  //!< The image list will not be deleted when the control is destroyed.
		ShowSelAlways = LVS_SHOWSELALWAYS,      //!< The selection, if any, is always shown, even if the control does not have the focus.
		SingleSel = LVS_SINGLESEL,              //!< Only one item at a time can be selected.
		SmallIcon = LVS_SMALLICON,              //!< This style specifies small icon view.
		SortAscending = LVS_SORTASCENDING,      //!< Item indexes are sorted based on item text in ascending order.
		SortDescending = LVS_SORTDESCENDING,    //!< Item indexes are sorted based on item text in descending order.
		TypeMask = LVS_TYPEMASK,                //!< Determines the control's current window style.
		TypeStyleMask = LVS_TYPESTYLEMASK,      //!< Determines the window styles that control item alignment and header appearance and behavior.
	};
}
namespace core::meta 
{
	metadata bool Settings<bitwise_enum, forms::ListViewStyle> = true;
	metadata bool Settings<compatible_enum, forms::ListViewStyle, forms::WindowStyle> = true;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o