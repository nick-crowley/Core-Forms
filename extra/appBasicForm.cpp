#include "library/core.forms.h"
#include "dialogs/Dialog.h"
#include "Resource.h"
using namespace core;

// Define program meta-data
metadata std::string_view meta::Settings<program_name> = "Basic Core.Forms Application";
metadata std::string_view meta::Settings<program_version> = "Demo";

// Define primitive form capable of running "IDD_BASICFORM"
class BasicForm : public forms::Dialog
{
	using base = forms::Dialog;
	
private:
	// Define which controls we want to be managed by Core-Forms. These will be subclassed at
	//  display-time whereas controls not listed here will have standard Window classes and their 
	//  default system-provided appearance and behaviour.
	forms::ButtonControl OkBtn = IDOK;
	forms::ButtonControl CancelBtn = IDCANCEL;

public:
	BasicForm() : base{win::ResourceId{IDD_BASICFORM}, 
		// Providing controls to a base-class via its constructor involves passing objects whose 
		//  memory has not been initialized. Only their address is valid and any attempt to use
		//  them results in undefined behaviour so we use an @c EarlyBoundControlCollection to save
		//  their addresses until the dialog is displayed, at which point, they are "bound" to
		//  controls in the dialog resource.
		EarlyBoundControlCollection{&this->OkBtn, &this->CancelBtn}}
	{
		// Event listeners are attached using the same syntax as C#. Global functions and lambda
		//  functions are supported as well as class methods.
		this->OkBtn.Clicked += {*this, &BasicForm::Button_Clicked};
		this->CancelBtn.Clicked += {*this, &BasicForm::Button_Clicked};
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
	// @c core::LogStream supports writing to any std::ostream, eg. file on disc. Alternatively we
	//  can also use std::cout and view the output in the console.
	clog.attach(std::cout);

	// Boilerplate logging entries containing standard system information are provided for diagnostic 
	//  purposes. If program name and version meta-data is provided, this is also emitted here.
	startupBanner();
	
	// Display modal dialog
	BasicForm mainDlg;
	std::ignore = mainDlg.showModal();
	return 0;
}
catch (std::exception const& e)
{
	// @c core::LogStream natively supports @c std::exception derived types
	clog << e;
	return -1;
}
