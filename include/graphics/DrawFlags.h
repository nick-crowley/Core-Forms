#pragma once
#include "library/core.Forms.h"

namespace core::forms
{
    enum class EdgeFlags : uint32_t;
    enum class BorderFlags : uint32_t;
    enum class DrawTextFlags : uint32_t;
    enum class OwnerDrawState : uint16_t;
}

//! \enum EdgeFlags - Defines DrawEdge edge flags
enum class core::forms::EdgeFlags : uint32_t
{
    RaisedInner = BDR_RAISEDINNER,      //!< Raised inner edge.
    SunkenInner = BDR_SUNKENINNER,      //!< Sunken inner edge.

    RaisedOuter = BDR_RAISEDOUTER,      //!< Raised outer edge.
    SunkenOuter = BDR_SUNKENOUTER,      //!< Sunken outer edge.

    Bump = EDGE_BUMP,          //!< Combination of RaisedOuter and SunkenInner.
    Etched = EDGE_ETCHED,      //!< Combination of SunkenOuter and RaisedInner.
    Raised = EDGE_RAISED,      //!< Combination of RaisedOuter and RaisedInner.
    Sunken = EDGE_SUNKEN,      //!< Combination of SunkenOuter and SunkenInner.

    None = 0,
};

metadata bool core::meta::Settings<core::bitwise_enum, core::forms::EdgeFlags> = true;

//! \enum BorderFlags - Defines DrawText border flags
enum class core::forms::BorderFlags : uint32_t
{
    Adjust = BF_ADJUST,                                 //!< Shrink the passed rectangle to exclude the edges that were drawn
    Bottom = BF_BOTTOM,                                 //!< Bottom of border rectangle.
    BottomLeft = BF_BOTTOMLEFT,                         //!< Bottom and left side of border rectangle.
    BottomRight = BF_BOTTOMRIGHT,                       //!< Bottom and right side of border rectangle.
    Diagonal = BF_DIAGONAL,                             //!< Diagonal border.
    DiagonalBottomLeft = BF_DIAGONAL_ENDBOTTOMLEFT,     //!< Diagonal border. The end point is the lower-left corner of the rectangle; the origin is top-right corner.
    DiagonalBottomRight = BF_DIAGONAL_ENDBOTTOMRIGHT,   //!< Diagonal border. The end point is the lower-right corner of the rectangle; the origin is top-left corner.
    DiagonalTopLeft = BF_DIAGONAL_ENDTOPLEFT,           //!< Diagonal border. The end point is the top-left corner of the rectangle; the origin is lower-right corner.
    DiagonalTopRight = BF_DIAGONAL_ENDTOPRIGHT,         //!< Diagonal border. The end point is the top-right corner of the rectangle; the origin is lower-left corner.
    Flag = BF_FLAT,                                     //!< Flat border.
    Left = BF_LEFT,                                     //!< Left side of border rectangle.
    Middle = BF_MIDDLE,                                 //!< Interior of rectangle to be filled.
    Mono = BF_MONO,                                     //!< One-dimensional border.
    Rect = BF_RECT,                                     //!< Entire border rectangle.
    Right = BF_RIGHT,                                   //!< Right side of border rectangle.
    Soft = BF_SOFT,                                     //!< Soft buttons instead of tiles.
    Top = BF_TOP,                                       //!< Top of border rectangle.
    TopLeft = BF_TOPLEFT,                               //!< Top and left side of border rectangle.
    TopRight = BF_TOPRIGHT,                             //!< Top and right side of border rectangle.
};

metadata bool core::meta::Settings<core::bitwise_enum, core::forms::BorderFlags> = true;

//! \enum DrawTextFlags - Defines DrawText flags
enum class core::forms::DrawTextFlags : uint32_t
{
    Top = DT_TOP,                                    //!< 
    Left = Top,                                      //!< 
    Centre = DT_CENTER,                              //!< 
    Right = DT_RIGHT,                                //!< 
    VCentre = DT_VCENTER,                            //!< 
    Bottom = DT_BOTTOM,                              //!< 
    WordBreak = DT_WORDBREAK,                        //!< 
    SingleLine = DT_SINGLELINE,                      //!< 
    ExpandTabs = DT_EXPANDTABS,                      //!< 
    TabStop = DT_TABSTOP,                            //!< 
    NoClip = DT_NOCLIP,                              //!< 
    ExternalLeading = DT_EXTERNALLEADING,            //!< 
    CalcRect = DT_CALCRECT,                          //!< 
    NoPrefix = DT_NOPREFIX,                          //!< 
    Internal = DT_INTERNAL,                          //!< 
    EditControl = DT_EDITCONTROL,                    //!< 
    PathEllipsis = DT_PATH_ELLIPSIS,                 //!< 
    EndElipsis = DT_END_ELLIPSIS,                    //!< 
    ModifyString = DT_MODIFYSTRING,                  //!< 
    RtlReading = DT_RTLREADING,                      //!< 
    WordElipsis = DT_WORD_ELLIPSIS,                  //!< 
    NoFullWidthCharBreak = DT_NOFULLWIDTHCHARBREAK,  //!< [windows 5.00]
    HidePrefix = DT_HIDEPREFIX,                      //!< [windows 5.00]
    PrefixOnly = DT_PREFIXONLY,                      //!< [windows 5.00]

    SimpleLeft = Left|VCentre|SingleLine,
    SimpleCentre = Centre|VCentre|SingleLine,
    SimpleRight = Right|VCentre|SingleLine,
    None = Top
};
  
metadata bool core::meta::Settings<core::bitwise_enum, core::forms::DrawTextFlags> = true;

namespace core::forms
{
    //! \enum DrawingMode - Defines background drawing modes
    enum class DrawingMode : uint32_t
    {
        Invalid = 0,                 //!< 
        Transparent = TRANSPARENT,   //!< Transparent background
        Opaque = OPAQUE,             //!< Opaque background
    };
  
    //! \enum DrawObjectType - Defines drawing object types 
    enum class DrawObjectType : uint32_t
    {
        Pen = OBJ_PEN,                       //!< 
        ExtendedPen = OBJ_EXTPEN,            //!< 
        Brush = OBJ_BRUSH,                   //!< 
        Font = OBJ_FONT,                     //!< 
        Bitmap = OBJ_BITMAP,                 //!< 
        Region = OBJ_REGION,                 //!< 
        Palette = OBJ_PAL,                   //!< 
        ColorSpace = OBJ_COLORSPACE,         //!< 
        DeviceContext = OBJ_DC,              //!< 
        MemoryDC = OBJ_MEMDC,                //!< 
        Metafile = OBJ_METAFILE,             //!< 
        MetafileDC = OBJ_METADC,             //!< 
        EnhancedMetafile = OBJ_ENHMETAFILE,  //!< 
        EnhancedMetafileDC = OBJ_ENHMETADC,  //!< 
    };
  
    //! \enum OwnerDrawAction - Defines owner drawing requests
    enum class OwnerDrawAction : uint32_t
    {
        None = 0,
        Entire = ODA_DRAWENTIRE,   //!< Entire control
        Focus = ODA_FOCUS,         //!< Focus rectange
        Select = ODA_SELECT,       //!< Selection status has changed
    };
  
    //! \enum OwnerDrawControl - Defines controls that support owner draw
    enum class OwnerDrawControl : uint32_t
    {
        Unknown = NULL,           //!< Unspecified
        Button = ODT_BUTTON,      //!< Button control
        ComboBox = ODT_COMBOBOX,  //!< ComboBox control
        ListBox = ODT_LISTBOX,    //!< ListBox control
        ListView = ODT_LISTVIEW,  //!< ListView control
        Menu = ODT_MENU,          //!< Window Menu 
        Static = ODT_STATIC,      //!< Static control
        Tab = ODT_TAB,            //!< Tab control
    };
}

//! \enum OwnerDrawState - Defines controls that support owner draw
enum class core::forms::OwnerDrawState : uint16_t
{
    None = 0,
    Checked = ODS_CHECKED,              //!< The menu item is to be checked. This bit is used only in a menu.
    ComboBoxEdit = ODS_COMBOBOXEDIT,    //!< The drawing takes place in the selection field (edit control) of an owner-drawn combo box.
    Default = ODS_DEFAULT,              //!< The item is the default item.
    Disabled = ODS_DISABLED,            //!< The item is to be drawn as disabled.
    Focus = ODS_FOCUS,                  //!< The item has the keyboard focus.
    Grayed = ODS_GRAYED,                //!< The item is to be grayed. This bit is used only in a menu.
    Hotlight = ODS_HOTLIGHT,            //!< The item is being hot-tracked, that is, the item will be highlighted when the mouse is on the item.
    Inactive = ODS_INACTIVE,            //!< The item is inactive and the window associated with the menu is inactive.
    NoAccel = ODS_NOACCEL,              //!< The control is drawn without the keyboard accelerator cues.
    NoFocusRect = ODS_NOFOCUSRECT,      //!< The control is drawn without focus indicator cues.
    Selected = ODS_SELECTED,            //!< The menu item's status is selected.
};
  
metadata bool core::meta::Settings<core::bitwise_enum, core::forms::OwnerDrawState> = true;

namespace core::forms
{
    //! \enum RasterOp - Defines operations for combining bitmap pixels
    enum class RasterOp : uint32_t
    {
        Blackness       = BLACKNESS,      //!< Fills the destination rectangle using the color associated with index 0 in the physical palette. (This color is black for the default physical palette.)
        CaptureBlt      = CAPTUREBLT,     //!< Includes any windows that are layered on top of your window in the resulting image. By default, the image only contains your window. Note that this generally cannot be used for printing device contexts.
        DstInvert       = DSTINVERT,      //!< Inverts the destination rectangle.
        MergeCopy       = MERGECOPY,      //!< Merges the colors of the source rectangle with the brush currently selected in hdcDest, by using the Boolean AND operator.
        MergePaint      = MERGEPAINT,     //!< Merges the colors of the inverted source rectangle with the colors of the destination rectangle by using the Boolean OR operator.
        NoMirrorBitmap  = NOMIRRORBITMAP, //!< Prevents the bitmap from being mirrored.
        NotSrcCopy      = NOTSRCCOPY,     //!< Copies the inverted source rectangle to the destination.
        NotSrcErase     = NOTSRCERASE,    //!< Combines the colors of the source and destination rectangles by using the Boolean OR operator and then inverts the resultant color.
        PatCopy         = PATCOPY,        //!< Copies the brush currently selected in hdcDest, into the destination bitmap.
        PatInvert       = PATINVERT,      //!< Combines the colors of the brush currently selected in hdcDest, with the colors of the destination rectangle by using the Boolean XOR operator.
        PatPaint        = PATPAINT,       //!< Combines the colors of the brush currently selected in hdcDest, with the colors of the inverted source rectangle by using the Boolean OR operator. The result of this operation is combined with the colors of the destination rectangle by using the Boolean OR operator.
        SrcAnd          = SRCAND,         //!< Combines the colors of the source and destination rectangles by using the Boolean AND operator.
        SrcCopy         = SRCCOPY,        //!< Copies the source rectangle directly to the destination rectangle.
        SrcErase        = SRCERASE,       //!< Combines the inverted colors of the destination rectangle with the colors of the source rectangle by using the Boolean AND operator.
        SrcInvert       = SRCINVERT,      //!< Combines the colors of the source and destination rectangles by using the Boolean XOR operator.
        SrcPaint        = SRCPAINT,       //!< Combines the colors of the source and destination rectangles by using the Boolean OR operator.
        Whiteness       = WHITENESS,      //!< Fills the destination rectangle using the color associated with index 1 in the physical palette. (This color is white for the default physical palette.)
    };

    //! \enum StockObject - Defines stock drawing objects
    enum class StockObject : int32_t
    {
        WhiteBrush = WHITE_BRUSH,                 //!< 
        LtGreyBrush = LTGRAY_BRUSH,               //!< 
        GreyBrush = GRAY_BRUSH,                   //!< 
        DkGreyBrush = DKGRAY_BRUSH,               //!< 
        BlackBrush = BLACK_BRUSH,                 //!< 
        HollowBrush = HOLLOW_BRUSH,               //!< 
        WhitePen = WHITE_PEN,                     //!< 
        BlackPen = BLACK_PEN,                     //!< 
        HollowPen = NULL_PEN,                     //!< 
        OemFixedFont = OEM_FIXED_FONT,            //!< 
        AnsiFixedFont = ANSI_FIXED_FONT,          //!< 
        AnsiVarFont = ANSI_VAR_FONT,              //!< 
        SystemFont = SYSTEM_FONT,                 //!< 
        DeviceDefaultFont = DEVICE_DEFAULT_FONT,  //!< 
        DefaultPalette = DEFAULT_PALETTE,         //!< 
        SystemFixedFont = SYSTEM_FIXED_FONT,      //!< 
        DefaultGuiFont = DEFAULT_GUI_FONT,        //!< 
        InternalBrush = DC_BRUSH,                 //!< [windows 2000]
        InternalPen = DC_PEN,                     //!< [windows 2000]
    };

    //! \enum SystemMetric - System metrics
    enum class SystemMetric
    {
        Arrange = SM_ARRANGE,                                //!< The flags that specify how the system arranged minimized windows. For more information, see the Remarks section in this topic.
        CleanBoot = SM_CLEANBOOT,                            //!< The value that specifies how the system is started: A fail-safe boot (also called SafeBoot, Safe Mode, or Clean Boot) bypasses the user startup files.
        cMonitors = SM_CMONITORS,                            //!< The number of display monitors on a desktop. For more information, see the Remarks section in this topic. 
        cMouseButtons = SM_CMOUSEBUTTONS,                    //!< The number of buttons on a mouse, or zero if no mouse is installed.
        //ConvertibleSlateMode = SM_CONVERTIBLESLATEMODE,    //!<    Reflects the state of the laptop or slate mode, 0 for Slate Mode and non-zero otherwise. When this system metric changes, the system sends a broadcast message via WM_SETTINGCHANGE with "ConvertibleSlateMode" in the LPARAM. Note that this system metric doesn't apply to desktop PCs. In that case, use GetAutoRotationState.

        cxBorder = SM_CXBORDER,                               //!< The width of a window border, in pixels. This is equivalent to the SM_CXEDGE value for windows with the 3-D look.
        cxCursor = SM_CXCURSOR,                               //!< The width of a cursor, in pixels. The system cannot create cursors of other sizes.
        cxDialogFrame = SM_CXDLGFRAME,                        //!< This value is the same as SM_CXFIXEDFRAME.
        cxDoubleClick = SM_CXDOUBLECLK,                       //!< The width of the rectangle around the location of a first click in a double-click sequence, in pixels. The second click must occur within the rectangle that is defined by SM_CXDOUBLECLK and SM_CYDOUBLECLK for the system to consider the two clicks a double-click. The two clicks must also occur within a specified time. To set the width of the double-click rectangle, call SystemParametersInfo with SPI_SETDOUBLECLKWIDTH.
        cxDrag = SM_CXDRAG,                                   //!< The number of pixels on either side of a mouse-down point that the mouse pointer can move before a drag operation begins. This allows the user to click and release the mouse button easily without unintentionally starting a drag operation. If this value is negative, it is subtracted from the left of the mouse-down point and added to the right of it.
        cxEdge = SM_CXEDGE,                                   //!< The width of a 3-D border, in pixels. This metric is the 3-D counterpart of SM_CXBORDER.
        cxFixedFrame = SM_CXFIXEDFRAME,                       //!< The thickness of the frame around the perimeter of a window that has a caption but is not sizable, in pixels. SM_CXFIXEDFRAME is the height of the horizontal border, and SM_CYFIXEDFRAME is the width of the vertical border. This value is the same as SM_CXDLGFRAME.
        cxFocusBorder = SM_CXFOCUSBORDER,                     //!< [Win XP] The width of the left and right edges of the focus rectangle that the DrawFocusRect draws. This value is in pixels. Windows 2000:  This value is not supported.
        cxFrame = SM_CXFRAME,                                 //!< This value is the same as SM_CXSIZEFRAME.
        cxFullScreen = SM_CXFULLSCREEN,                       //!< The width of the client area for a full-screen window on the primary display monitor, in pixels. To get the coordinates of the portion of the screen that is not obscured by the system taskbar or by application desktop toolbars, call the SystemParametersInfo function with the SPI_GETWORKAREA value.
        cxHScroll = SM_CXHSCROLL,                             //!< The width of the arrow bitmap on a horizontal scroll bar, in pixels.
        cxHThumb = SM_CXHTHUMB,                               //!< The width of the thumb box in a horizontal scroll bar, in pixels.
        cxIcon = SM_CXICON,                                   //!< The default width of an icon, in pixels. The LoadIcon function can load only icons with the dimensions that SM_CXICON and SM_CYICON specifies.
        cxIconSpacing = SM_CXICONSPACING,                     //!< The width of a grid cell for items in large icon view, in pixels. Each item fits into a rectangle of size SM_CXICONSPACING by SM_CYICONSPACING when arranged. This value is always greater than or equal to SM_CXICON.
        cxMaximized = SM_CXMAXIMIZED,                         //!< The default width, in pixels, of a maximized top-level window on the primary display monitor.
        cxMaxTrack = SM_CXMAXTRACK,                           //!< The default maximum width of a window that has a caption and sizing borders, in pixels. This metric refers to the entire desktop. The user cannot drag the window frame to a size larger than these dimensions. A window can override this value by processing the WM_GETMINMAXINFO message.
        cxMenuCheck = SM_CXMENUCHECK,                         //!< The width of the default menu check-mark bitmap, in pixels.
        cxMenuSize = SM_CXMENUSIZE,                           //!< The width of menu bar buttons, such as the child window close button that is used in the multiple document interface, in pixels.
        cxMinWindow = SM_CXMIN,                               //!< The minimum width of a window, in pixels.
        cxMinimized = SM_CXMINIMIZED,                         //!< The width of a minimized window, in pixels.
        cxMinSpacing = SM_CXMINSPACING,                       //!< The width of a grid cell for a minimized window, in pixels. Each minimized window fits into a rectangle this size when arranged. This value is always greater than or equal to SM_CXMINIMIZED.
        cxMinTrack = SM_CXMINTRACK,                           //!< The minimum tracking width of a window, in pixels. The user cannot drag the window frame to a size smaller than these dimensions. A window can override this value by processing the WM_GETMINMAXINFO message.
        //cxPaddedBorder = SM_CXPADDEDBORDER,                 //!< [¬XP/2000] The amount of border padding for captioned windows, in pixels. Windows XP/2000:  This value is not supported.
        cxScreen = SM_CXSCREEN,                               //!< The width of the screen of the primary display monitor, in pixels. This is the same value obtained by calling GetDeviceCaps as follows: GetDeviceCaps( hdcPrimaryMonitor, HORZRES).
        cxSizeButton = SM_CXSIZE,                             //!< The width of a button in a window caption or title bar, in pixels.
        cxSizeFrame = SM_CXSIZEFRAME,                         //!< The thickness of the sizing border around the perimeter of a window that can be resized, in pixels. SM_CXSIZEFRAME is the width of the horizontal border, and SM_CYSIZEFRAME is the height of the vertical border. This value is the same as SM_CXFRAME.
        cxSmallIcon = SM_CXSMICON,                            //!< The recommended width of a small icon, in pixels. Small icons typically appear in window captions and in small icon view.
        cxSmallButton = SM_CXSMSIZE,                          //!< The width of small caption buttons, in pixels.
        cxVirtualScreen = SM_CXVIRTUALSCREEN,                 //!< The width of the virtual screen, in pixels. The virtual screen is the bounding rectangle of all display monitors. The SM_XVIRTUALSCREEN metric is the coordinates for the left side of the virtual screen. 
        cxVScroll = SM_CXVSCROLL,                             //!< The width of a vertical scroll bar, in pixels.

        cyBorder = SM_CYBORDER,                               //!< The height of a window border, in pixels. This is equivalent to the SM_CYEDGE value for windows with the 3-D look.
        cyCaption = SM_CYCAPTION,                             //!< The height of a caption area, in pixels.
        cyCursor = SM_CYCURSOR,                               //!< The height of a cursor, in pixels. The system cannot create cursors of other sizes.
        cyDialogFrame = SM_CYDLGFRAME,                        //!< This value is the same as SM_CYFIXEDFRAME.
        cyDoubleClick = SM_CYDOUBLECLK,                       //!< The height of the rectangle around the location of a first click in a double-click sequence, in pixels. The second click must occur within the rectangle defined by SM_CXDOUBLECLK and SM_CYDOUBLECLK for the system to consider the two clicks a double-click. The two clicks must also occur within a specified time. To set the height of the double-click rectangle, call SystemParametersInfo with SPI_SETDOUBLECLKHEIGHT.
        cyDrag = SM_CYDRAG,                                   //!< The number of pixels above and below a mouse-down point that the mouse pointer can move before a drag operation begins. This allows the user to click and release the mouse button easily without unintentionally starting a drag operation. If this value is negative, it is subtracted from above the mouse-down point and added below it. 
        cyEdge = SM_CYEDGE,                                   //!< The height of a 3-D border, in pixels. This is the 3-D counterpart of SM_CYBORDER.
        cyFixedFrame = SM_CYFIXEDFRAME,                       //!< The thickness of the frame around the perimeter of a window that has a caption but is not sizable, in pixels. SM_CXFIXEDFRAME is the height of the horizontal border, and SM_CYFIXEDFRAME is the width of the vertical border.This value is the same as SM_CYDLGFRAME.
        cyFocusBorder = SM_CYFOCUSBORDER,                     //!< [¬Win 2000] The height of the top and bottom edges of the focus rectangle drawn by DrawFocusRect. This value is in pixels. Windows 2000:  This value is not supported.
        cyFrame = SM_CYFRAME,                                 //!< This value is the same as SM_CYSIZEFRAME.
        cyFullScreen = SM_CYFULLSCREEN,                       //!< The height of the client area for a full-screen window on the primary display monitor, in pixels. To get the coordinates of the portion of the screen not obscured by the system taskbar or by application desktop toolbars, call the SystemParametersInfo function with the SPI_GETWORKAREA value.
        cyHScroll = SM_CYHSCROLL,                             //!< The height of a horizontal scroll bar, in pixels.
        cyIcon = SM_CYICON,                                   //!< The default height of an icon, in pixels. The LoadIcon function can load only icons with the dimensions SM_CXICON and SM_CYICON.
        cyIconSpacing = SM_CYICONSPACING,                     //!< The height of a grid cell for items in large icon view, in pixels. Each item fits into a rectangle of size SM_CXICONSPACING by SM_CYICONSPACING when arranged. This value is always greater than or equal to SM_CYICON.
        cyKanjiWindow = SM_CYKANJIWINDOW,                     //!< For double byte character set versions of the system, this is the height of the Kanji window at the bottom of the screen, in pixels.
        cyMaximized = SM_CYMAXIMIZED,                         //!< The default height, in pixels, of a maximized top-level window on the primary display monitor.
        cyMaxTrack = SM_CYMAXTRACK,                           //!< The default maximum height of a window that has a caption and sizing borders, in pixels. This metric refers to the entire desktop. The user cannot drag the window frame to a size larger than these dimensions. A window can override this value by processing the WM_GETMINMAXINFO message.
        cyMenu = SM_CYMENU,                                   //!< The height of a single-line menu bar, in pixels.
        cyMenuCheck = SM_CYMENUCHECK,                         //!< The height of the default menu check-mark bitmap, in pixels.
        cyMenuSize = SM_CYMENUSIZE,                           //!< The height of menu bar buttons, such as the child window close button that is used in the multiple document interface, in pixels.
        cyMinWindow = SM_CYMIN,                               //!< The minimum height of a window, in pixels.
        cyMinimized = SM_CYMINIMIZED,                         //!< The height of a minimized window, in pixels.
        cyMinSpacing = SM_CYMINSPACING,                       //!< The height of a grid cell for a minimized window, in pixels. Each minimized window fits into a rectangle this size when arranged. This value is always greater than or equal to SM_CYMINIMIZED.
        cyMinTrack = SM_CYMINTRACK,                           //!< The minimum tracking height of a window, in pixels. The user cannot drag the window frame to a size smaller than these dimensions. A window can override this value by processing the WM_GETMINMAXINFO message.
        cyScreen = SM_CYSCREEN,                               //!< The height of the screen of the primary display monitor, in pixels. This is the same value obtained by calling GetDeviceCaps as follows: GetDeviceCaps( hdcPrimaryMonitor, VERTRES).
        cySizeButton = SM_CYSIZE,                             //!< The height of a button in a window caption or title bar, in pixels.
        cySizeFrame = SM_CYSIZEFRAME,                         //!< The thickness of the sizing border around the perimeter of a window that can be resized, in pixels. SM_CXSIZEFRAME is the width of the horizontal border, and SM_CYSIZEFRAME is the height of the vertical border. This value is the same as SM_CYFRAME.
        cySmallCaption = SM_CYSMCAPTION,                      //!< The height of a small caption, in pixels.
        cySmallIcon = SM_CYSMICON,                            //!< The recommended height of a small icon, in pixels. Small icons typically appear in window captions and in small icon view.
        cySmallButton = SM_CYSMSIZE,                          //!< The height of small caption buttons, in pixels.
        cyVirtualScreen = SM_CYVIRTUALSCREEN,                 //!< The height of the virtual screen, in pixels. The virtual screen is the bounding rectangle of all display monitors. The SM_YVIRTUALSCREEN metric is the coordinates for the top of the virtual screen.
        cyVScroll = SM_CYVSCROLL,                             //!< The height of the arrow bitmap on a vertical scroll bar, in pixels.
        cyVThumb = SM_CYVTHUMB,                               //!< The height of the thumb box in a vertical scroll bar, in pixels.

        DbcsEnabled = SM_DBCSENABLED,                         //!< Nonzero if User32.dll supports DBCS; otherwise, 0. 
        Debug = SM_DEBUG,                                     //!< Nonzero if the debug version of User.exe is installed; otherwise, 0.
        //Digitizer = SM_DIGITIZER,                           //!< Nonzero if the current operating system is Windows 7 or Windows Server 2008 R2 and the Tablet PC Input service is started; otherwise, 0. The return value is a bitmask that specifies the type of digitizer input supported by the device. For more information, see Remarks. Windows Server 2008, Windows Vista, and Windows XP/2000:  This value is not supported.
        ImeEnabled = SM_IMMENABLED,                           //!< Nonzero if Input Method Manager/Input Method Editor features are enabled; otherwise, 0. SM_IMMENABLED indicates whether the system is ready to use a Unicode-based IME on a Unicode application. To ensure that a language-dependent IME works, check SM_DBCSENABLED and the system ANSI code page. Otherwise the ANSI-to-Unicode conversion may not be performed correctly, or some components like fonts or registry settings may not be present.
        //MAXIMUMTOUCHES = SM_MAXIMUMTOUCHES,                 //!< Nonzero if there are digitizers in the system; otherwise, 0. SM_MAXIMUMTOUCHES returns the aggregate maximum of the maximum number of contacts supported by every digitizer in the system. If the system has only single-touch digitizers, the return value is 1. If the system has multi-touch digitizers, the return value is the number of simultaneous contacts the hardware can provide. Windows Server 2008, Windows Vista, and Windows XP/2000:  This value is not supported.
    };
}