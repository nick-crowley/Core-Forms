#include "library/core.forms.h"
#include "dialogs/Dialog.h"
#include "lookNfeel/Nt6LookNFeel.h"
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
	forms::GroupBoxControl ListGroup = IDC_GROUP1;
	forms::ListBoxControl StringList = IDC_LIST1;
	forms::GroupBoxControl PopulateGroup = IDC_GROUP2;
	forms::CheckBoxControl ReverseCheck = IDC_CHECK1;
	forms::ButtonControl PopulateBtn = IDC_BUTTON1;
	
	forms::GroupBoxControl EditGroup = IDC_GROUP3;
	forms::EditControl SingleLineEdit = IDC_EDIT1;
	forms::EditControl MultiLineEdit = IDC_EDIT2;

	forms::ButtonControl OkBtn = IDOK;

public:
	BasicForm() : base{forms::ResourceId{IDD_CONTROLS}, {
		&this->ListGroup,
		&this->StringList,
		&this->PopulateGroup,
		&this->ReverseCheck,
		&this->PopulateBtn,
		&this->EditGroup,
		&this->SingleLineEdit,
		&this->MultiLineEdit,
		&this->OkBtn
	}}
	{
		this->OkBtn.Clicked += {*this, &BasicForm::Button_Clicked};
		this->LookNFeel = forms::Nt6LookNFeel::Instance;
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
