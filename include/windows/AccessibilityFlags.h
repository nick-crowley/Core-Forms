#pragma once
#include "library/core.Forms.h"

namespace core::forms 
{
	enum class WindowRole {
		Alert,              //!< An alert or a condition that a user should be notified about. This role is used only for objects that embody an alert but are not associated with another user interface element, such as a message box, graphic, text, or sound.
		Animation,          //!< An animation control whose content changes over time, such as a control that displays a series of bitmap frames. Animation controls are displayed when files are copied or when some other time-consuming task is performed.
		Application,        //!< A main window for an application.
		Border,             //!< A window border. The entire border is represented by a single object rather than by separate objects for each side.
		ButtonDropdown,     //!< A button that expands a list of items.
		ButtonDropdownGrid, //!< A button that expands a grid.
		ButtonMenu,         //!< A button that expands a menu.
		Caret,              //!< The system caret.
		Cell,               //!< A cell within a table.
		Character,          //!< A cartoon-like graphic object, such as Microsoft Office Assistant, which is displayed to provide help to users of an application.
		Chart,              //!< A graphical image used to chart data.
		CheckButton,        //!< A check box control: an option that is selected or cleared independently of other options.
		Client,             //!< A window's client area. Microsoft Active Accessibility uses this role as a default if there is a question about the role of a UI element.
		Clock,              //!< A control that displays time.
		Column,             //!< A column of cells within a table.
		ColumnHeader,       //!< A column header, providing a visual label for a column in a table.
		ComboBox,           //!< A combo box: an edit control with an associated list box that provides a set of predefined choices.
		Cursor,             //!< The system's mouse pointer.
		Diagram,            //!< A graphical image that is used to diagram data.
		Dial,               //!< A dial or knob.
		Dialog,             //!< A dialog box or message box.
		Document,           //!< A document window. A document window is always contained within an application window. This role applies only to MDI windows and refers to the object that contains the MDI title bar.
		DropList,           //!< The calendar control or SysDateTimePick32. The Microsoft Active Accessibility runtime component uses this role to indicate that either a date or a calendar control has been found.
		Equation,           //!< A mathematical equation.
		Graphic,            //!< A picture.
		Grip,               //!< A special mouse pointer that allows a user to manipulate user interface elements such as windows. One example of this involves resizing a window by dragging its lower-right corner.
		Grouping,           //!< Logically groups other objects. There is not always a parent-child relationship between the grouping object and the objects it contains.
		HelpBalloon,        //!< Displays a help topic in the form of a tooltip or help balloon.
		HotKeyField,        //!< A keyboard shortcut field that allows the user to enter a combination or sequence of keystrokes.
		Indicator,          //!< An indicator, such as a pointer graphic, that points to the current item.
		IpAddress,          //!< An edit control that is designed for an IP address. The edit control is divided into sections, each for a specific part of the IP address.
		Link,               //!< A link to something else. This object might look like text or a graphic, but it acts like a button.
		List,               //!< A list box, allowing the user to select one or more items.
		ListItem,           //!< An item in a list box or in the list portion of a combo box, drop-down list box, or drop-down combo box.
		MenuBar,            //!< The menu bar (positioned beneath the title bar of a window) from which users select menus.
		MenuItem,           //!< A menu item: an menu entry that the user can choose to carry out a command, select an option, or display another menu. Functionally, a menu item is equivalent to a push button, a radio button, a check box, or a menu.
		MenuPopup,          //!< A menu: a list of options, each with a specific action. All menu types must have role, including the drop-down menus which are displayed when selected from a menu bar; and shortcut menus, which are displayed by clicking the right mouse button.
		Outline,            //!< An outline or a tree structure, such as a tree view control, that displays a hierarchical list and allows the user to expand and collapse branches.
		OutlineButton,      //!< An item that navigates like an outline item. The UP and DOWN ARROW keys are used to navigate through the outline. However, instead of expanding and collapsing when the LEFT and RIGHT ARROW key is pressed, these menus expand or collapse when the SPACEBAR or ENTER key is pressed and the item has focus.
		OutlineItem,        //!< An item in an outline or tree structure.
		PageTab,            //!< A page tab. The only child of a page tab control is a ROLE_SYSTEM_GROUPING object that has the contents of the associated page.
		PageTabList,        //!< A container of page tab controls.
		Pane,               //!< A pane within a frame or a document window. Users can navigate between panes and within the contents of the current pane, but cannot navigate between items in different panes. Thus, panes represent a grouping level that is lower than frames or document windows, but higher than individual controls. The user navigates between panes by pressing TAB, F6, or CTRL+TAB, depending on the context.
		ProgressBar,        //!< A progress bar, which dynamically shows how much of an operation in progress has completed. This control takes no user input.
		PropertyPage,       //!< A property sheet.
		PushButton,         //!< A push-button control.
		RadioButton,        //!< An option button (formerly, a radio button). It is one of a group of mutually exclusive options. All objects that share the same parent and that have this attribute are assumed to be part of a single mutually exclusive group. To divide these objects into separate groups, use ROLE_SYSTEM_GROUPING objects.
		Row,                //!< A row of cells within a table.
		RowHeader,          //!< A row header, which provides a visual label for a table row.
		ScrollBar,          //!< A vertical or horizontal scroll bar, which is part of the client area or is used in a control.
		Separator,          //!< The object is used to visually divide a space into two regions. Examples of separator objects include a separator menu item, and a bar that divides split panes within a window.
		Slider,             //!< A slider, which allows the user to adjust a setting in particular increments between minimum and maximum values.
		Sound,              //!< A system sound, which is associated with various system events.
		SpinButton,         //!< A spin box, which is a control that allows the user to increment or decrement the value displayed in a separate "buddy" control that is associated with the spin box.
		SplitButton,        //!< A button on a toolbar that has a drop-down list icon that is directly adjacent to the button.
		StaticText,         //!< Read-only text, such as labels for other controls or instructions in a dialog box. Static text cannot be modified or selected.
		StatusBar,          //!< A status bar, which is an area at the bottom of a window and which displays information about the current operation, state of the application, or selected object. The status bar has multiple fields, which display different kinds of information.
		Table,              //!< A table that contains rows and columns of cells, and, optionally, row headers and column headers.
		Text,               //!< Selectable text that allows edits or is designated as read-only.
		TitleBar,           //!< A title or caption bar for a window.
		Toolbar,            //!< A toolbar, which is a grouping of controls that provides easy access to frequently used features.
		Tooltip,            //!< A tooltip that provides helpful hints.
		Whitespace,         //!< Blank space between other objects.
		Window,             //!< The window frame, which contains child objects such as a title bar, client, and other objects of a window.
	};
}
