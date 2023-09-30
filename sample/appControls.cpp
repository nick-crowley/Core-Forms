#include "library/core.forms.h"
#include "dialogs/Dialog.h"
#include "Resource.h"
using namespace core;

// Define program meta-data
metadata std::string_view meta::Settings<program_name> = "Core-Forms Controls Demo";
metadata std::string_view meta::Settings<program_version> = "1.0";

// Define form to run "IDD_CONTROLS"
class ControlsForm : public forms::Dialog
{
	using base = forms::Dialog;
	
private:
	// Control identifiers have to be provided at construction to improve the developer experience
	//  by reducing the need to lookup IDs in the resource header or Visual Studio dialog editor
	forms::GroupBoxControl ListControlsGroup = IDC_GROUP1;
	forms::ListBoxControl ItemList = IDC_LIST1;
	forms::GroupBoxControl ConfigurationGroup = IDC_GROUP2;
	forms::CheckBoxControl ReverseItemsCheck = IDC_CHECK1;
	forms::ButtonControl PopulateBtn = IDC_BUTTON1;
	
	forms::GroupBoxControl EditControlsGroup = IDC_GROUP3;
	forms::EditControl SingleLineEdit = IDC_EDIT1;
	forms::EditControl MultiLineEdit = IDC_EDIT2;

	forms::ButtonControl OkBtn = IDOK;

public:
	ControlsForm() : base{win::ResourceId{IDD_CONTROLS}, 
		EarlyBoundControlCollection{
			&this->ListControlsGroup,
			&this->ItemList,
			&this->ConfigurationGroup,
			&this->ReverseItemsCheck,
			&this->PopulateBtn,
			&this->EditControlsGroup,
			&this->SingleLineEdit,
			&this->MultiLineEdit,
			&this->OkBtn
		}}
	{
		// Attach listeners for events
		this->OkBtn.Clicked += {*this, &ControlsForm::OkBtn_Clicked};
		this->PopulateBtn.Clicked += {*this, &ControlsForm::PopulateBtn_Clicked};
		this->SingleLineEdit.TextChanged += {*this, &ControlsForm::SingleLineEdit_TextChanged};

		// Define how controls should behave when the dialog is resized
		// * Anchoring on the left/top alone has no effect
		// * Anchoring on the right/bottom causes controls to move
		// * Anchoring two opposite sides causes controls to stretch
		using enum forms::Side;
		this->ListControlsGroup.anchors(Left|Top|Right);
		this->ItemList.anchors(Left|Top|Right);

		this->ConfigurationGroup.anchors(Top|Right);
		this->ReverseItemsCheck.anchors(Top|Right);
		this->PopulateBtn.anchors(Top|Right);

		this->EditControlsGroup.anchors(Left|Top|Right|Bottom);
		this->SingleLineEdit.anchors(Left|Top|Right);
		this->MultiLineEdit.anchors(Left|Top|Right|Bottom);

		this->OkBtn.anchors(Right|Bottom);
	}

protected:
	Response
	virtual onInitDialog(forms::InitDialogEventArgs args) override
	{
		this->PopulateBtn.focus();
		return FALSE;
	}

	void
	virtual onLoadDialog(forms::LoadDialogEventArgs args) override
	{
		// Enable owner-draw on all controls (where supported) to enable look-n-feel support
		for(auto& ctrl : args.Template.Controls) 
			ctrl.setOwnerDraw();
	}
	
private:
	void
	OkBtn_Clicked(Window& sender)
	{
		this->endModal(sender.ident());
	}
	
	void
	PopulateBtn_Clicked(Window& sender)
	{
		std::vector indexes {1,2,3,4,5,6,7,8};

		// CheckBoxes expose their state via @c CheckBoxControl::checked()
		if (this->ReverseItemsCheck.checked())
			std::reverse(indexes.begin(), indexes.end());
		
		// ListBoxes expose their items via their @c Items and @SelectedIndicies properties, which
		//  are both C++ container facades over the standard Windows messages.
		this->ItemList.Items.clear();
		for (unsigned idx : indexes)
			this->ItemList.Items.push_back(std::format(L"Item #{}", idx));
	}
	
	void
	SingleLineEdit_TextChanged(Window& sender)
	{
		// Window text can be retrieved or modified using @c Window::text() overloads
		this->MultiLineEdit.text(this->SingleLineEdit.text());
	}
};

// Entry point: Display modal dialog and exit
int main() 
try 
{
	clog.attach(std::cout);
	startupBanner();
	
	ControlsForm mainDlg;
	std::ignore = mainDlg.showModal();
	return 0;
}
catch (std::exception const& e)
{
	clog << e;
	return -1;
}
