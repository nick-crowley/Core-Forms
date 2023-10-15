#include "library/core.forms.h"
#include "dialogs/Dialog.h"
#include "graphics/FontBuilder.h"
#include "graphics/ImageList.h"
#include "graphics/Icon.h"
#include "lookNFeel/DarkRetroLookNFeel.h"
#include "lookNFeel/DarkModernLookNFeel.h"
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
	                                              .withSize(11_pt);
	forms::Font  HeadingFont = forms::FontBuilder{}.withName(L"Consolas")
	                                               .withSize(12_pt)
	                                               .withWeight(forms::FontWeight::Bold);
	forms::Icon  SampleIcon = forms::Icon::load(win::ProcessModule, win::ResourceId{IDI_ICON1});

public:
	DevTesting() 
	  : base{
			win::ResourceId{IDD_DEVTEST}, 
			EarlyBoundControlCollection{&this->OkBtn, &this->NarrowComboBox, &this->WideComboBox, &this->ListBox}
		}
	{
		// Attach listeners
		this->OkBtn.Clicked += {*this, &DevTesting::Button_Clicked};
		
		// A design flaw means custom fonts for ComboBox itself (the currently selected item) must be
		//  specified prior to creation and, once set, cannot be changed.
		this->WideComboBox.editFont(this->HeadingFont);
	}

protected:
	forms::Response 
	virtual onInitDialog(forms::InitDialogEventArgs args) override {
		// Populate list
		this->ListBox.features(forms::ListBoxFeature::Icons);
		for (int idx = 1; idx <= 8; ++idx) {
			auto const ws = std::format(L"This is an item #{}", idx);
			this->ListBox.Items.push_back(forms::RichText{ws, forms::Colour::DarkBlue}, this->SampleIcon);
		}
		
		// Separate design flaw means custom font for currently selected ComboBox item must be
		//  specified again after creation of the ComboBox. This can be changed at runtime but
		//  the control will never resize to fit the new font.
		this->WideComboBox.font(this->HeadingFont);

		// Set custom styles
		using enum forms::ComboBoxFeature;
		this->WideComboBox.features(Headings|Icons);
		this->WideComboBox.headingFont(this->HeadingFont);

		// Populate with example items
		struct ExampleData { gsl::cwzstring title; gsl::cwzstring detail; };
		for (auto const [title,detail] : std::initializer_list<ExampleData>{
			{L"Rain in Spain",       L"The rain in spain falls mainly on the plane"},
			{L"Quick Brown Fox",     L"The quick brown fox jumped over the lazy dog"},
			{L"Very Earthly Mother", L"My very earthly mother just said you're nearly perfect"}
		}) {
			this->WideComboBox.Items.push_back(
				forms::RichText{detail, forms::Colour::Grey, this->DetailFont},
				forms::RichText{title},
				this->SampleIcon
			);
		}
		
		// Set custom styles
		this->NarrowComboBox.features(Icons);

		// Populate with example items
		this->NarrowComboBox.Items.push_back(forms::RichText{L"Rain in Spain", forms::Colour::DarkBlue, forms::StockFont::SystemFixed}, this->SampleIcon);
		this->NarrowComboBox.Items.push_back(L"The quick brown fox jumped over the lazy dog");
		this->NarrowComboBox.Items.push_back(forms::RichText{L"Very Earthly Mother", forms::Colour::Red, forms::StockFont::AnsiFixed}, this->SampleIcon);
		this->NarrowComboBox.Items.push_back(forms::RichText{L"The quick brown fox jumped over the lazy dog", forms::Colour::Forest, forms::StockFont::SystemFixed}, this->SampleIcon);

		// Select first item of each comboBox
		this->NarrowComboBox.Items.select(this->NarrowComboBox.Items.cbegin());
		this->WideComboBox.Items.select(this->WideComboBox.Items.cbegin());
		
		// Tell system to set the initial focus
		return args.SetInitialFocus;
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
	forms::lookNFeel = forms::DarkRetroLookNFeel::Instance;
	
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
