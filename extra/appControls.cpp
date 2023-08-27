#include "library/core.forms.h"
#include "dialogs/Dialog.h"
#include <windows.h>
#include "Resource.h"
using namespace core;

// Define program meta-data
metadata std::string_view meta::Settings<program_name> = "Core-Forms Controls Demo";
metadata std::string_view meta::Settings<program_version> = "1.0";

// Define form to run "IDD_CONTROLS"
class BasicForm : public forms::Dialog
{
	using base = forms::Dialog;
	
private:
	forms::ButtonControl OkBtn = IDOK;

public:
	BasicForm() : base{forms::ResourceId{IDD_CONTROLS}}
	{
		this->OkBtn.Clicked += {*this, &BasicForm::Button_Clicked};
		
		this->BoundControls += forms::ControlDictionary{
			&this->OkBtn
		};
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
