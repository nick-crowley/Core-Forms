#include "library/core.forms.h"
#include "dialogs/Dialog.h"
#include "lookNFeel/RetroLookNFeel.h"
#include "Resource.h"
using namespace core;
using namespace forms::literals;

// Define program meta-data
metadata std::string_view meta::Settings<program_name> = "Core.Forms ModernUi UI Application";
metadata std::string_view meta::Settings<program_version> = "Demo";

// Define primitive form capable of running "IDD_MODERN"
class ModernUi : public forms::Dialog
{
	using base = forms::Dialog;
	
private:
	forms::LabelControl Heading = IDC_HEADING1;
	forms::LabelControl SubHeading = IDC_HEADING2;
	forms::CheckBoxControl Option = IDC_CHECK1;
	forms::ButtonControl OkBtn = IDOK;
	forms::ButtonControl CancelBtn = IDCANCEL;
	
public:
	ModernUi() 
	  : base{
	      win::ResourceId{IDD_MODERN}, 
	      EarlyBoundControlCollection{&this->OkBtn, &this->CancelBtn, &this->Heading, 
		                              &this->SubHeading, &this->Option}
	    }
	{
		this->OkBtn.Clicked += {*this, &ModernUi::Button_Clicked};
		this->CancelBtn.Clicked += {*this, &ModernUi::Button_Clicked};

		using enum forms::Side;
		this->OkBtn.anchors(Right|Bottom);
		this->CancelBtn.anchors(Right|Bottom);
	}

protected:
	Response 
	virtual onInitDialog(forms::InitDialogEventArgs args) override {
		this->Heading.font(this->LookNFeel->heading1());
		this->SubHeading.font(this->LookNFeel->heading2());
		return FALSE;
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

	// Set modern look-n-feel
	forms::lookNFeel = forms::RetroLookNFeel::Instance;
	
	// Display modal dialog
	ModernUi mainDlg;
	std::ignore = mainDlg.showModal();
	return 0;
}
catch (std::exception const& e)
{
	clog << e;
	return -1;
}
