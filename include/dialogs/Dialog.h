/* o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o */ /*!
* @copyright	Copyright (c) 2023, Nick Crowley. All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted
* provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions
*    and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of
*    conditions and the following disclaimer in the documentation and/or other materials provided
*    with the distribution.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
* FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
* WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
* The views and conclusions contained in the software and documentation are those of the author 
* and should not be interpreted as representing official policies, either expressed or implied, of
* the projects which contain it.
*/
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Preprocessor Directives o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#pragma once
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Header Files o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#include "library/core.Forms.h"
#include "controls/ControlDictionary.h"
#include "controls/CommonControls.h"
#include "dialogs/DialogEventArgs.h"
#include "dialogs/DialogTemplateReader.h"
#include "dialogs/DialogTemplateWriter.h"
#include "forms/ClassId.h"
#include "forms/Window.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class FormsExport Dialog : public Window 
	{
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	protected:
		//! @brief	Temporary storage for controls of _derived_ classes 
		//! 
		//! @remarks These are passed to this class prior to their constructors being executed so
		//!  they cannot be used immediately (but they can be stored)
		class EarlyBoundControlCollection
		{
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			using ControlAddressCollection = std::vector<Control*>;
			
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			ControlAddressCollection  Controls;
			
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			template <nstd::InputRangeConvertibleTo<Control*> AnyCollection>
			explicit
			EarlyBoundControlCollection(AnyCollection&& controls)
				: Controls(std::from_range, std::forward<AnyCollection>(controls))
			{}
			
			explicit
			EarlyBoundControlCollection(std::initializer_list<Control*> controls)
				: Controls(controls)
			{}
			
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			satisfies(EarlyBoundControlCollection,
				IsSemiRegular
			);
			
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			ControlAddressCollection::const_iterator
			begin() const noexcept { 
				return this->Controls.begin(); 
			}

			ControlAddressCollection::const_iterator
			end() const noexcept { 
				return this->Controls.end(); 
			}

			ControlAddressCollection::size_type
			size() const noexcept { 
				return this->Controls.size();  
			}

			ControlDictionary
			to_dictionary() const {
				return ControlDictionary{this->Controls};
			}

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		};

	public:
		class FormsExport WindowClass : public forms::WindowClass {
		public:
			::WNDPROC	OriginalWndProc;
		public:
			WindowClass() : forms::WindowClass(ResourceId::parse(WC_DIALOG))
			{
				this->Name = ResourceId{L"Custom.DIALOG"};
				this->OriginalWndProc = std::exchange(this->WndProc, Dialog::InterceptMessageHandler);
				this->Style |= ClassStyle::GlobalClass;
				this->regist�r();
			}
		};

	protected:
		enum DialogMode { Modal, NonModal };

	private:
		// BUG: Dialog::DialogCreationParameter cannot be __declspec(dllexport)
		std::optional<CreateWindowParameter>
		static /*thread_local*/ DialogCreationParameter;
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		// NB: Fields ordered for debugging convenience
		ResourceId const                DialogId;
		std::optional<DialogMode const>	DisplayMode;
		::DLGPROC const					DialogProc = Dialog::DefaultDialogHandler;
		DialogTemplate const            Template;
		EarlyBoundControlCollection const EarlyBoundControls;
		ControlDictionary               BoundControls;

	public:
		InitDialogEvent		Initialized;
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	protected:
		Dialog(ResourceId resource, EarlyBoundControlCollection controls = {})
		  : Dialog{resource, forms::ProcessModule, controls}
		{
		}

		Dialog(ResourceId resource, Module source, EarlyBoundControlCollection controls = {})
		  : DialogId{resource},
		    Template{DialogTemplateReader{source.loadResource(resource, RT_DIALOG)}.readTemplate()},
			EarlyBoundControls{controls}
		{
			ControlRegistration::ensureRegistered();
		}

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		Response 
		static onSetFont(SetFontEventArgs args) {
			return TRUE;
		}

	protected:
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
				gsl::czstring const name = Window::MessageDatabase.name(message);
				Response response;
				Dialog* dlg {};
			
				if (Window::ExistingWindows.contains(hDlg)) 
				{
					dlg = static_cast<Dialog*>(Window::ExistingWindows[hDlg]);

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

				Invariant(response.Status != Response::Invalid);
			
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
			if (message == WM_NCCREATE && Dialog::DialogCreationParameter) {
				CreateWindowEventArgs args(wParam,lParam);	

				::CREATESTRUCT replacement = *args.Data;
				replacement.lpCreateParams = &Dialog::DialogCreationParameter.value();	// BUG: Returning address which is invalidated on next line
				Dialog::DialogCreationParameter = std::nullopt;

				return Window::DefaultMessageHandler(hWnd, message, wParam, (LPARAM)&replacement);
			} 

			return Window::DefaultMessageHandler(hWnd, message, wParam, lParam);
		}
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		Rect
		mapRect(Rect rc) const {
			::MapDialogRect(this->handle(), rc);
			return rc;
		}
	
		WindowRole
		role() const override {
			return WindowRole::Dialog;
		}

		WindowClass const& 
		wndcls() const override {
			static WindowClass c;
			return c;
		}

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		void 
		virtual createEmbedded(Module source, Window& parent, Border border)
		{
			auto const Area = parent.clientRect() - border;
			this->createInternal(source, DialogMode::NonModal, &parent);
			this->move(Area.topLeft());
			this->resize(Area.size());
		}

		void 
		virtual createEmbedded(Window& parent, Border border)
		{
			this->createEmbedded(forms::ProcessModule, parent, border);
		}

		void 
		virtual createModeless(Module source, Window* parent)
		{
			this->createInternal(source, DialogMode::NonModal, parent);
		}
		
		void 
		virtual createModeless(Window* parent)
		{
			this->createModeless(forms::ProcessModule, parent);
		}
		
		Response 
		onResize(ResizeWindowEventArgs args) override 
		{
			auto const origDimensions = this->mapRect(this->Template.Area).size();
			auto const newDimensions = args.Dimensions;

			for (auto const& kvp : this->BoundControls)
			{
				uint16_t id = kvp.first;
				Control* ctrl = kvp.second;
				
				// Skip controls whose sides aren't anchored 
				// Skip controls which aren't present in the dialog template
				if (auto const anchors = ctrl->anchors(); anchors == Side::None)
					continue;
				else if (auto const original = ranges::find_if(this->Template.Controls, lambda(=, const& c, c.Ident == id)); 
					     original == this->Template.Controls.end())
					continue;
				else {
					Rect const origPosition = this->mapRect(original->Area);
					Rect newPosition = ctrl->wndRect(*this);
					
					// [RIGHT] Maintain original distance to dialog's right-edge  (ie. horizontal stretch)
					if (anchors.test(Side::Right))
					{
						newPosition.Right = newDimensions.Width - (origDimensions.Width - origPosition.Right);
						// [�LEFT] Move right to maintain original size
						if (!anchors.test(Side::Left))
							newPosition.Left = newPosition.Right - origPosition.width();
					}
					// [BOTTOM] Maintain original distance to dialog's bottom-edge  (ie. vertical stretch)
					if (anchors.test(Side::Bottom))
					{
						newPosition.Bottom = newDimensions.Height - (origDimensions.Height - origPosition.Bottom);
						// [�TOP] Move down to maintain original size
						if (!anchors.test(Side::Top))
							newPosition.Top = newPosition.Bottom - origPosition.height();
					}

					ctrl->reposition(newPosition);
				}
			}

			return 0;
		}
	
		intptr_t 
		virtual showModal(Module source, Window* parent)
		{
			return *this->createInternal(source, DialogMode::Modal, parent);
		}
	
		intptr_t 
		virtual showModal(Window* parent)
		{
			return this->showModal(forms::ProcessModule, parent);
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
			this->LookNFeel->initialize(*this,args);
			return FALSE;
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
				if (this->DisplayMode == DialogMode::Modal) {
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
			return ::CallWindowProc(this->wndcls().OriginalWndProc, hDlg, message, wParam, lParam);
		}

		/*::LRESULT 
		unhandledMessage(::HWND hDlg, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
			return Dialog::FallbackDialogHandler(hDlg, message, wParam, lParam);
		}*/
	private:
		std::optional<intptr_t>
		createInternal(Module source, DialogMode mode, Window* parent)
		{
			// Change the wndclass for the dialog
			auto customTemplate = this->Template;
			customTemplate.ClassName = this->wndcls().Name;

			// BUG: Prevent callers from wrapping more than one window handle using the same C++ object
			
			// Late-bind any controls provided at class construction-time
			this->BoundControls += this->EarlyBoundControls.to_dictionary();

			// Change the wndclass for each wrapped control
			if (!this->BoundControls.empty()) {
				for (auto& ctrl : customTemplate.Controls) {
					if (ctrl.ClassName && this->BoundControls.contains(ctrl.Ident)) {
						if (ctrl.ClassName->is_numeric())
							switch (uint16_t id = ctrl.ClassName->as_number(); id) {
							case ClassId::Button:    ctrl.ClassName = ResourceId(L"Custom.BUTTON");    break;
							case ClassId::Edit:      ctrl.ClassName = ResourceId(L"Custom.EDIT");      break;
							case ClassId::Static:    ctrl.ClassName = ResourceId(L"Custom.STATIC");    break;
							case ClassId::Listbox:   ctrl.ClassName = ResourceId(L"Custom.LISTBOX");   break;
							case ClassId::Scrollbar: ctrl.ClassName = ResourceId(L"Custom.SCROLLBAR"); break;
							case ClassId::Combobox:  ctrl.ClassName = ResourceId(L"Custom.COMBOBOX");  break;
							default: throw invalid_argument{"Controls with class id #{0} not yet supported", id};
							}
						else if (ctrl.ClassName == ResourceId{WC_LINK})
							ctrl.ClassName = ResourceId(L"Custom.LINK");

						CreateWindowParameter param(this->BoundControls[ctrl.Ident]);
						ctrl.Data = param.asBytes();
					}
				}
			}

			// Offer derived classes opportunity to modify the template
			this->onLoadDialog(customTemplate);

			// Pre-creation state
			Dialog::DialogCreationParameter = CreateWindowParameter(this);
			this->DisplayMode.emplace(mode);
			auto const on_exit = this->Debug.setTemporaryState(ProcessingState::BeingCreated);
		
			// Generate template with our customizations
			DialogTemplateBlob blob = DialogTemplateWriter{}.writeTemplate(customTemplate);
			auto const container = source.handle();
			auto const owner = parent ? parent->handle() : nullptr;
			
			// [MODAL] Display, block, and return result
			if (mode == DialogMode::Modal) { 
				auto result = ::DialogBoxIndirectW(container, blob, owner, this->DialogProc);
				if (result == -1)
					win::LastError{}.throwIfError("Failed to display '{}' dialog", to_string(this->DialogId));

				this->DisplayMode = std::nullopt;
				return result;
			}

			// [MODELESS] Display and return handle
			this->Handle = ::CreateDialogIndirectW(container, blob, owner, this->DialogProc);
			return std::nullopt;
		}
	};
}	// namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o