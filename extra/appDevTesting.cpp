#include "library/core.forms.h"
#include "dialogs/Dialog.h"
#include "Resource.h"
using namespace core;

// Define program meta-data
metadata std::string_view meta::Settings<program_name> = "Core.Forms Dev-Testing Application";
metadata std::string_view meta::Settings<program_version> = "Demo";

// Define primitive form capable of running "IDD_DEVTEST"
class DevTesting : public forms::Dialog
{
	using base = forms::Dialog;
	
private:
	forms::ButtonControl OkBtn = IDOK;
	forms::ComboBoxControl ComboBox = IDC_COMBO1;
	forms::ListBoxControl ListBox = IDC_LIST1;

public:
	DevTesting() : base{win::ResourceId{IDD_DEVTEST}, 
		EarlyBoundControlCollection{&this->OkBtn, &this->ComboBox, &this->ListBox}}
	{
		this->OkBtn.Clicked += {*this, &DevTesting::Button_Clicked};
	}

private:
	void
	Button_Clicked(Window& sender)
	{
		this->endModal(sender.ident());
	}
};

// Entry point
int main() 
try 
{
	clog.attach(std::cout);
	startupBanner();
	
	// Display modal dialog
	DevTesting mainDlg;
	std::ignore = mainDlg.showModal();
	return 0;
}
catch (std::exception const& e)
{
	clog << e;
	return -1;
}
