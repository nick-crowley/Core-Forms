#include "library/core.forms.h"
#include "dialogs/Dialog.h"
#include <windows.h>
#include "Resource.h"
using namespace core;

//! @brief	
metadata std::string_view meta::Settings<program_name> = "Basic Core.Forms Application";
metadata std::string_view meta::Settings<program_version> = "Demo";

// Define primitive form capable of running "IDD_BASICFORM"
class BasicForm : public forms::Dialog
{
	using base = forms::Dialog;
	
private:
	forms::ButtonControl OkBtn = IDOK;
	forms::ButtonControl CancelBtn = IDCANCEL;

public:
	BasicForm() : base{forms::ResourceId{IDD_BASICFORM}, EarlyBoundControlCollection{&this->OkBtn, &this->CancelBtn}}
	{
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

// Display modal dialog
int main() 
try 
{
	clog.attach(std::cout);
	startupBanner();
	
	BasicForm mainDlg;
	return mainDlg.showModal(nullptr);
}
catch (std::exception const& e)
{
	clog << e;
	return -1;
}
