#pragma once
#include "library/core.Forms.h"
#include "windows/Window.h"
#include "windows/WindowClass.h"
#include "system/MessageDictionary.h"
#include "controls/ButtonStyle.h"

namespace core::forms
{
	enum class ButtonState;
}
constdata bool core::meta::Settings<core::meta::bitwise_enum_t, core::forms::ButtonState> = true;

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

	class ButtonControl : public Window {
		class ButtonWindowClass : public WindowClass {
		public:
			::WNDPROC	OriginalMessageHandler;

		public:
			ButtonWindowClass() : WindowClass{ResourceId{WC_BUTTON}}  {
				this->name(ResourceId{L"Custom.BUTTON"});
				this->OriginalMessageHandler = std::exchange(this->lpfnWndProc, Window::DefaultMessageHandler);
				this->register$();
			}
		};
	
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
		ButtonControl() 
		{}

	public:
		ButtonWindowClass const& 
		wndcls() override {
			static ButtonWindowClass c;
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

		EnumBitset<ButtonState>
		state() const {
			return static_cast<ButtonState>(this->send<BM_GETSTATE>(0,0));
		}

	protected:
		const char*
		notification_name(::UINT notification) override {
			static const ButtonNotificationDictionary names;
			return names.at(notification);
		}

		Response
		offer_notification(::UINT notification) override {
			auto const on_exit = this->Debug.setTemporaryState(
				{ProcessingState::NotificationProcessing, this->notification_name(notification)}
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
		unhandled_message(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
			return ::CallWindowProc(this->wndcls().OriginalMessageHandler, hWnd, message, wParam, lParam);
		}
	};
} // namespace core::forms