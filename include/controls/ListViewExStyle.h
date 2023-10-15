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
#include "forms/ExWindowStyle.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	enum class ListViewExStyle : uint32_t {
		None                  = 0,
		AutoAutoArrange       = LVS_EX_AUTOAUTOARRANGE,       //!< [v6.00+] Automatically arrange icons if no icon positions have been set (Similar to LVS_AUTOARRANGE).
		AutoCheckselect       = LVS_EX_AUTOCHECKSELECT,       //!< [v6.00+] Automatically select check boxes on single click.
		AutoSizeColumns       = LVS_EX_AUTOSIZECOLUMNS,       //!< [v6.00+] Automatically size listview columns.
		BorderSelect          = LVS_EX_BORDERSELECT,          //!< [v4.71+] Changes border color when an item is selected, instead of highlighting the item.
		CheckBoxes            = LVS_EX_CHECKBOXES,            //!< [v4.70+] Enables check boxes for items in a list-view control.
		                                                      //!< [v6.00+] Check boxes are visible and functional with all list view modes except the tile view mode.
		ColumnOverflow        = LVS_EX_COLUMNOVERFLOW,        //!< Indicates that an overflow button should be displayed in icon/tile view if there is not enough client width to display the complete set of header items.
		ColumnSnapPoints      = LVS_EX_COLUMNSNAPPOINTS,      //!< [v6.00+] Snap to minimum column width when the user resizes a column.
		DoubleBuffer          = LVS_EX_DOUBLEBUFFER,          //!< [v6.00+] Paints via double-buffering, which reduces flicker.
		FlatScrollBars        = LVS_EX_FLATSB,                //!< Enables flat scroll bars in the list view.
		FullRowSelect         = LVS_EX_FULLROWSELECT,         //!< When an item is selected, the item and all its subitems are highlighted.
		Gridlines             = LVS_EX_GRIDLINES,             //!< Displays gridlines around items and subitems.
		HeaderDragDrop        = LVS_EX_HEADERDRAGDROP,        //!< Enables drag-and-drop reordering of columns in a list-view control.
		HeaderInAllViews      = LVS_EX_HEADERINALLVIEWS,      //!< [v6.00+] Show column headers in all view modes.
		HideLabels            = LVS_EX_HIDELABELS,            //!< [v6.00+] Hides the labels in icon and small icon view.
		InfoTip               = LVS_EX_INFOTIP,               //!< When a list-view control uses the LVS_EX_INFOTIP style, the LVN_GETINFOTIP notification code is sent to the parent window before displaying an item's tooltip.
		JustifyColumns        = LVS_EX_JUSTIFYCOLUMNS,        //!< [v6.00+] Icons are lined up in columns that use up the whole view.
		LabelTip              = LVS_EX_LABELTIP,              //!< If a partially hidden label in any list view mode lacks tooltip text, the list-view control will unfold the label.
		MultiWorkAreas        = LVS_EX_MULTIWORKAREAS,        //!< If the list-view control has the LVS_AUTOARRANGE style, the control will not autoarrange its icons until one or more work areas are defined.
		OneClickActivate      = LVS_EX_ONECLICKACTIVATE,      //!< The list-view control sends an LVN_ITEMACTIVATE notification code to the parent window when the user clicks an item.
		Regional              = LVS_EX_REGIONAL,              //!< [v4.71..v5.80] Sets the list view window region to include only the item icons and text using SetWindowRgn.
		SimpleSelect          = LVS_EX_SIMPLESELECT,          //!< [v6.00+] In icon view, moves the state image of the control to the top right of the large icon rendering.
		SingleRow             = LVS_EX_SINGLEROW,             //!< [v6.00+] Not used.
		SnapToGrid            = LVS_EX_SNAPTOGRID,            //!< [v6.00+] In icon view, icons automatically snap into a grid.
		SubItemImages         = LVS_EX_SUBITEMIMAGES,         //!< Allows images to be displayed for subitems.
		TrackSelect           = LVS_EX_TRACKSELECT,           //!< Enables hot-track selection in a list-view control.
		TransparentBkgnd      = LVS_EX_TRANSPARENTBKGND,      //!< [v6.00+] Background is painted by the parent via WM_PRINTCLIENT.
		TransparentShadowText = LVS_EX_TRANSPARENTSHADOWTEXT, //!< [v6.00+] Enable shadow text on transparent backgrounds only.
		TwoClickActivate      = LVS_EX_TWOCLICKACTIVATE,      //!< The list-view control sends an LVN_ITEMACTIVATE notification code to the parent window when the user double-clicks an item.
		UnderlineCold         = LVS_EX_UNDERLINECOLD,         //!< Causes those non-hot items that may be activated to be displayed with underlined text.
		UnderlineHot          = LVS_EX_UNDERLINEHOT,          //!< Causes those hot items that may be activated to be displayed with underlined text.
	};
}
namespace core::meta 
{
	metadata bool Settings<bitwise_enum, forms::ListViewExStyle> = true;
	metadata bool Settings<compatible_enum, forms::ListViewExStyle, forms::ExWindowStyle> = true;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o