#pragma once
#include "library/core.Forms.h"
#include "controls/Control.h"
#include "controls/ButtonStyle.h"
#include "windows/WindowClass.h"

namespace core::forms
{
	enum class ButtonState;
}
constdata bool core::meta::Settings<core::bitwise_enum, core::forms::ButtonState> = true;

namespace core::forms
{
	enum class ButtonState {	
		Unchecked      = BST_UNCHECKED,     // 
		Checked        = BST_CHECKED,       // 
		Indeterminate  = BST_INDETERMINATE, // 
		Pushed         = BST_PUSHED,        // 
		Focus          = BST_FOCUS,         // 
		None           = Unchecked,
	};

	class ButtonControl : public Control {
		using base = Control;

	public:
		class WindowClass : public forms::WindowClass {
		public:
			::WNDPROC	OriginalWndProc;

		public:
			WindowClass() : forms::WindowClass{ResourceId{WC_BUTTON}}  {
				this->Name = ResourceId{L"Custom.BUTTON"};
				this->OriginalWndProc = std::exchange(this->WndProc, Window::DefaultMessageHandler);
				this->Style |= ClassStyle::GlobalClass;
				this->register_();
			}
		};
	
	protected:
		class ButtonNotificationDictionary : public MessageDictionary {
			using base = MessageDictionary;
		public:
			ButtonNotificationDictionary() : base({
	#define MakeMessageName(msg)  { msg, #msg }
				MakeMessageName(BN_CLICKED),
				MakeMessageName(BN_PAINT),
				MakeMessageName(BN_HILITE),
				MakeMessageName(BN_UNHILITE),
				MakeMessageName(BN_DISABLE),
				MakeMessageName(BN_DOUBLECLICKED),
				MakeMessageName(BN_PUSHED),
				MakeMessageName(BN_UNPUSHED),
				MakeMessageName(BN_DBLCLK),
				MakeMessageName(BN_SETFOCUS),
				MakeMessageName(BN_KILLFOCUS)
	#undef MakeMessageName
				})
			{}
		};

	public:
		ButtonControl(uint16_t id)
		  : Control{id}
		{}
		
	public:
		bool
		ownerDraw() const override {
			return (this->style<ButtonStyle>() & ButtonStyle::TypeMask).test(ButtonStyle::OwnerDraw);
		}

	public:
		WindowClass const& 
		wndcls() override {
			static WindowClass c;
			return c;
		}
	
		Response 
		onOwnerDraw(OwnerDrawEventArgs args) override {
			if (args.Ident == this->ident()) {
				this->LookNFeel->draw(*this, args);
				return TRUE;
			}

			return Unhandled;
		}

		nstd::bitset<ButtonState>
		state() const {
			return static_cast<ButtonState>(this->send<BM_GETSTATE>(0,0));
		}

	protected:
		gsl::czstring
		notificationName(::UINT notification) override {
			static const ButtonNotificationDictionary names;
			return names.at(notification);
		}

		Response
		offerNotification(::UINT notification) override {
			auto const on_exit = this->Debug.setTemporaryState(
				{ProcessingState::NotificationProcessing, this->notificationName(notification)}
			); 

			switch (notification) {
			case BN_CLICKED:
				this->Clicked.raise(*this);
				return 0;
			
			case BN_PAINT:
			case BN_PUSHED:
			case BN_UNPUSHED:
			case BN_SETFOCUS:
			case BN_KILLFOCUS:
				InvalidateRect(this->handle(), 0, FALSE);
				UpdateWindow(this->handle());
				return 0;
			}

			return Unhandled;
		}

		::LRESULT 
		unhandledMessage(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
			return ::CallWindowProc(this->wndcls().OriginalWndProc, hWnd, message, wParam, lParam);
		}
	};
} // namespace core::forms