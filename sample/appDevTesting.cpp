#include "library/core.forms.h"
#include "dialogs/Dialog.h"
#include "graphics/ImageList.h"
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

protected:
	Response 
	virtual onInitDialog(forms::InitDialogEventArgs args) override {
		std::ignore = base::onInitDialog(args);

		for (int idx = 1; idx <= 8; ++idx) {
			this->ListBox.Items.push_back(std::format(L"Item #{}", idx));
		}

#ifdef ItemsWithHeadings
		this->ComboBox.Items.push_back(L"The rain in spain falls mainly on the plane", L"Rain in Spain");
		this->ComboBox.Items.push_back(L"The quick brown fox jumped over the lazy dog", L"Quick Brown Fox");
		this->ComboBox.Items.push_back(L"My very earthly mother just said you're nearly perfect", L"Very Earthly Mother");*/
#endif
		this->ComboBox.Items.push_back(forms::ComboBoxItemElement{L"Rain in Spain", forms::Colour::DarkBlue, forms::StockFont::SystemFixed});
		this->ComboBox.Items.push_back(L"Quick Brown Fox", L"Example heading");
		this->ComboBox.Items.push_back(forms::ComboBoxItemElement{L"Very Earthly Mother", forms::Colour::Red, forms::StockFont::AnsiFixed});

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
	DevTesting mainDlg;
	std::ignore = mainDlg.showModal();
	return 0;
}
catch (std::exception const& e)
{
	clog << e;
	return -1;
}
