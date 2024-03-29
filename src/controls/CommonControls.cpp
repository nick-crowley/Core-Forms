#include "controls/CommonControls.h"
#include "dialogs/Dialog.h"
using namespace core;
using namespace forms;

forms::detail::ComCtl32Registration::ComCtl32Registration() 
{
	// Query version of ComCtl32.dll 
	win::SharedLibrary libCommonControls{L"ComCtl32.dll"};
	if (auto dllGetVersion = libCommonControls.loadFunction<::HRESULT __stdcall(::DLLVERSIONINFO*)>("DllGetVersion"); !dllGetVersion) 
		throw runtime_error{"ComCtl32.dll is corrupt"};
	else {
		::DLLVERSIONINFO2 dv{sizeof ::DLLVERSIONINFO2};
		if (win::HResult hr = dllGetVersion(&dv.info1); !hr)
			hr.throwIfError("Failed to query version of ComCtl32.dll");
		this->Current = com::Version{(uint16_t)dv.info1.dwMajorVersion, (uint16_t)dv.info1.dwMinorVersion};
		clog << core::Verbose{"Running with ComCtl32.dll = v{}.{}", this->Current.Major, this->Current.Minor};
	}
				
	// Enable ComCtl32.dll v5 controls
	::INITCOMMONCONTROLSEX cmnControls{sizeof ::INITCOMMONCONTROLSEX, ICC_WIN95_CLASSES};
	if (!::InitCommonControlsEx(&cmnControls))
		win::LastError{}.throwIfError("Failed to enable Windows 95 common controls");

	// Enable ComCtl32.dll v6 controls
	if (this->Current >= com::Version{6,0}) 
		if (cmnControls.dwICC = ICC_LINK_CLASS; !::InitCommonControlsEx(&cmnControls))
			win::LastError{}.throwIfError("Failed to enable SysLink common controls");
}

ControlRegistration::ControlRegistration()
{
	Dialog::WindowClass const              [[maybe_unused]] static DialogClass;
	AnimationControl::WindowClass const    [[maybe_unused]] static AnimationClass;
	ButtonControl::WindowClass const       [[maybe_unused]] static ButtonClass;
	ComboBoxControl::WindowClass const     [[maybe_unused]] static ComboBoxeClass;
	EditControl::WindowClass const         [[maybe_unused]] static EditClass;
	ListBoxControl::WindowClass const      [[maybe_unused]] static ListBoxeClass;
	ListViewControl::WindowClass const     [[maybe_unused]] static ListViewClass;
	ProgressBarControl::WindowClass const  [[maybe_unused]] static ProgressBarClass;
	StaticControl::WindowClass const       [[maybe_unused]] static StaticClass;
	
	// Optionally register ComCtl32.dll v6 controls
	if (this->version() >= com::Version{6,0})
		static LinkControl::WindowClass [[maybe_unused]] obj;
}

CommonControl
forms::identifyControl(win::ResourceId className) noexcept {
	win::ResourceId constexpr  static CustomStatic(L"Custom.STATIC");
	win::ResourceId constexpr  static StandardStatic(WC_STATIC);
	win::ResourceId constexpr  static NumericStatic(ClassId::Static);
	win::ResourceId constexpr  static CustomEdit(L"Custom.EDIT");
	win::ResourceId constexpr  static StandardEdit(WC_EDIT);
	win::ResourceId constexpr  static NumericEdit(ClassId::Edit);
	win::ResourceId constexpr  static CustomButton(L"Custom.BUTTON");
	win::ResourceId constexpr  static StandardButton(WC_BUTTON);
	win::ResourceId constexpr  static NumericButton(ClassId::Button);
	win::ResourceId constexpr  static CustomComboBox(L"Custom.COMBOBOX");
	win::ResourceId constexpr  static StandardComboBox(WC_COMBOBOX);
	win::ResourceId constexpr  static NumericComboBox(ClassId::ComboBox);
	win::ResourceId constexpr  static CustomListBox(L"Custom.LISTBOX");
	win::ResourceId constexpr  static StandardListBox(WC_LISTBOX);
	win::ResourceId constexpr  static NumericListBox(ClassId::ListBox);
	win::ResourceId constexpr  static CustomScrollBar(L"Custom.SCROLLBAR");
	win::ResourceId constexpr  static StandardScrollBar(WC_SCROLLBAR);
	win::ResourceId constexpr  static NumericScrollBar(ClassId::ScrollBar);

	if (className == NumericButton || className == StandardButton || className == CustomButton)
		return CommonControl::Button;
	else if (className == NumericEdit || className == StandardEdit || className == CustomEdit)
		return CommonControl::Edit;
	else if (className == NumericStatic || className == StandardStatic || className == CustomStatic)
		return CommonControl::Static;
	else if (className == NumericListBox || className == StandardListBox || className == CustomListBox || className == win::ResourceId{L"ComboLBox"})
		return CommonControl::ListBox;
	else if (className == NumericComboBox || className == StandardComboBox || className == CustomComboBox)
		return CommonControl::ComboBox;
	else if (className == NumericScrollBar || className == StandardScrollBar || className == CustomScrollBar)
		return CommonControl::ScrollBar;
	else if (className == win::ResourceId{WC_LISTVIEW} || className == win::ResourceId{L"Custom.LISTVIEW"})
		return CommonControl::ListView;
	else if (className == win::ResourceId{WC_HEADER} || className == win::ResourceId{L"Custom.HEADER"})
		return CommonControl::Header;
	else if (className == win::ResourceId{WC_TREEVIEW} || className == win::ResourceId{L"Custom.TREEVIEW"})
		return CommonControl::TreeView;
	else if (className == win::ResourceId{WC_TABCONTROL} || className == win::ResourceId{L"Custom.TABS"})
		return CommonControl::Tabs;
	else if (className == win::ResourceId{STATUSCLASSNAME} || className == win::ResourceId{L"Custom.STATUSBAR"})
		return CommonControl::StatusBar;
	else if (className == win::ResourceId{TOOLBARCLASSNAME} || className == win::ResourceId{L"Custom.TOOLBAR"})
		return CommonControl::ToolBar;
	else if (className == win::ResourceId{PROGRESS_CLASS} || className == win::ResourceId{L"Custom.PROGRESSBAR"})
		return CommonControl::ProgressBar;
	else if (className == win::ResourceId{ANIMATE_CLASS} || className == win::ResourceId{L"Custom.ANIMATION"})
		return CommonControl::Animation;
	else if (className == win::ResourceId{WC_LINK} || className == win::ResourceId{L"Custom.LINK"})
		return CommonControl::Link;

	return CommonControl::Unknown;
}
