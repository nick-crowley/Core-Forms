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
namespace core::meta 
{
	template <typename R>
	concept DialogResult = std::is_pointer_v<R> || sizeof(R) <= sizeof(intptr_t);
}
namespace core::forms
{
	class FormsExport Dialog : public Window 
	{
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		using base = Window;
		
		//! @brief  Manages capturing mouse input
		class MouseCaptureState {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
			Window&  Owner;
			bool     IsCaptured = false;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			MouseCaptureState(Window& owner) : Owner{owner}
			{}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			bool
			captured() const {
				return this->IsCaptured;
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			void
			capture() {
				::SetCapture(this->Owner.handle());
				this->IsCaptured = true;
			}

			void
			release() {
				::ReleaseCapture();
				this->IsCaptured = false;
			}
		};
		
		//! @brief  Window caption button
		class NonClientButton
		{
			MouseCaptureState          Mouse;
			Window&                    Owner;
			nstd::bitset<ButtonState>  State = ButtonState::None;

		public:
			WindowEvent   Clicked;

		public:
			NonClientButton(Window& owner)
			  : Mouse{owner},
			    Owner{owner}
			{}

		public:
			bool
			captured() const {
				return this->Mouse.captured();
			}

			nstd::bitset<ButtonState>
			state() const {
				return this->State;
			}

		public:
			void 
			onMouseDown(Rect const& wndRect) 
			{
				this->State = ButtonState::Pushed;
				this->Mouse.capture();
				this->Owner.onNonClientPaint(NonClientPaintEventArgs{this->Owner,Region{wndRect}});
			}

			void 
			onMouseUp(Rect const& wndRect) {
				bool const changed = (this->State != ButtonState::None);
				this->State = ButtonState::None;
				
				if (this->Mouse.captured())
					this->Mouse.release();

				if (changed) {
					this->Owner.onNonClientPaint(NonClientPaintEventArgs{this->Owner,Region{wndRect}});
					this->Clicked.raise(this->Owner);
				}
			}

			void 
			onMouseMove(Rect const& wndRect) {
				//! @todo  Update window caption button appearance on mouse-hover
				//! @todo  Extend forms::ButtonState enum to support mouse-hover
			}
		};

		//! @brief  Provides event-handling logic and maintains state for caption buttons
		class WindowCaption 
		{
			friend class Dialog;

		public:
			NonClientButton  CloseBtn;
			NonClientButton  MaximizeBtn;
			NonClientButton  MinimizeBtn;

		public:
			WindowCaption(Window& owner)
			  : CloseBtn{owner},
			    MinimizeBtn{owner},
			    MaximizeBtn{owner}
			{
			}

		private:
			Response
			onMouseUp(Dialog& owner, MouseEventArgs args) 
			{
				// Perform hit-test against the non-client area
				auto const bounds = owner.nonClient();
				auto const object = bounds.hitTest(owner.screenPoint(args.Position));
				auto const style = owner.style();
				
				// Delegate mouse-event to appropriate caption button
				if (this->MaximizeBtn.captured() || (object == WindowHitTest::MaxButton && style.test(WindowStyle::MaximizeBox)))
					this->MaximizeBtn.onMouseUp(bounds.MaximizeBtn);
				else if (this->MinimizeBtn.captured() || (object == WindowHitTest::MinButton && style.test(WindowStyle::MinimizeBox)))
					this->MinimizeBtn.onMouseUp(bounds.MinimizeBtn);
				else if (this->CloseBtn.captured() || (object == WindowHitTest::CloseButton && style.test(WindowStyle::SysMenu)))
					this->CloseBtn.onMouseUp(bounds.CloseBtn);
				else
					return Unhandled;

				return 0;
			}
			
			Response
			onNonClientMouseDown(Dialog& owner, NonClientMouseEventArgs args) 
			{
				// Repeat hit-test for our custom non-client area
				auto const bounds = owner.nonClient();
				auto const object = bounds.hitTest(args.Position);
				auto const style = owner.style(); 

				// Delegate mouse-event to appropriate caption button
				if (object == WindowHitTest::MaxButton && style.test(WindowStyle::MaximizeBox))
					this->MaximizeBtn.onMouseDown(bounds.MaximizeBtn);
				else if (object == WindowHitTest::MinButton && style.test(WindowStyle::MinimizeBox))
					this->MinimizeBtn.onMouseDown(bounds.MinimizeBtn);
				else if (object == WindowHitTest::CloseButton && style.test(WindowStyle::SysMenu))
					this->CloseBtn.onMouseDown(bounds.CloseBtn);
				else
					return Unhandled;

				return 0;
			}

			Response
			onNonClientMouseMove(Dialog& owner, NonClientMouseEventArgs args) 
			{
				// Repeat hit-test for our custom non-client area
				auto const bounds = owner.nonClient();
				auto const object = bounds.hitTest(args.Position);
				auto const style = owner.style();

				// Delegate mouse-event to appropriate caption button
				if (object == WindowHitTest::MaxButton && style.test(WindowStyle::MaximizeBox))
					this->MaximizeBtn.onMouseMove(bounds.MaximizeBtn);
				else if (object == WindowHitTest::MinButton && style.test(WindowStyle::MinimizeBox))
					this->MinimizeBtn.onMouseMove(bounds.MinimizeBtn);
				else if (object == WindowHitTest::CloseButton && style.test(WindowStyle::SysMenu))
					this->CloseBtn.onMouseMove(bounds.CloseBtn);
				else
					return Unhandled;

				return 0;
			}
		};

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

		public:
			using const_iterator = ControlAddressCollection::const_iterator;
			using iterator = ControlAddressCollection::iterator;
			using value_type = ControlAddressCollection::value_type;
			using reference = ControlAddressCollection::reference;
			using const_reference = ControlAddressCollection::const_reference;
			using size_type = ControlAddressCollection::size_type;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			ControlAddressCollection  Controls;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			template <nstd::InputRangeConvertibleTo<Control*> AnyCollection>
				requires (!std::same_as<std::remove_cvref_t<AnyCollection>, EarlyBoundControlCollection>)
			explicit constexpr
			EarlyBoundControlCollection(AnyCollection&& controls)
				: Controls(std::from_range, std::forward<AnyCollection>(controls))
			{}
			
			explicit constexpr
			EarlyBoundControlCollection(std::initializer_list<Control*> controls)
				: Controls(controls)
			{}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			satisfies(EarlyBoundControlCollection,
				constexpr IsSemiRegular
			);
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			const_iterator constexpr
			begin() const noexcept {
				return this->Controls.begin(); 
			}

			const_iterator constexpr
			end() const noexcept {
				return this->Controls.end(); 
			}

			size_type constexpr
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
		class FormsExport DialogWindowClass : public forms::WindowClass {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			using const_reference = DialogWindowClass const&;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			::WNDPROC	OriginalWndProc;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			DialogWindowClass() : forms::WindowClass(win::ResourceId::parse(WC_DIALOG))
			{
				this->Name = win::ResourceId{L"Custom.DIALOG"};
				this->OriginalWndProc = std::exchange(this->WndProc, Dialog::interceptMessageHandler);
				this->Style |= ClassStyle::GlobalClass;
				this->registér();
			}
		};

		using WindowClass = DialogWindowClass;

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
		::DLGPROC const					DialogProc = Dialog::defaultDialogHandler;
		DialogTemplate const            Template;
		EarlyBoundControlCollection const EarlyBoundControls;
		ControlDictionary               BoundControls;
		WindowCaption                   Caption;

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
			EarlyBoundControls{controls},
			Caption{*this}
		{
			this->Caption.CloseBtn.Clicked += {*this, &Dialog::CloseBtn_Clicked};
			this->Caption.MaximizeBtn.Clicked += {*this, &Dialog::MaximizeBtn_Clicked};
			this->Caption.MinimizeBtn.Clicked += {*this, &Dialog::MinimizeBtn_Clicked};

			ControlRegistration::ensureRegistered();
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	protected:
		::INT_PTR 
		static CALLBACK defaultDialogHandler(::HWND hDlg, ::UINT message, ::WPARAM wParam, ::LPARAM lParam)
		{
			WndProcLoggingSentry logEntry(message);
			try {
				Response response;
				
				// Search for the C++ object managing this handle
				if (Dialog* dlg = Window::ExistingWindows.find<Dialog>(hDlg); !dlg) 
					// [UNMANAGED] Shouldn't be possible to receive messages for dialog prior to WM_NCCREATE saving its handle
					throw runtime_error{"No associated dialog object"};
				else
				{
					// Offer the message to the C++ object managing this handle
					response = dlg->offerMessage(message, wParam, lParam);
					Invariant(response.Status != Response::Invalid);

					// Raise equivalent event, if any, after processing completed
					dlg->raiseMessageEvent(message, wParam, lParam);

					// [HANDLED] Return the result provided
					if (response.Status == Response::Handled) 
					{
						logEntry.setResult(Response::Handled, *response.Value);
						if (message != WM_NOTIFY) 
							return *response.Value;

						// [WM_NOTIFY] Return result differently when running in a dialog procedure
						dlg->setMessageResult(response);
						return TRUE;
					}
				}
				
				// [UNHANDLED/ERROR] Inform ::DefWindowProc() we didn't handle this message
				logEntry.setResult(Response::Unhandled, FALSE);
				return FALSE;
			} 
			catch (const std::exception& e) {
				logEntry.setException(e);
				clog << Failure{"Exception processing {}: {}", Window::MessageDatabase[message].Name, e.what()};
				return (INT_PTR)FALSE;
			}
		}

		::LRESULT 
		static CALLBACK interceptMessageHandler(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam)
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
				Dialog::DialogCreationParameter = nullopt;

				return Window::defaultMessageHandler(hWnd, message, wParam, (LPARAM)&replacement);
			} 

			return Window::defaultMessageHandler(hWnd, message, wParam, lParam);
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		WindowCaption const&
		caption() const {
			return this->Caption;
		}

		Rect
		mapRect(Rect rc) const noexcept {
			::MapDialogRect(this->handle(), rc);
			return rc;
		}
	
		NonClientLayout 
		nonClient() const noexcept
		{
			return this->LookNFeel->nonClient(Coords::Screen, this->style(), this->wndRect(), this->clientRect(HWND_DESKTOP));
		}

		WindowRole
		virtual role() const noexcept override {
			return WindowRole::Dialog;
		}

		DialogWindowClass::const_reference
		virtual wndcls() const override {
			WindowClass const  static wc;
			return wc;
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		void 
		createEmbedded(win::Module source, Window& parent, Border border)
		{
			auto const Area = parent.clientRect() - border;
			this->createInternal(source, DialogMode::NonModal, &parent);
			this->move(Area.topLeft());
			this->resize(Area.size());
		}

		void 
		createEmbedded(Window& parent, Border border)
		{
			this->createEmbedded(win::ProcessModule, parent, border);
		}

		void 
		createModeless(win::Module source, Window* parent = nullptr)
		{
			this->createInternal(source, DialogMode::NonModal, parent);
		}
		
		void 
		createModeless(Window* parent = nullptr)
		{
			this->createModeless(win::ProcessModule, parent);
		}
		
		template <meta::DialogResult Result = intptr_t>
		Result
		showModal(win::Module source, Window* parent = nullptr)
		{
			return nstd::cstyle_cast<Result>(*this->createInternal(source, DialogMode::Modal, parent));
		}
	
		template <meta::DialogResult Result = intptr_t>
		Result 
		showModal(Window* parent = nullptr)
		{
			return this->showModal<Result>(win::ProcessModule, parent);
		}
	
	protected:
		void 
		endModal(uint16_t const id) noexcept 
		{
			::EndDialog(this->handle(), id);
		}
		
		Response 
		virtual onControlColour(ControlColourEventArgs args) override
		{
			if (args.Managed && args.Managed != this)
				return args.Managed->onControlColour(args);

			// Apply the current look-n-feel to dialogs but also unmanaged static controls so that
			//  applications don't have to bind every static label in the dialog simply to achieve
			//  a consistent appearance.
			else if (args.Source == ControlColourEventArgs::Dialog
			      || (!args.Managed && args.Source == ControlColourEventArgs::Static)) {
				args.Graphics.textColour(this->textColour());
				return *this->background();
			}

			return Unhandled;
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
			return FALSE;
		}
	
		Response
		virtual onMouseUp(MouseEventArgs args) override
		{
			// Delegate to custom non-client area, if any
			if (this->LookNFeel->customCaption())
				if (auto const r = this->Caption.onMouseUp(*this, args); r)
					return r;

			return Unhandled;
		}
		
		Response
		virtual onNonClientActivate(NonClientActivateEventArgs args) override 
		{
			// Repaint custom non-client area, if any
			if (this->LookNFeel->customCaption()) {
				this->onNonClientPaint(NonClientPaintEventArgs{std::move(args)});
				return args.AllowActivation;
			}

			return Unhandled;
		}
	
		Response
		virtual onNonClientCalculate(NonClientCalculateEventArgs args) 
		{
			// Calculate client area from custom non-client area, if any
			if (this->LookNFeel->customCaption()) {
				args.ClientArea = this->LookNFeel->clientRect(*this, args.ProposedWindow);
				return 0;
			}

			return Unhandled;
		}

		Response
		virtual onNonClientHitTest(NonClientHitTestEventArgs args) override 
		{
			// Perform hit-test against custom non-client area, if any
			if (this->LookNFeel->customCaption())
				return this->nonClient().hitTest(args.Position);
			
			return Unhandled;
		}
		
		Response
		virtual onNonClientMouseDown(NonClientMouseEventArgs args) override 
		{
			// Delegate to custom non-client area, if any
			if (this->LookNFeel->customCaption())
				if (auto const r = this->Caption.onNonClientMouseDown(*this, args); r)
					return r;

			// Allow remaining presses to retain existing functionality
			// * Moving the window by pressing the title-bar
			// * Display the system context-menu by double-pressing its button
			// * Resizing the window by dragging the frame
			return Unhandled;
		}
		
		Response
		virtual onNonClientMouseMove(NonClientMouseEventArgs args) 
		{	
			// Delegate to custom non-client area, if any
			if (this->LookNFeel->customCaption())
				if (auto const r = this->Caption.onNonClientMouseMove(*this, args); r)
					return r;

			return Unhandled;
		}
	
		Response
		virtual onNonClientPaint(NonClientPaintEventArgs args) override 
		{
			if (this->LookNFeel->customCaption())
				return this->LookNFeel->draw(*this, args);

			return Unhandled;
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
			return ::CallWindowProc(this->wndcls().OriginalWndProc, this->handle(), message, wParam, lParam);
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
					
					// [CENTRE] Reposition so horizontally centred; maintain original size (ie. horizontal translation)
					if (anchors.test(Side::Centre))
					{
						newPosition.Right = (newDimensions.Width + origPosition.width()) / 2;
						newPosition.Left = newPosition.Right - origPosition.width();
					}
					// [RIGHT] Maintain original distance to dialog's right-edge  (ie. horizontal stretch)
					else if (anchors.test(Side::Right))
					{
						newPosition.Right = newDimensions.Width - (origDimensions.Width - origPosition.Right);
						// [¬LEFT] Move right to maintain original size
						if (!anchors.test(Side::Left))
							newPosition.Left = newPosition.Right - origPosition.width();
					}

					// [VCENTRE] Reposition so horizontally centred; maintain original size (ie. horizontal translation)
					if (anchors.test(Side::VCentre))
					{
						newPosition.Bottom = (newDimensions.Height + origPosition.height()) / 2;
						newPosition.Top = newPosition.Bottom - origPosition.height();
					}
					// [BOTTOM] Maintain original distance to dialog's bottom-edge  (ie. vertical stretch)
					else if (anchors.test(Side::Bottom))
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
	
	private:
		std::optional<intptr_t>
		createInternal(win::Module source, DialogMode mode, Window* parent)
		{
			// Aggregate all template customizations into a new template resource
			DialogTemplateBlob const blob = this->loadTemplate();
			
			// Indirectly pass our custom window creation data to the dialog's WM_NCCREATE handler
			//  by storing it temporarily in a static variable because the APIs for creating dialogs
			//  do not support passing data into the window-creation portion of dialog construction.
			Dialog::DialogCreationParameter = CreationData(this);

			// Transition internal state
			this->DisplayMode.emplace(mode);
			auto const onExit = this->setTemporaryState(ProcessingState::BeingCreated);
		
			// [MODAL] Display, block, and return result
			auto const container = source.handle();
			auto const owner = parent ? parent->handle() : nullptr;
			if (mode == DialogMode::Modal) { 
				auto result = ::DialogBoxIndirectW(container, blob, owner, this->DialogProc);
				if (result == -1)
					win::LastError{}.throwIfError("Failed to display '{}' dialog", to_string(this->DialogId));

				this->DisplayMode = nullopt;
				return result;
			}

			// [MODELESS] Display, set handle, and return nothing
			if (auto* const dlg = ::CreateDialogIndirectW(container, blob, owner, this->DialogProc); dlg)
				this->attach(dlg);
			return nullopt;
		}
		
		DialogTemplateBlob
		loadTemplate() {
			DialogTemplate customTemplate = this->Template;

			// Store any early-bound controls provided to our constructor
			//! @bug  Prevent callers from wrapping more than one window handle using the same C++ object
			//! @bug  Early-bound controls should not be copied when displaying this dialog for a second time
			this->BoundControls += this->EarlyBoundControls.to_dictionary();
			
			// Subclass the dialog window
			customTemplate.subclassDialog(this->wndcls().Name);

			// Subclass each bound control
			if (!this->BoundControls.empty())
				customTemplate.subclassControls(this->BoundControls);

			// Set dialog font from look-n-feel
			auto initialFont = this->LookNFeel->default();
			customTemplate.Font = std::move(initialFont.Name);
			customTemplate.Height = std::to_underlying(-initialFont.Height);

			// Offer derived classes opportunity to modify the template
			this->onLoadDialog(customTemplate);
			
			// Aggregate all template customizations into a new template resource
			return DialogTemplateWriter{}.writeTemplate(customTemplate);
		}
		
		void
		setMessageResult(Response const& r) {
			::SetWindowLongPtr(this->handle(), DWLP_MSGRESULT, *r.Value);
		}
		
	private:
		void 
		CloseBtn_Clicked(Window& self) 
		{
			this->destroy();
		}

		void 
		MaximizeBtn_Clicked(Window& self) 
		{
			if (this->maximized())
				this->restore();
			else
				this->maximize();
		}

		void 
		MinimizeBtn_Clicked(Window& self) 
		{
			if (!this->minimized())
				this->minimize();
		}
	};
}	// namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-~o Test Code o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms::testing
{
	struct DialogSpy : Dialog 
	{
		//! @test  Verify @c Dialog::EarlyBoundControlCollection can be constructed
		static_assert(EarlyBoundControlCollection{}.size() == 0);
		
		//! @test  Verify @c Dialog::EarlyBoundControlCollection::size() returns number of controls provided at construction
		static_assert(EarlyBoundControlCollection{{nullptr, nullptr}}.size() == 2);

		//! @test  Verify @c Dialog::EarlyBoundControlCollection move-constructor transfers all elements
		static_assert(EarlyBoundControlCollection{
			EarlyBoundControlCollection{{nullptr, nullptr}}
		}.size() == 2);

		//! @test  Verify @c Dialog::EarlyBoundControlCollection copy-constructor copies all elements
		static_assert(EarlyBoundControlCollection{
			(EarlyBoundControlCollection const&)EarlyBoundControlCollection{{nullptr, nullptr}}
		}.size() == 2);
	};
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o