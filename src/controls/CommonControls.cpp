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
	static Dialog::WindowClass [[maybe_unused]] DialogClass;
	static ButtonControl::WindowClass [[maybe_unused]] ButtonClass;
	static ComboBoxControl::WindowClass [[maybe_unused]] ComboBoxeClass;
	static EditControl::WindowClass [[maybe_unused]] EditClass;
	static ListBoxControl::WindowClass [[maybe_unused]] ListBoxeClass;
	static ListViewControl::WindowClass [[maybe_unused]] ListViewClass;
	static ProgressBarControl::WindowClass [[maybe_unused]] ProgressBarClass;
	static StaticControl::WindowClass [[maybe_unused]] StaticClass;
	
	// Optionally register ComCtl32.dll v6 controls
	if (this->version() >= com::Version{6,0})
		static LinkControl::WindowClass [[maybe_unused]] obj;
}
