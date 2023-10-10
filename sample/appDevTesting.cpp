#include "library/core.forms.h"
#include "dialogs/Dialog.h"
#include "graphics/FontBuilder.h"
#include "graphics/ImageList.h"
#include "graphics/Icon.h"
#include "lookNFeel/DarkRetroLookNFeel.h"
#include "Resource.h"
using namespace core;
using namespace forms::literals;

// Define program meta-data
metadata std::string_view meta::Settings<program_name> = "Core.Forms Dev-Testing Application";
metadata std::string_view meta::Settings<program_version> = "Demo";

// Define primitive form capable of running "IDD_DEVTEST"
class DevTesting : public forms::Dialog
{
	using base = forms::Dialog;
	
private:
	forms::ButtonControl OkBtn = IDOK;
	forms::ComboBoxControl WideComboBox = IDC_COMBO1;
	forms::ComboBoxControl NarrowComboBox = IDC_COMBO2;
	forms::ListBoxControl ListBox = IDC_LIST1;
	forms::Font  DetailFont = forms::FontBuilder{}.withName(L"Consolas")
	                                              .withSize(11_pt)
	                                              .build();
	forms::Font  TitleFont = forms::FontBuilder{}.withName(L"Consolas")
	                                             .withSize(14_pt)
	                                             .withWeight(forms::FontWeight::Bold)
	                                             .build();

public:
	DevTesting() 
	  : base{
			win::ResourceId{IDD_DEVTEST}, 
			EarlyBoundControlCollection{&this->OkBtn, &this->NarrowComboBox, &this->WideComboBox, &this->ListBox}
		}
	{
		this->OkBtn.Clicked += {*this, &DevTesting::Button_Clicked};
		
		// A design flaw means custom fonts for currently selected ComboBox item must be
		//  specified prior to creation of the ComboBox and, once set, cannot be changed.
		this->WideComboBox.editFont(this->TitleFont);
	}

protected:
	forms::Response 
	virtual onInitDialog(forms::InitDialogEventArgs args) override {
		std::ignore = base::onInitDialog(args);

		for (int idx = 1; idx <= 8; ++idx) {
			this->ListBox.Items.push_back(std::format(L"Item #{}", idx));
		}

		forms::Icon const sampleIcon = forms::Icon::load(win::ProcessModule, win::ResourceId{IDI_ICON1});

		// Separate design flaw means custom font for currently selected ComboBox item must be
		//  specified again after creation of the ComboBox. This can be changed at runtime but
		//  the control will never resize to fit the new font.
		this->WideComboBox.font(this->TitleFont);
		this->WideComboBox.titleFont(this->TitleFont);

		struct { gsl::cwzstring title; gsl::cwzstring detail; } const itemDefinitions[3] {
			{L"Rain in Spain",       L"The rain in spain falls mainly on the plane"},
			{L"Quick Brown Fox",     L"The quick brown fox jumped over the lazy dog"},
			{L"Very Earthly Mother", L"My very earthly mother just said you're nearly perfect"}
		};
		for (auto item : itemDefinitions) {
			this->WideComboBox.Items.push_back(
				forms::ComboBoxElement{item.detail, forms::Colour::Grey, this->DetailFont},
				forms::ComboBoxElement{item.title},
				sampleIcon
			);
		}

		this->NarrowComboBox.Items.push_back(forms::ComboBoxElement{L"Rain in Spain", forms::Colour::DarkBlue, forms::StockFont::SystemFixed}, sampleIcon);
		this->NarrowComboBox.Items.push_back(L"Quick Brown Fox");
		this->NarrowComboBox.Items.push_back(forms::ComboBoxElement{L"Very Earthly Mother", forms::Colour::Red, forms::StockFont::AnsiFixed}, sampleIcon);

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
