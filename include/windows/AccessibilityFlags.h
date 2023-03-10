#pragma once
#include "library/core.Forms.h"

namespace core::forms 
{
	enum class ObjectId {
		Alert              = OBJID_ALERT,             //!< An alert that is associated with a window or an application. System provided message boxes are the only UI elements that send events with this object identifier. Server applications cannot use the AccessibleObjectFromX functions with this object identifier. This is a known issue with Microsoft Active Accessibility.
		Caret              = OBJID_CARET,             //!< Text insertion bar (caret) in the window.
		Client             = OBJID_CLIENT,            //!< Window's client area. In most cases, the operating system controls the frame elements and the client object contains all elements that are controlled by the application. Servers only process the WM_GETOBJECT messages in which the lParam is OBJID_CLIENT, OBJID_WINDOW, or a custom object identifier.
		Cursor             = OBJID_CURSOR,            //!< Mouse pointer. There is only one mouse pointer in the system, and it is not a child of any window.
		HScroll            = OBJID_HSCROLL,           //!< Rindow's horizontal scroll bar.
		NativeOm           = OBJID_NATIVEOM,          //!< Response to this object identifier, third-party applications can expose their own object model. Third-party applications can return any COM interface in response to this object identifier.
		Menu               = OBJID_MENU,              //!< Window's menu bar.
		QueryClassnameIdx  = OBJID_QUERYCLASSNAMEIDX, //!< Object identifier that Oleacc.dll uses internally. For more information, see Appendix F: Object Identifier Values for OBJID_QUERYCLASSNAMEIDX.
		SizeGrip           = OBJID_SIZEGRIP,          //!< Window's size grip: an optional frame component located at the lower-right corner of the window frame.
		Sound              = OBJID_SOUND,             //!< Sound object. Sound objects do not have screen locations or children, but they do have name and state attributes. They are children of the application that is playing the sound.
		SysMenu            = OBJID_SYSMENU,           //!< Window's system menu.
		TitleBar           = OBJID_TITLEBAR,          //!< Window's title bar.
		VScroll            = OBJID_VSCROLL,           //!< Window's vertical scroll bar.
		Window             = OBJID_WINDOW,            //!< Window itself rather than a child object.
	};

	enum class WindowRole {
		Alert              = ROLE_SYSTEM_ALERT,             //!< An alert or a condition that a user should be notified about. This role is used only for objects that embody an alert but are not associated with another user interface element, such as a message box, graphic, text, or sound.
		Animation          = ROLE_SYSTEM_ANIMATION,         //!< An animation control whose content changes over time, such as a control that displays a series of bitmap frames. Animation controls are displayed when files are copied or when some other time-consuming task is performed.
		Application        = ROLE_SYSTEM_APPLICATION,       //!< A main window for an application.
		Border             = ROLE_SYSTEM_BORDER,            //!< A window border. The entire border is represented by a single object rather than by separate objects for each side.
		ButtonDropdown     = ROLE_SYSTEM_BUTTONDROPDOWN,    //!< A button that expands a list of items.
		ButtonDropdownGrid = ROLE_SYSTEM_BUTTONDROPDOWNGRID,//!< A button that expands a grid.
		ButtonMenu         = ROLE_SYSTEM_BUTTONMENU,        //!< A button that expands a menu.
		Caret              = ROLE_SYSTEM_CARET,             //!< The system caret.
		Cell               = ROLE_SYSTEM_CELL,              //!< A cell within a table.
		Character          = ROLE_SYSTEM_CHARACTER,         //!< A cartoon-like graphic object, such as Microsoft Office Assistant, which is displayed to provide help to users of an application.
		Chart              = ROLE_SYSTEM_CHART,             //!< A graphical image used to chart data.
		CheckButton        = ROLE_SYSTEM_CHECKBUTTON,       //!< A check box control: an option that is selected or cleared independently of other options.
		Client             = ROLE_SYSTEM_CLIENT,            //!< A window's client area. Microsoft Active Accessibility uses this role as a default if there is a question about the role of a UI element.
		Clock              = ROLE_SYSTEM_CLOCK,             //!< A control that displays time.
		Column             = ROLE_SYSTEM_COLUMN,            //!< A column of cells within a table.
		ColumnHeader       = ROLE_SYSTEM_COLUMNHEADER,      //!< A column header, providing a visual label for a column in a table.
		ComboBox           = ROLE_SYSTEM_COMBOBOX,          //!< A combo box: an edit control with an associated list box that provides a set of predefined choices.
		Cursor             = ROLE_SYSTEM_CURSOR,            //!< The system's mouse pointer.
		Diagram            = ROLE_SYSTEM_DIAGRAM,           //!< A graphical image that is used to diagram data.
		Dial               = ROLE_SYSTEM_DIAL,              //!< A dial or knob.
		Dialog             = ROLE_SYSTEM_DIALOG,            //!< A dialog box or message box.
		Document           = ROLE_SYSTEM_DOCUMENT,          //!< A document window. A document window is always contained within an application window. This role applies only to MDI windows and refers to the object that contains the MDI title bar.
		DropList           = ROLE_SYSTEM_DROPLIST,          //!< The calendar control or SysDateTimePick32. The Microsoft Active Accessibility runtime component uses this role to indicate that either a date or a calendar control has been found.
		Equation           = ROLE_SYSTEM_EQUATION,          //!< A mathematical equation.
		Graphic            = ROLE_SYSTEM_GRAPHIC,           //!< A picture.
		Grip               = ROLE_SYSTEM_GRIP,              //!< A special mouse pointer that allows a user to manipulate user interface elements such as windows. One example of this involves resizing a window by dragging its lower-right corner.
		Grouping           = ROLE_SYSTEM_GROUPING,          //!< Logically groups other objects. There is not always a parent-child relationship between the grouping object and the objects it contains.
		HelpBalloon        = ROLE_SYSTEM_HELPBALLOON,       //!< Displays a help topic in the form of a tooltip or help balloon.
		HotKeyField        = ROLE_SYSTEM_HOTKEYFIELD,       //!< A keyboard shortcut field that allows the user to enter a combination or sequence of keystrokes.
		Indicator          = ROLE_SYSTEM_INDICATOR,         //!< An indicator, such as a pointer graphic, that points to the current item.
		IpAddress          = ROLE_SYSTEM_IPADDRESS,         //!< An edit control that is designed for an IP address. The edit control is divided into sections, each for a specific part of the IP address.
		Link               = ROLE_SYSTEM_LINK,              //!< A link to something else. This object might look like text or a graphic, but it acts like a button.
		List               = ROLE_SYSTEM_LIST,              //!< A list box, allowing the user to select one or more items.
		ListItem           = ROLE_SYSTEM_LISTITEM,          //!< An item in a list box or in the list portion of a combo box, drop-down list box, or drop-down combo box.
		MenuBar            = ROLE_SYSTEM_MENUBAR,           //!< The menu bar (positioned beneath the title bar of a window) from which users select menus.
		MenuItem           = ROLE_SYSTEM_MENUITEM,          //!< A menu item: an menu entry that the user can choose to carry out a command, select an option, or display another menu. Functionally, a menu item is equivalent to a push button, a radio button, a check box, or a menu.
		MenuPopup          = ROLE_SYSTEM_MENUPOPUP,         //!< A menu: a list of options, each with a specific action. All menu types must have role, including the drop-down menus which are displayed when selected from a menu bar; and shortcut menus, which are displayed by clicking the right mouse button.
		Outline            = ROLE_SYSTEM_OUTLINE,           //!< An outline or a tree structure, such as a tree view control, that displays a hierarchical list and allows the user to expand and collapse branches.
		OutlineButton      = ROLE_SYSTEM_OUTLINEBUTTON,     //!< An item that navigates like an outline item. The UP and DOWN ARROW keys are used to navigate through the outline. However, instead of expanding and collapsing when the LEFT and RIGHT ARROW key is pressed, these menus expand or collapse when the SPACEBAR or ENTER key is pressed and the item has focus.
		OutlineItem        = ROLE_SYSTEM_OUTLINEITEM,       //!< An item in an outline or tree structure.
		PageTab            = ROLE_SYSTEM_PAGETAB,           //!< A page tab. The only child of a page tab control is a ROLE_SYSTEM_GROUPING object that has the contents of the associated page.
		PageTabList        = ROLE_SYSTEM_PAGETABLIST,       //!< A container of page tab controls.
		Pane               = ROLE_SYSTEM_PANE,              //!< A pane within a frame or a document window. Users can navigate between panes and within the contents of the current pane, but cannot navigate between items in different panes. Thus, panes represent a grouping level that is lower than frames or document windows, but higher than individual controls. The user navigates between panes by pressing TAB, F6, or CTRL+TAB, depending on the context.
		ProgressBar        = ROLE_SYSTEM_PROGRESSBAR,       //!< A progress bar, which dynamically shows how much of an operation in progress has completed. This control takes no user input.
		PropertyPage       = ROLE_SYSTEM_PROPERTYPAGE,      //!< A property sheet.
		PushButton         = ROLE_SYSTEM_PUSHBUTTON,        //!< A push-button control.
		RadioButton        = ROLE_SYSTEM_RADIOBUTTON,       //!< An option button (formerly, a radio button). It is one of a group of mutually exclusive options. All objects that share the same parent and that have this attribute are assumed to be part of a single mutually exclusive group. To divide these objects into separate groups, use ROLE_SYSTEM_GROUPING objects.
		Row                = ROLE_SYSTEM_ROW,               //!< A row of cells within a table.
		RowHeader          = ROLE_SYSTEM_ROWHEADER,         //!< A row header, which provides a visual label for a table row.
		ScrollBar          = ROLE_SYSTEM_SCROLLBAR,         //!< A vertical or horizontal scroll bar, which is part of the client area or is used in a control.
		Separator          = ROLE_SYSTEM_SEPARATOR,         //!< The object is used to visually divide a space into two regions. Examples of separator objects include a separator menu item, and a bar that divides split panes within a window.
		Slider             = ROLE_SYSTEM_SLIDER,            //!< A slider, which allows the user to adjust a setting in particular increments between minimum and maximum values.
		Sound              = ROLE_SYSTEM_SOUND,             //!< A system sound, which is associated with various system events.
		SpinButton         = ROLE_SYSTEM_SPINBUTTON,        //!< A spin box, which is a control that allows the user to increment or decrement the value displayed in a separate "buddy" control that is associated with the spin box.
		SplitButton        = ROLE_SYSTEM_SPLITBUTTON,       //!< A button on a toolbar that has a drop-down list icon that is directly adjacent to the button.
		StaticText         = ROLE_SYSTEM_STATICTEXT,        //!< Read-only text, such as labels for other controls or instructions in a dialog box. Static text cannot be modified or selected.
		StatusBar          = ROLE_SYSTEM_STATUSBAR,         //!< A status bar, which is an area at the bottom of a window and which displays information about the current operation, state of the application, or selected object. The status bar has multiple fields, which display different kinds of information.
		Table              = ROLE_SYSTEM_TABLE,             //!< A table that contains rows and columns of cells, and, optionally, row headers and column headers.
		Text               = ROLE_SYSTEM_TEXT,              //!< Selectable text that allows edits or is designated as read-only.
		TitleBar           = ROLE_SYSTEM_TITLEBAR,          //!< A title or caption bar for a window.
		Toolbar            = ROLE_SYSTEM_TOOLBAR,           //!< A toolbar, which is a grouping of controls that provides easy access to frequently used features.
		Tooltip            = ROLE_SYSTEM_TOOLTIP,           //!< A tooltip that provides helpful hints.
		Whitespace         = ROLE_SYSTEM_WHITESPACE,        //!< Blank space between other objects.
		Window             = ROLE_SYSTEM_WINDOW,            //!< The window frame, which contains child objects such as a title bar, client, and other objects of a window.
	};
}
