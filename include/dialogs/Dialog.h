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
	private:
		using base = Window;

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
			WindowClass() : forms::WindowClass(win::ResourceId::parse(WC_DIALOG))
			{
				this->Name = win::ResourceId{L"Custom.DIALOG"};
				this->OriginalWndProc = std::exchange(this->WndProc, Dialog::InterceptMessageHandler);
				this->Style |= ClassStyle::GlobalClass;
				this->registér();
			}
		};

	protected:
		enum DialogMode { Modal, NonModal };

	private:
		// BUG: Dialog::DialogCreationParameter cannot be __declspec(dllexport)
		std::optional<CreationData>
		static /*thread_local*/ DialogCreationParameter;
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		// NB: Fields ordered for debugging convenience
		win::ResourceId const           DialogId;
		std::optional<DialogMode const>	DisplayMode;
		::DLGPROC const					DialogProc = Dialog::DefaultDialogHandler;
		DialogTemplate const            Template;
		EarlyBoundControlCollection const EarlyBoundControls;
		ControlDictionary               BoundControls;

	public:
		InitDialogEvent		Initialized;
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	protected:
		Dialog(win::ResourceId resource, EarlyBoundControlCollection controls = {})
		  : Dialog{resource, win::ProcessModule, controls}
		{
		}

		Dialog(win::ResourceId resource, win::Module source, EarlyBoundControlCollection controls = {})
		  : DialogId{resource},
		    Template{DialogTemplateReader{source.loadResource(resource, RT_DIALOG)}.readTemplate()},
			EarlyBoundControls{controls}
		{
			ControlRegistration::ensureRegistered();
		}

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	protected:
		::INT_PTR 
		static CALLBACK DefaultDialogHandler(::HWND hDlg, ::UINT message, ::WPARAM wParam, ::LPARAM lParam)
		{
			WndProcLoggingSentry log_entry(message);
			// FIXME: This method needs documenting
			try {
				Response response;
				
				if (Dialog* dlg = Window::ExistingWindows.find<Dialog>(hDlg); dlg) 
				{
					response = dlg->offerMessage(message, wParam, lParam);

					dlg->raiseMessageEvent(message, wParam, lParam);
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
		virtual createEmbedded(win::Module source, Window& parent, Border border)
		{
			auto const Area = parent.clientRect() - border;
			this->createInternal(source, DialogMode::NonModal, &parent);
			this->move(Area.topLeft());
			this->resize(Area.size());
		}

		void 
		virtual createEmbedded(Window& parent, Border border)
		{
			this->createEmbedded(win::ProcessModule, parent, border);
		}

		void 
		virtual createModeless(win::Module source, Window* parent = nullptr)
		{
			this->createInternal(source, DialogMode::NonModal, parent);
		}
		
		void 
		virtual createModeless(Window* parent = nullptr)
		{
			this->createModeless(win::ProcessModule, parent);
		}
		
		Response 
		virtual onResize(ResizeWindowEventArgs args) override 
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
						// [¬LEFT] Move right to maintain original size
						if (!anchors.test(Side::Left))
							newPosition.Left = newPosition.Right - origPosition.width();
					}
					// [BOTTOM] Maintain original distance to dialog's bottom-edge  (ie. vertical stretch)
					if (anchors.test(Side::Bottom))
					{
						newPosition.Bottom = newDimensions.Height - (origDimensions.Height - origPosition.Bottom);
						// [¬TOP] Move down to maintain original size
						if (!anchors.test(Side::Top))
							newPosition.Top = newPosition.Bottom - origPosition.height();
					}

					ctrl->reposition(newPosition);
				}
			}

			return 0;
		}
	
		intptr_t 
		virtual showModal(win::Module source, Window* parent = nullptr)
		{
			// FIXME: It'd be preferable if Dialog::showModal() were not virtual so it could be a template
			return *this->createInternal(source, DialogMode::Modal, parent);
		}
	
		intptr_t 
		virtual showModal(Window* parent = nullptr)
		{
			return this->showModal(win::ProcessModule, parent);
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
	
		Response
		virtual onPaint(PaintWindowEventArgs args) override {
			args.beginPaint();
			this->LookNFeel->draw(*this, args);
			args.endPaint();
			return TRUE;
		}

		Response
		virtual onOfferMessage(::UINT message, ::WPARAM wParam, ::LPARAM lParam) override
		{
			switch (message) {
			case WM_INITDIALOG: 
				return this->onInitDialog({wParam,lParam});
			}

			return base::onOfferMessage(message, wParam, lParam);
		} 
	
		Response
		virtual onOfferNotification(::UINT notification) override {
			switch (notification) {
			case IDOK:
			case IDCANCEL:
				if (this->DisplayMode == DialogMode::Modal) {
					this->endModal(static_cast<uint16_t>(notification));
					return TRUE;
				}
			}

			return Unhandled;
		}
	
		void
		virtual onRaiseMessageEvent(::UINT message, ::WPARAM wParam, ::LPARAM lParam) override
		{
			switch (message) {
			case WM_INITDIALOG: 
				this->Initialized.raise(*this, InitDialogEventArgs{wParam,lParam});
				return;
			}

			base::onRaiseMessageEvent(message, wParam, lParam);
		} 

		::LRESULT 
		virtual onRouteUnhandled(::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
			return ::CallWindowProc(this->wndcls().OriginalWndProc, this->Handle, message, wParam, lParam);
		}

	private:
		std::optional<intptr_t>
		createInternal(win::Module source, DialogMode mode, Window* parent)
		{
			// Subclass the dialog prior to creation
			DialogTemplate customTemplate = this->Template;
			customTemplate.subclassDialog(this->wndcls().Name);

			// BUG: Prevent callers from wrapping more than one window handle using the same C++ object
			
			// Store any early-bound controls provided to our constructor
			this->BoundControls += this->EarlyBoundControls.to_dictionary();

			// Subclass each bound control prior to creation
			if (!this->BoundControls.empty())
				customTemplate.subclassControls(this->BoundControls);

			// Offer derived classes opportunity to modify the template
			this->onLoadDialog(customTemplate);
			
			// Aggregate all template customizations into a new template resource
			DialogTemplateBlob blob = DialogTemplateWriter{}.writeTemplate(customTemplate);
			
			// Indirectly pass our custom window creation data to the dialog's WM_NCCREATE handler
			//  by storing it temporarily in a static variable because the APIs for creating dialogs
			//  do not support passing data into the window-creation portion of dialog construction.
			Dialog::DialogCreationParameter = CreationData(this);

			// Transition internal state
			this->DisplayMode.emplace(mode);
			auto const onExit = this->Debug.setTemporaryState(ProcessingState::BeingCreated);
		
			// [MODAL] Display, block, and return result
			auto const container = source.handle();
			auto const owner = parent ? parent->handle() : nullptr;
			if (mode == DialogMode::Modal) { 
				auto result = ::DialogBoxIndirectW(container, blob, owner, this->DialogProc);
				if (result == -1)
					win::LastError{}.throwIfError("Failed to display '{}' dialog", to_string(this->DialogId));

				this->DisplayMode = std::nullopt;
				return result;
			}

			// [MODELESS] Display, set handle, and return nothing
			this->Handle = ::CreateDialogIndirectW(container, blob, owner, this->DialogProc);
			return std::nullopt;
		}
	};
}	// namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o