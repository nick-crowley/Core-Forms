#pragma once
#include "library/core.Forms.h"
#include "windows/ClassId.h"
#include "windows/Window.h"
#include "dialogs/DialogEventArgs.h"
#include "system/Resources.h"

namespace core::forms
{
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
		ResourceId const                DialogId;
		std::optional<DialogMode const>	DisplayMode;
		::DLGPROC const					DialogProc;
		
	public:
		InitDialogEvent		Initialized;

	protected:
		Dialog(ResourceId resource, ::DLGPROC handler = Dialog::DefaultDialogHandler) 
		  : DialogId{resource},
		    DialogProc{handler}
		{
		}
	
	public:
		Response 
		static onSetFont(SetFontEventArgs args) {
			return TRUE;
		}
	
	public:
		template <typename Self>
		Module
		instance(this Self&& self) {
			return Resources<std::remove_cvref_t<Self>>;
		}

		DialogWindowClass const& 
		wndcls() override {
			static DialogWindowClass c;
			return c;
		}

	public:
		void 
		virtual showEmbedded(Window& parent, std::optional<Border> border, std::optional<ControlDictionary> wrappers = std::nullopt)
		{
			auto const Area = border ? parent.clientRect() - *border : parent.clientRect();
			this->showDialog(DialogMode::NonModal, &parent, wrappers);
			this->move(Area.topLeft());
			this->resize(Area.size());
		}

		intptr_t 
		virtual showModal(Window* parent, std::optional<ControlDictionary> wrappers = std::nullopt)
		{
			return *this->showDialog(DialogMode::Modal, parent, wrappers);
		}
	
		void 
		virtual showModeless(Window* parent, std::optional<ControlDictionary> wrappers = std::nullopt)
		{
			this->showDialog(DialogMode::NonModal, parent, wrappers);
		}

	protected:
		void 
		endModal(uint16_t const id)
		{
			::EndDialog(this->handle(), id);
		}

		Response 
		virtual onDestroy() override {
			return Unhandled;
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
		virtual offerMessage(::HWND hDlg, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override
		{
			switch (message) {
			case WM_INITDIALOG: 
				return this->onInitDialog({wParam,lParam});
			}

			return Window::offerMessage(hDlg, message, wParam, lParam);
		} 
	
		Response
		virtual offerNotification(::UINT notification) override {
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
		virtual raiseMessageEvent(::HWND hDlg, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override
		{
			switch (message) {
			case WM_INITDIALOG: 
				this->Initialized.raise(InitDialogEventArgs{wParam,lParam});
				return;
			}

			Window::raiseMessageEvent(hDlg, message, wParam, lParam);
		} 

		::LRESULT 
		virtual unhandledMessage(::HWND hDlg, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
			return ::CallWindowProc(this->wndcls().OriginalMessageHandler, hDlg, message, wParam, lParam);
		}

		/*::LRESULT 
		unhandledMessage(::HWND hDlg, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
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
				gsl::czstring const name = s_MessageDatabase.name(message);
				Response response;
				Dialog* dlg {};
			
				if (s_ExistingWindows.contains(hDlg)) 
				{
					dlg = static_cast<Dialog*>(s_ExistingWindows[hDlg]);

					{
						auto const on_exit = dlg->Debug.setTemporaryState({ProcessingState::DialogProcessing,name});
						response = dlg->offerMessage(hDlg, message, wParam, lParam);
					}

					{
						auto const on_exit = dlg->Debug.setTemporaryState({ProcessingState::EventProcessing,name});
						dlg->raiseMessageEvent(hDlg, message, wParam, lParam);
					}
				}
				else if (message == WM_SETFONT) {
					response = Dialog::onSetFont({wParam,lParam});
				}
				else {
					throw runtime_error{"No associated dialog object"};
				}

				assert(response.Status != Response::Invalid);
			
				if (response.Status == Response::Handled) 
				{
					log_entry.setResult(Response::Handled, *response.Value);
					return *response.Value;
				}

				log_entry.setResult(Response::Unhandled, FALSE);
				return FALSE;
			} 
			catch (const std::exception& e) {
				log_entry.setException(e);
				return (INT_PTR)FALSE;
			}
		}

		::LRESULT 
		static CALLBACK InterceptMessageHandler(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam)
		{
			// The documented dialog box Windows API functions do not provide a way of passing a custom
			// parameter into the creation of dialogs; they only provide a facility to pass a parameter
			// into the synthetic WM_INITDIALOG message, which is raised post the creation of child windows.
			// In order to manage the dialog handle throughout its lifetime, we intercept WM_NCCREATE and
			// manually pass a custom parameter stored (temporarily) in a threadlocal variable by showModal()
			if (message == WM_NCCREATE && Dialog::s_DialogCreationParameter) {
				CreateWindowEventArgs args(wParam,lParam);	

				::CREATESTRUCT replacement = *args.Data;
				replacement.lpCreateParams = &Dialog::s_DialogCreationParameter.value();
				Dialog::s_DialogCreationParameter = std::nullopt;

				return Window::DefaultMessageHandler(hWnd, message, wParam, (LPARAM)&replacement);
			} 

			return Window::DefaultMessageHandler(hWnd, message, wParam, lParam);
		}

	private:
		std::optional<intptr_t>
		showDialog(DialogMode mode, Window* parent, std::optional<ControlDictionary> wrappers)
		{
			Module module = this->instance();
			auto customTemplate = DialogTemplateReader{module.loadResource(this->DialogId, RT_DIALOG)}.read_template();

			// Change the wndclass for the dialog
			customTemplate.ClassName = ResourceId::parse(this->wndcls().lpszClassName);

			// BUG: Prevent callers from wrapping more than one window handle using the same C++ object
			
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
						default: throw invalid_argument{"Controls with class id #{0} not yet supported", id};
						}

						CreateWindowParameter param((*wrappers)[ctrl.Ident]);
						ctrl.Data = param.asBytes();
					}
				}
			}

			// Offer derived classes opportunity to modify the template
			this->onLoadDialog(customTemplate);

			// Pre-creation state
			Dialog::s_DialogCreationParameter = CreateWindowParameter(this);
			this->DisplayMode.emplace(mode);
			auto const on_exit = this->Debug.setTemporaryState(ProcessingState::BeingCreated);
		
			// Generate template with our customizations
			DialogTemplateBlob blob = DialogTemplateWriter{}.write_template(customTemplate);
			auto container = module.handle();
			auto owner = parent ? parent->handle() : nullptr;

			// [MODAL] Display, block, and return result
			if (mode == DialogMode::Modal) { 
				auto result = ::DialogBoxIndirectW(container, blob, owner, this->DialogProc);
				this->DisplayMode = std::nullopt;
				return result;
			}

			// [MODELESS] Display and return handle
			this->Handle = ::CreateDialogIndirectW(container, blob, owner, this->DialogProc);
			return std::nullopt;
		}
	};
}	// namespace core::forms
