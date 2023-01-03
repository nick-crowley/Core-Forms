#pragma once
#include "formsFramework.h"
#include "windows/ClassId.h"
#include "windows/Window.h"
#include "dialogs/DialogEventArgs.h"

class Dialog : public Window {
public:
	class DialogWindowClass : public WindowClass {
	public:
		::WNDPROC	OriginalMessageHandler;
	public:
		DialogWindowClass() : WindowClass(ResourceId::parse(WC_DIALOG))
		{
			this->name(ResourceId(L"Custom.DIALOG"));
			this->OriginalMessageHandler = std::exchange(this->lpfnWndProc, Dialog::InterceptMessageHandler);
			this->register$();
		}
	};
	
	using ControlDictionary = std::map<uint16_t, Window*>;
	
protected:
	enum DialogMode { Modal, NonModal };

private:
	std::optional<CreateWindowParameter>
	static thread_local s_DialogCreationParameter;

	// NB: Fields ordered for debugging convenience
private:
	std::optional<DialogMode const>	DisplayMode;
	::DLGPROC const					DialogProc;
	DialogTemplate					Template;
	
public:
	InitDialogEvent		Initialized;

protected:
	Dialog(ResourceId resource, ::DLGPROC handler = Dialog::DefaultDialogHandler, std::optional<Module> module = std::nullopt) 
	  : DialogProc(handler)
	{
		auto bytes = Module::load_resource(resource, RT_DIALOG, module);
		DialogTemplateReader r{ bytes };
		this->Template = r.read_template();
	}
	
public:
	Response 
	static onSetFont(SetFontEventArgs args) {
		return TRUE;
	}
	
public:
	DialogWindowClass const& 
	wndcls() override {
		static DialogWindowClass c;
		return c;
	}

public:
	intptr_t 
	showModal(Window* parent, std::optional<ControlDictionary> wrappers, std::optional<Module> module = std::nullopt) 
	{
		auto customTemplate = this->Template;

		// Change the wndclass for the dialog
		customTemplate.ClassName = ResourceId::parse(this->wndcls().lpszClassName);

		// Change the wndclass for each wrapped control
		if (wrappers) {
			for (auto& ctrl : customTemplate.Controls) {
				if (ctrl.ClassName && ctrl.ClassName->is_numeric() && wrappers->contains(ctrl.Ident)) {
					switch (uint16_t id = ctrl.ClassName->as_number(); id) {
					case ClassId::Button:    ctrl.ClassName = ResourceId(L"Custom.BUTTON");    break;
					case ClassId::Edit:      ctrl.ClassName = ResourceId(L"Custom.EDIT");      break;
					case ClassId::Static:    ctrl.ClassName = ResourceId(L"Custom.STATIC");    break;
					case ClassId::Listbox:   ctrl.ClassName = ResourceId(L"Custom.LISTBOX");   break;
					case ClassId::Scrollbar: ctrl.ClassName = ResourceId(L"Custom.SCROLLBAR"); break;
					case ClassId::Combobox:  ctrl.ClassName = ResourceId(L"Custom.COMBOBOX");  break;
					default: throw std::invalid_argument("Controls with class id #" + std::to_string(id) + " not yet supported");
					}

					CreateWindowParameter param((*wrappers)[ctrl.Ident]);
					ctrl.Data = param.as_bytes();
				}
			}
		}

		// Offer derived classes opportunity to modify the template
		this->onLoadDialog(customTemplate);

		// Pre-creation state
		Dialog::s_DialogCreationParameter = CreateWindowParameter(this);
		this->DisplayMode.emplace(DialogMode::Modal);
		auto const on_exit = this->Debug.setTemporaryState(ProcessingState::BeingCreated);
		
		// Display modal
		DialogTemplateBlob blob = DialogTemplateWriter{}.write_template(customTemplate);
		auto container = module ? module->handle() : nullptr;
		auto owner = parent ? parent->handle() : nullptr;
		auto result = ::DialogBoxIndirectW(container, blob, owner, this->DialogProc);

		// Post-destroy state
		this->DisplayMode = std::nullopt;
		return result;
	}

protected:
	void 
	endModal(uint16_t const id)
	{
		::EndDialog(this->handle(), id);
	}

	Response 
	virtual onDestroy() override {
		if (this->DisplayMode == Modal) {
			return Unhandled;
		}

		return Window::onDestroy();
	}
	
	void
	virtual onLoadDialog(LoadDialogEventArgs args)
	{
	}
	
	Response 
	virtual onInitDialog(InitDialogEventArgs args) {
		return Unhandled;
	}
	
	Window::Response
	virtual onPaint(PaintWindowEventArgs args) override {
		args.beginPaint();
		this->LookNFeel->draw(*this, args);
		args.endPaint();
		return TRUE;
	}

	Response
	virtual offer_message(::HWND hDlg, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override
	{
		switch (message) {
		case WM_INITDIALOG: 
			return this->onInitDialog({wParam,lParam});
		}

		return Window::offer_message(hDlg, message, wParam, lParam);
	} 
	
	Response
	virtual offer_notification(::UINT notification) override {
		switch (notification) {
		case IDOK:
		case IDCANCEL:
			if (this->DisplayMode == Modal) {
				this->endModal(notification);
				return TRUE;
			}
		}

		return Unhandled;
	}
	
	void
	virtual raise_message_event(::HWND hDlg, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override
	{
		switch (message) {
		case WM_INITDIALOG: 
			this->Initialized.raise(InitDialogEventArgs{wParam,lParam});
			return;
		}

		Window::raise_message_event(hDlg, message, wParam, lParam);
	} 

	::LRESULT 
	virtual unhandled_message(::HWND hDlg, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
		return ::CallWindowProc(this->wndcls().OriginalMessageHandler, hDlg, message, wParam, lParam);
	}

	/*::LRESULT 
	unhandled_message(::HWND hDlg, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
		return Dialog::FallbackDialogHandler(hDlg, message, wParam, lParam);
	}*/

	/*::INT_PTR 
	static CALLBACK FallbackDialogHandler(::HWND hDlg, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) {
		return ::DefDlgProcW(hDlg, message, wParam, lParam);
	}*/

	::INT_PTR 
	static CALLBACK DefaultDialogHandler(::HWND hDlg, ::UINT message, ::WPARAM wParam, ::LPARAM lParam)
	{
		WindowProcLoggingSentry log_entry(__FUNCTION__, message);
		// FIXME: This method needs documenting
		try {
			const char* const name = s_MessageDatabase.name(message);
			Response response;
			Dialog* dlg {};
			
			if (s_ExistingWindows.contains(hDlg)) 
			{
				dlg = static_cast<Dialog*>(s_ExistingWindows[hDlg]);

				{
					auto const on_exit = dlg->Debug.setTemporaryState({ProcessingState::DialogProcessing,name});
					response = dlg->offer_message(hDlg, message, wParam, lParam);
				}

				{
					auto const on_exit = dlg->Debug.setTemporaryState({ProcessingState::EventProcessing,name});
					dlg->raise_message_event(hDlg, message, wParam, lParam);
				}
			}
			else if (message == WM_SETFONT) {
				response = Dialog::onSetFont({wParam,lParam});
			}
			else {
				throw std::runtime_error("No associated dialog object");
			}

			assert(response.Status != Response::Invalid);
			
			if (response.Status == Response::Handled) 
			{
				log_entry.set_result(Response::Handled, *response.Value);
				return *response.Value;
			}

			log_entry.set_result(Response::Unhandled, FALSE);
			return FALSE;
		} 
		catch (const std::exception& e) {
			log_entry.set_exception(e);
			return (INT_PTR)FALSE;
		}
	}

	::LRESULT 
	static CALLBACK InterceptMessageHandler(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam)
	{
		// FIXME: This method needs documenting
		if (message == WM_NCCREATE && Dialog::s_DialogCreationParameter) {
			CreateWindowEventArgs args(wParam,lParam);	

			::CREATESTRUCT replacement = *args.Data;
			replacement.lpCreateParams = &Dialog::s_DialogCreationParameter.value();
			Dialog::s_DialogCreationParameter = std::nullopt;

			return Window::DefaultMessageHandler(hWnd, message, wParam, (LPARAM)&replacement);
		} 

		return Window::DefaultMessageHandler(hWnd, message, wParam, lParam);
	}
};

