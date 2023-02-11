#pragma once
#include "library/core.Forms.h"
#include "controls/Control.h"
#include "controls/ComboBoxStyle.h"
#include "windows/WindowClass.h"

namespace core::forms
{
	class ComboBoxControl : public Control {
	public:
		class WindowClass : public forms::WindowClass {
		public:
			::WNDPROC	OriginalWndProc;

		public:
			WindowClass() : forms::WindowClass{ResourceId{WC_COMBOBOX}}  {
				this->Name = ResourceId{L"Custom.COMBOBOX"};
				this->OriginalWndProc = std::exchange(this->WndProc, Window::DefaultMessageHandler);
				this->Style |= ClassStyle::GlobalClass;
				this->register_();
			}
		};
		
	protected:
		class ComboBoxNotificationDictionary : public MessageDictionary {
			using base = MessageDictionary;
		public:
			ComboBoxNotificationDictionary() : base({
	#define MakeMessageName(msg)  { msg, #msg }
				MakeMessageName(CBN_ERRSPACE),
				MakeMessageName(CBN_SELCHANGE),
				MakeMessageName(CBN_DBLCLK),
				MakeMessageName(CBN_SETFOCUS),
				MakeMessageName(CBN_KILLFOCUS),
				MakeMessageName(CBN_EDITCHANGE),
				MakeMessageName(CBN_EDITUPDATE),
				MakeMessageName(CBN_DROPDOWN),
				MakeMessageName(CBN_CLOSEUP),
				MakeMessageName(CBN_SELENDOK),
				MakeMessageName(CBN_SELENDCANCEL)
	#undef MakeMessageName
				})
			{}
		};
	
		class Item {
			ComboBoxControl&  ComboBox;
			size_t            Index;

		public:
			Item(ComboBoxControl& Combo, size_t idx) 
				: ComboBox{Combo}, Index{idx}
			{}

		public:
			template <typename Pointer>
			Pointer
			data() const {
				return (Pointer)ComboBox_GetItemData(this->ComboBox.handle(), this->Index);
			}

			size_t
			index() const {
				return this->Index;
			}

			std::wstring
			text() const {
				std::wstring s(ComboBox_GetLBTextLen(this->ComboBox.handle(), this->Index), L'\0');
				ComboBox_GetLBText(this->ComboBox.handle(), this->Index, s.data());
				return std::move(s);
			}

			size_t
			height() const {
				return (size_t)this->ComboBox.send<CB_GETITEMHEIGHT>(/*this->Index*/0, 0);
			}

		public:
			template <typename Pointer>
			void
			data(Pointer ptr) {
				ComboBox_SetItemData(this->ComboBox.handle(), this->Index, ptr);
			}

			void
			height(size_t h) {
				ComboBox_SetItemHeight(this->ComboBox.handle(), this->Index, h);
			}
		};

		class ItemCollection {
		public:
			class ItemIterator : public boost::iterator_facade<ItemIterator, Item, boost::random_access_traversal_tag>{
				using type = ItemIterator;
			private:
				ComboBoxControl& ComboBox;
				size_t			Index;

			public:
				ItemIterator(ComboBoxControl& Combobox, unsigned initialIdx) 
				  : ComboBox{Combobox}, 
					Index{initialIdx}
				{}

				ItemIterator(ComboBoxControl& Combobox) noexcept
				  : ComboBox{Combobox}, Index{(size_t)ComboBox_GetCount(Combobox.handle())}
				{}
			
				satisfies(ItemIterator,
					NotDefaultConstructible,
					IsCopyable,
					IsMovable,
					NotSortable
				);

			private:
				bool 
				equal(const type& r) const {
					return this->ComboBox.handle() == r.ComboBox.handle()
						&& this->Index == r.Index;
				}

				Item
				dereference() const { 
					return Item{this->ComboBox, this->Index};
				}

				ptrdiff_t
				distance_to(const type& r) const {
					return static_cast<ptrdiff_t>(r.Index) - static_cast<ptrdiff_t>(this->Index);
				}

			private:
				void 
				advance(ptrdiff_t n) { 
					this->Index += n;
				}

				void 
				decrement() { 
					--this->Index;
				}

				void 
				increment() { 
					++this->Index;
				}
			};

		private:
			ComboBoxControl& ComboBox;

		public:
			ItemCollection(ComboBoxControl& ctrl)
			  : ComboBox{ctrl}
			{}

		public:
			ItemIterator
			begin() const {
				return ItemIterator{this->ComboBox, 0};
			}
		
			ItemIterator
			end() const {
				return ItemIterator{this->ComboBox};
			}

			std::optional<size_t>
			find(std::wstring const& txt) const {
				signed idx = ComboBox_FindStringExact(this->ComboBox.handle(), 0, txt.c_str());
				return idx != -1 ? std::optional<size_t>(idx) : std::optional<size_t>{};
			}
		
			std::optional<size_t>
			search(std::wstring const& substring) const {
				signed idx = ComboBox_FindString(this->ComboBox.handle(), 0, substring.c_str());
				return idx != -1 ? std::optional<size_t>(idx) : std::optional<size_t>{};
			}

			Item
			selected() const {
				return Item{this->ComboBox, *this->selectedIndex()};
			}

			std::optional<size_t>
			selectedIndex() const {
				auto const idx = ComboBox_GetCurSel(this->ComboBox.handle());
				return idx != -1 ? std::optional<size_t>{idx} : std::optional<size_t>{};
			}

			size_t 
			size() const {
				return ComboBox_GetCount(this->ComboBox.handle());
			}

		public:
			void
			insert(size_t idx, std::wstring const& txt) {
				ComboBox_InsertString(this->ComboBox.handle(), idx, txt.c_str());
			}
		
			void
			select(Item const& item) {
				this->select(item.index());
			}

			void
			select(size_t idx) {
				ComboBox_SetCurSel(this->ComboBox.handle(), idx);
			}

			void
			push_back(std::wstring const& txt) {
				ComboBox_InsertString(this->ComboBox.handle(), -1, txt.c_str());
			}
		};
	
	public:
		ItemCollection   Items;

	public:
		ComboBoxControl(uint16_t id) 
		  : Control{id}, 
		    Items{*this}
		{}
		
	public:
		bool
		ownerDraw() const override {
			return this->style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawFixed|ComboBoxStyle::OwnerDrawVariable);
		}

	public:
		Response 
		onOwnerDraw(OwnerDrawEventArgs args) override {
			return Unhandled;
		}

		WindowClass const& 
		wndcls() override {
			static WindowClass c;
			return c;
		}

	protected:
		gsl::czstring
		notificationName(::UINT notification) override {
			static const ComboBoxNotificationDictionary names;
			return names.at(notification);
		}

		//Response
		//offerNotification(::UINT notification) override {
		//  auto const on_exit = this->Debug.setTemporaryState(
		//		{ProcessingState::NotificationProcessing, this->notificationName(notification)}
		//	); 
		//	switch (notification) {
		//	case CBN_SELCHANGE:
		//		this->update();
		//		//this->Clicked.raise();
		//		return 0;
		//	}

		//	return Unhandled;
		//}

		::LRESULT 
		unhandledMessage(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
			return ::CallWindowProc(this->wndcls().OriginalWndProc, hWnd, message, wParam, lParam);
		}
	};
}	// namespace core::forms