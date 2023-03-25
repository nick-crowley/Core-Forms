#pragma once
#include "windows/WindowStyle.h"

namespace core::forms
{
	enum class StaticStyle : uint32_t {
		Left				= SS_LEFT					, // -+  Left-aligns the text in the rectangle. Lines are wrapped. Words too wide to display are truncated.
		Centre				= SS_CENTER					, //  |  Centre-aligns the text in the rectangle. Lines are wrapped. Words too wide to display are truncated.
		Right				= SS_RIGHT					, //  |  Right-aligns the text in the rectangle. Lines are wrapped. Words too wide to display are truncated.
		BlackRect			= SS_BLACKRECT				, //  |  Rectangle filled with the current window frame color [default=black]
		GrayRect			= SS_GRAYRECT				, //  |  Rectangle filled with the current screen background color [defaul=grey]
		WhiteRect			= SS_WHITERECT				, //  |  Rectangle filled with the current window background color [default=white]
		BlackFrame			= SS_BLACKFRAME				, //  |  Box/frame filled with the current window frame color [default=black]
		GrayFrame			= SS_GRAYFRAME				, //  |  Box/frame filled with the current screen background color [defaul=grey]
		WhiteFrame			= SS_WHITEFRAME				, //  |  Box/frame filled with the current window background color [default=white]
														  //  +---> Mutually exclusive control type styles
		UserItem			= SS_USERITEM				, //  |  [Undocumented]
		Simple				= SS_SIMPLE					, //  |  Displays a single line of left-aligned text. The text line cannot be shortened or altered in any way. Also, if the control is disabled, the control does not gray its text.
		LeftNoWordWrap		= SS_LEFTNOWORDWRAP			, //  |  Displays left-aligns the text; tabs are expanded; words are not wrapped. Excess lines are clipped
		OwnerDraw			= SS_OWNERDRAW				, //  |  Parent is responsible for drawing the control
		Bitmap				= SS_BITMAP					, //  |  Displays a bitmap; control shrinks-to-fit [text is the resource-id]
		Icon				= SS_ICON					, //  |  Display an icon/cursor/animated-cursor in system-default size; control shrinks-to-fit  [text is the resource-id]
		EnhMetaFile			= SS_ENHMETAFILE			, //  |  Displays a vector; control is fixed-size so image is scaled [text is the resource-id]
		EtchedHorz			= SS_ETCHEDHORZ				, //  |  Draws the top/bottom edges using the EDGE_ETCHED edge style
		EtchedVert			= SS_ETCHEDVERT				, //  |  Draws the left/right edges using the EDGE_ETCHED edge style
		EtchedFrame			= SS_ETCHEDFRAME			, // -+  Draws the frame using the EDGE_ETCHED edge style
		TypeMask			= SS_TYPEMASK				, // Mask for control type style-bits

		RealSizeControl		= SS_REALSIZECONTROL		, // Bitmap: Stretch bitmap to fit the control. Text is the resource ID (not a filename) of the bitmap.
		NoPrefix			= SS_NOPREFIX				, // Prevents interpretation of any ampersand (&) characters in the control's text as accelerator prefix characters.
		Notify				= SS_NOTIFY					, // Sends STN_CLICKED, STN_DBLCLK, STN_DISABLE, and STN_ENABLE when the user clicks or double-clicks the control.
		CentreImage			= SS_CENTERIMAGE			, // Bitmap: Centre the image
														  // Text: Centre the text vertically
		RightJust			= SS_RIGHTJUST				, // {Something about resizing the control when displaying bitmaps/icons}
		RealSizeImage		= SS_REALSIZEIMAGE			, // [Icons] Don't load the icon using system-default size
		Sunken				= SS_SUNKEN					, // Draws a half-sunken border around a static control
		EditControl			= SS_EDITCONTROL			, // Average character width is calculated in the same manner as with an edit control, and the function does not display a partially visible last line.
		EndEllipsis			= SS_ENDELLIPSIS			, // -+ Only truncates the final word if it does not fit in the rectangle [disables word-wrap onto multiple lines]
		PathEllipsis		= SS_PATHELLIPSIS			, //  | Replaces characters in the middle of the string with ellipses so that the result fits in the specified rectangle [disables word-wrap onto multiple lines]
		WordEllipsis		= SS_WORDELLIPSIS			, // -+ Truncates any word that does not fit in the rectangle with ellipses [disables word-wrap onto multiple lines]
		EllipsisMask		= SS_ELLIPSISMASK			, // Mask for ellipsis style-bits
	
		None				= Left
	};
}	// namespace core::forms

namespace core::meta 
{
	metadata bool Settings<bitwise_enum, core::forms::StaticStyle> = true;
	metadata bool Settings<compatible_enum, core::forms::StaticStyle, core::forms::WindowStyle> = true;
	metadata bool Settings<compatible_enum, core::forms::WindowStyle, core::forms::StaticStyle> = true;
}
