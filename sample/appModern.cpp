#include "library/core.forms.h"
#include "dialogs/Dialog.h"
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
	forms::ButtonControl OkBtn = IDOK;
	forms::ButtonControl CancelBtn = IDCANCEL;
	
public:
	ModernUi() 
	  : base{
	      win::ResourceId{IDD_MODERN}, 
	      EarlyBoundControlCollection{&this->OkBtn, &this->CancelBtn}
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
		std::ignore = base::onInitDialog(args);
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
