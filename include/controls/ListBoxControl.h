#pragma once
#include "formsFramework.h"
#include "windows/Window.h"
#include "windows/WindowClass.h"
#include "controls/ListBoxStyle.h"
#include "system/MessageDictionary.h"

class ListBoxControl : public Window {
protected:
	class ListBoxNotificationDictionary : public MessageDictionary {
		using base = MessageDictionary;
	public:
		ListBoxNotificationDictionary() : base({
#define MakeMessageName(msg)  { msg, #msg }
			MakeMessageName(LBN_ERRSPACE),
			MakeMessageName(LBN_SELCHANGE),
			MakeMessageName(LBN_DBLCLK),
			MakeMessageName(LBN_SELCANCEL),
			MakeMessageName(LBN_SETFOCUS),
			MakeMessageName(LBN_KILLFOCUS)
#undef MakeMessageName
			})
		{}
	};

public:
	class ListBoxWindowClass : public WindowClass {
	public:
		::WNDPROC	OriginalMessageHandler;

	public:
		ListBoxWindowClass() : WindowClass{ResourceId{WC_LISTBOX}}  {
			this->name(ResourceId{L"Custom.LISTBOX"});
			this->OriginalMessageHandler = std::exchange(this->lpfnWndProc, Window::DefaultMessageHandler);
			this->register$();
		}
	};

	class Item {
		ListBoxControl&		ListBox;
		size_t				Index;

	public:
		Item(ListBoxControl& list, size_t idx) 
			: ListBox(list), Index(idx)
		{}

	public:
		Rect
		area() const {
			Rect r;
			ListBox_GetItemRect(this->ListBox.handle(), this->Index, static_cast<::RECT*>(r));
			return std::move(r);
		}

		template <typename Pointer>
		Pointer
		data() const {
			return (Pointer)ListBox_GetItemData(this->ListBox.handle(), this->Index);
		}

		std::wstring
		text() const {
			size_t const length = ListBox_GetTextLen(this->ListBox.handle(), this->Index)+1;
			std::wstring buffer(length, L'\0');
			ListBox_GetText(this->ListBox.handle(), this->Index, buffer.data());
			buffer.pop_back();
			return buffer;
		}

		size_t
		height() const {
			return (size_t)ListBox_GetItemHeight(this->ListBox.handle(), this->Index);
		}

	public:
		template <typename Pointer>
		void
		data(Pointer ptr) {
			ListBox_SetItemData(this->ListBox.handle(), this->Index, ptr);
		}

		void
		height(size_t h) {
			ListBox_SetItemHeight(this->ListBox.handle(), this->Index, h);
		}
	};

	class ItemCollection {
	public:
		class ItemIterator : public boost::iterator_facade<ItemIterator, Item, boost::random_access_traversal_tag>{
			friend class boost::iterator_core_access;
			using type = ItemIterator;

			class ItemProxy {
				Item  Item$;

			public:
				explicit
				ItemProxy(Item itm) : Item${itm}
				{}

				operator
				Item&() {
					return this->Item$;
				}
			};

		private:
			ListBoxControl& ListBox;
			size_t			Index;

		public:
			ItemIterator(ListBoxControl& listbox, unsigned initialIdx) 
			  : ListBox(listbox), 
				Index(initialIdx)
			{}

			ItemIterator(ListBoxControl& listbox) noexcept
			  : ListBox(listbox), Index(ListBox_GetCount(listbox.handle()))
			{}
			
			satisfies(ItemIterator,
				NotDefaultConstructible,
				IsCopyConstructible,	//FIXME: IsCopyable,
				IsCopyAssignable,
				IsMoveConstructible,	//FIXME: IsMovable,
				IsMoveAssignable,
				NotSortable
			);

		private:
			bool 
			equal(const type& r) const {
				return this->ListBox.handle() == r.ListBox.handle()
					&& this->Index == r.Index;
			}

			ItemProxy
			dereference() const { 
				return ItemProxy{Item(this->ListBox, this->Index)};
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
		ListBoxControl& ListBox;

	public:
		ItemCollection(ListBoxControl& ctrl)
			: ListBox(ctrl)
		{}

	public:
		ItemIterator
		begin() const {
			return ItemIterator(this->ListBox, 0);
		}
		
		ItemIterator
		end() const {
			return ItemIterator(this->ListBox);
		}

		std::optional<size_t>
		find(std::wstring const& txt) const {
			signed idx = ListBox_FindStringExact(this->ListBox.handle(), 0, txt.c_str());
			return idx != -1 ? std::optional<size_t>(idx) : std::optional<size_t>{};
		}
		
		std::optional<size_t>
		search(std::wstring const& substring) const {
			signed idx = ListBox_FindString(this->ListBox.handle(), 0, substring.c_str());
			return idx != -1 ? std::optional<size_t>(idx) : std::optional<size_t>{};
		}

		size_t 
		size() const {
			return ListBox_GetCount(this->ListBox.handle());
		}

		Item
		operator[](size_t idx) const {
			return Item(this->ListBox, idx);
		}

	public:
		void
		insert(size_t idx, std::wstring const& txt) {
			ListBox_InsertString(this->ListBox.handle(), idx, txt.c_str());
		}

		void
		push_back(std::wstring const& txt) {
			ListBox_InsertString(this->ListBox.handle(), -1, txt.c_str());
		}
	};
	
	class SelectedIndexCollection
	{
	public:		
		class SelectedIndexIterator : public boost::iterator_facade<SelectedIndexIterator, size_t, boost::forward_traversal_tag>{
			friend class boost::iterator_core_access;
		private:
			using type = SelectedIndexIterator;

			signed const 
			static npos = -1;

		private:
			ListBoxControl&		ListBox;
			signed				Index;
			std::vector<signed>	Indicies;

		public:
			SelectedIndexIterator(ListBoxControl& listbox, unsigned initialIdx) 
			  : ListBox(listbox), 
				Index(static_cast<signed>(initialIdx)), 
				Indicies(ListBox_GetSelCount(listbox.handle()), 0)
			{
				if (!this->Indicies.empty())
					ListBox_GetSelItems(listbox.handle(), this->Indicies.size(), this->Indicies.data());
				else
					this->Index = npos;
			}

			SelectedIndexIterator(ListBoxControl& listbox) noexcept
			  : ListBox(listbox), Index(npos)
			{}
			
			satisfies(SelectedIndexIterator,
				NotDefaultConstructible,
				IsCopyConstructible,	//FIXME: IsCopyable,
				IsCopyAssignable,
				IsMoveConstructible,	//FIXME: IsMovable,
				IsMoveAssignable,
				NotSortable
			);

		private:
			bool 
			equal(const type& r) const {
				return this->ListBox.handle() == r.ListBox.handle()
					&& this->Index == r.Index;
			}

			size_t
			dereference() const { 
				return this->Indicies[this->Index]; 
			}

			void 
			increment() { 
				if (++this->Index == this->Indicies.size()) {
					this->Index = npos;
				}
			}
		};
	
	private:
		ListBoxControl&		ListBox;
	public:
		
	public:
		SelectedIndexCollection(ListBoxControl& listbox) 
		  : ListBox(listbox)
		{}

	public:
		SelectedIndexIterator
		begin() const {
			return SelectedIndexIterator(this->ListBox, 0);
		}

		SelectedIndexIterator
		end() const {
			return SelectedIndexIterator(this->ListBox);
		}

		size_t
		size() const { 
			return (size_t)ListBox_GetSelCount(this->ListBox.handle());
		}
	};

public:
	SelectedIndexCollection		SelectedItems;
	ItemCollection				Items;

public:
	ListBoxControl() 
		: SelectedItems(*this), Items(*this)
	{}

public:
	size_t 
	caret() const {
		return (size_t)ListBox_GetCaretIndex(this->handle());
	}
	
	size_t 
	first() const {
		return (size_t)ListBox_GetTopIndex(this->handle());
	}
	
public:
	void 
	caret(size_t idx) {
		ListBox_SetCaretIndex(this->handle(), idx);
	}
	
	void
	first(size_t idx) {
		ListBox_SetTopIndex(this->handle(), idx);
	}
	
	void
	item_height(size_t h) {
		ListBox_SetItemHeight(this->handle(), 0, h);
	}
	
	Response 
	onEraseBackground(EraseBackgroundEventArgs args) override {
		if (!this->style<ListBoxStyle>().test(ListBoxStyle::OwnerDrawFixed|ListBoxStyle::OwnerDrawVariable))
			return Unhandled;

		this->LookNFeel->erase(*this, args);
		return 0;
	}

	Response 
	onOwnerDraw(OwnerDrawEventArgs args) override {
		if (args.Ident == this->ident()) {
			this->LookNFeel->draw(*this, args);
			return TRUE;
		}

		return Unhandled;
	}

	ListBoxWindowClass const& 
	wndcls() override {
		static ListBoxWindowClass c;
		return c;
	}

protected:
	const char*
	notification_name(::UINT notification) override {
		static const ListBoxNotificationDictionary names;
		return names.at(notification);
	}

	/*Response
	offer_notification(::UINT notification) override {
		auto const on_exit = this->Debug.setTemporaryState(
			{ProcessingState::NotificationProcessing, this->notification_name(notification)}
		); 
		switch (notification) {
		case BN_CLICKED:
			this->Clicked.raise();
			return 0;
		}

		return Unhandled;
	}*/

	::LRESULT 
	unhandled_message(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
		return ::CallWindowProc(this->wndcls().OriginalMessageHandler, hWnd, message, wParam, lParam);
	}
};
