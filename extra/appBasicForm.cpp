#include "library/core.forms.h"
#include "com/Implements.h"
#include "dialogs/Dialog.h"
#include "controls/CommonControls.h"
#include <windows.h>
#include "Resource.h"
using namespace core;

// Provide storage for reference counts for COM objects created directly or indirectly by this app
std::atomic_long
com::numInstances = 0;

// Define primitive form capable of running "IDD_BASICFORM"
class BasicForm : public forms::Dialog
{
	using base = forms::Dialog;
	
private:
	forms::ButtonControl OkBtn = IDOK;
	forms::ButtonControl CancelBtn = IDCANCEL;

public:
	BasicForm() : base{forms::ResourceId{IDD_BASICFORM}}
	{
		this->OkBtn.Clicked += {*this, &BasicForm::Button_Clicked};
		this->CancelBtn.Clicked += {*this, &BasicForm::Button_Clicked};
		
		this->BoundControls += forms::ControlDictionary{
			&this->OkBtn, 
			&this->CancelBtn
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
	forms::ControlRegistration registerControls;
	
	BasicForm mainDlg;
	return mainDlg.showModal(nullptr);
}
catch (std::exception const& e)
{
	clog << e;
	return -1;
}
