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
#include "controls/Control.h"
#include "controls/ComboBoxStyle.h"
#include "controls/ComboBoxInfo.h"
#include "graphics/Icon.h"
#include "forms/WindowClass.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class ComboBoxControl : public Control 
	{
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
		using base = Control;

	protected:
		class ComboBoxNotificationDictionary : public forms::MessageDictionary {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			using base = forms::MessageDictionary;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
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
	
		class ComboBoxWindowClass : public forms::WindowClass {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			using const_reference = ComboBoxWindowClass const&;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			::WNDPROC	OriginalWndProc;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			ComboBoxWindowClass() : forms::WindowClass{win::ResourceId{WC_COMBOBOX}}  {
				this->Name = win::ResourceId{L"Custom.COMBOBOX"};
				this->OriginalWndProc = std::exchange(this->WndProc, Window::defaultMessageHandler);
				this->Style |= ClassStyle::GlobalClass;
				this->registér();
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		};
		
		struct ItemData {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
			std::wstring                 Text;
			AnyColour                    TextColour = SystemColour::WindowText;
			std::optional<std::wstring>  Title;
			std::optional<Icon>          Icon;
			std::optional<void*>         UserData = nullptr;

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
			ItemData(std::wstring_view text) 
			  : Text{text}
			{}

			ItemData(std::wstring_view text, std::wstring_view title) 
			  : Text{text}, Title{title}
			{}

			ItemData(std::wstring_view text, std::wstring_view title, forms::Icon icon) 
			  : Text{text}, Title{title}, Icon{icon}
			{}
		};

	public:
		class Item {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			ComboBoxControl&  Owner;
			size_t            Index;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			Item(ComboBoxControl& Combo, size_t idx) 
			  : Owner{Combo}, 
			    Index{idx}
			{}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		protected:
			template <typename Self>
			auto*
			internalData(this Self&& self) {
				if (::LRESULT itemData = self.Owner.send<CB_GETITEMDATA>(self.Index); !itemData)
					throw runtime_error{"Missing ComboBox data for item {}", self.Index};
				else {
					using item_data_t = nstd::mirror_cv_ref_t<Self,ItemData>;
					return reinterpret_cast<item_data_t*>(itemData);
				}
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			template <typename UserData>
			UserData*
			data() const {
				return static_cast<UserData*>(this->internalData()->UserData.value_or(nullptr));
			}

			std::optional<Icon>
			icon() const {
				Invariant(!this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::HasStrings));
				return this->internalData()->Icon;
			}

			size_t
			index() const {
				return this->Index;
			}

			std::wstring
			text() const {
				if (!this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::HasStrings))
					return this->internalData()->Text;
				else if (auto const length = ComboBox_GetLBTextLen(this->Owner.handle(), this->Index); !length)
					return {};
				else {
					std::wstring content(static_cast<size_t>(length), L'\0');
					ComboBox_GetLBText(this->Owner.handle(), this->Index, &content[0]);
					return content;
				}
			}
			
			std::optional<std::wstring>
			title() const {
				Invariant(!this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::HasStrings));
				return this->internalData()->Title;
			}

			uint32_t
			height() const {
				Invariant(this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable));
				return static_cast<uint32_t>(this->Owner.send<CB_GETITEMHEIGHT>(this->Index));
			}
			
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			template <typename UserData>
			void
			data(UserData* userData) {
				this->internalData()->UserData = userData;
			}

			void
			height(uint32_t individualItem) {
				Invariant(this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable));
				ComboBox_SetItemHeight(this->Owner.handle(), this->Index, individualItem);
			}
		};

		class ItemCollection {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			class ItemIterator : public boost::iterator_facade<ItemIterator, Item, boost::random_access_traversal_tag>{
				// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
			private:
				using type = ItemIterator;
				// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
			private:
				ComboBoxControl& Owner;
				size_t			 Index;
				// o~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~o
			public:
				ItemIterator(ComboBoxControl& owner, unsigned initialIdx) 
				  : Owner{owner}, 
				    Index{initialIdx}
				{}

				ItemIterator(ComboBoxControl& owner) noexcept
				  : Owner{owner}, 
				    Index{(size_t)ComboBox_GetCount(owner.handle())}
				{}
				// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~o
			public:
				satisfies(ItemIterator,
					NotDefaultConstructible,
					IsCopyable,
					IsMovable,
					NotSortable
				);
				// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

				// o~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~o
			private:
				bool 
				equal(const type& r) const {
					return this->Owner.handle() == r.Owner.handle()
						&& this->Index == r.Index;
				}

				Item
				dereference() const { 
					return Item{this->Owner, this->Index};
				}

				ptrdiff_t
				distance_to(const type& r) const {
					return static_cast<ptrdiff_t>(r.Index) - static_cast<ptrdiff_t>(this->Index);
				}

				// o~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~o
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
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			ComboBoxControl& Owner;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			ItemCollection(ComboBoxControl& ctrl)
			  : Owner{ctrl}
			{}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			ItemIterator
			begin() const {
				return ItemIterator{this->Owner, 0};
			}
		
			ItemIterator
			end() const {
				return ItemIterator{this->Owner};
			}

			std::optional<Item>
			find(std::wstring_view item) const {
				if (signed const idx = ComboBox_FindStringExact(this->Owner.handle(), 0, item.data()); idx == -1)
					return std::nullopt;
				else
					return Item{this->Owner, static_cast<size_t>(idx)};
			}
		
			uint32_t
			height() const {
				Invariant(!this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable));
				return ComboBox_GetItemHeight(this->Owner.handle());
			}

			std::optional<Item>
			selected() const {
				if (signed const idx = ComboBox_GetCurSel(this->Owner.handle()); idx == -1)
					return std::nullopt;
				else 
					return Item{this->Owner, static_cast<size_t>(idx)};
			}
			
			size_t 
			size() const {
				return ComboBox_GetCount(this->Owner.handle());
			}
			
			std::optional<Item>
			substr(std::wstring_view substring) const {
				if (signed const idx = ComboBox_FindStringExact(this->Owner.handle(), 0, substring.data()); idx == -1)
					return std::nullopt;
				else
					return Item{this->Owner, static_cast<size_t>(idx)};
			}

			Item
			operator[](size_t idx) const {
				return Item(this->Owner, idx);
			}

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			void
			height(uint32_t allItems) {
				Invariant(!this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable));
				ComboBox_SetItemHeight(this->Owner.handle(), 0, allItems);
			}

			void
			insert(size_t idx, std::wstring_view text) {
				this->insertAt(idx, text);
			}
			
			void
			insert(size_t idx, std::wstring_view text, std::wstring_view title) {
				this->insertAt(idx, std::make_unique<ItemData>(text, title));
			}

			void
			insert(size_t idx, std::wstring_view text, std::wstring_view title, Icon icon) {
				this->insertAt(idx, std::make_unique<ItemData>(text, title, icon));
			}

			void
			push_back(std::wstring_view text) {
				this->insertAt(static_cast<size_t>(-1), text);
			}
			
			void
			push_back(std::wstring_view text, std::wstring_view title) {
				this->insertAt(static_cast<size_t>(-1), std::make_unique<ItemData>(text, title));
			}

			void
			push_back(std::wstring_view text, std::wstring_view title, Icon icon) {
				this->insertAt(static_cast<size_t>(-1), std::make_unique<ItemData>(text, title, icon));
			}
		
			void
			select(Item const& item) {
				this->select(item.index());
			}

			void
			select(size_t idx) {
				ComboBox_SetCurSel(this->Owner.handle(), idx);
			}

		private:
			void
			insertAt(size_t idx, std::wstring_view text) {
				if (this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::HasStrings))
					ComboBox_InsertString(this->Owner.handle(), idx, text.data());
				else
					this->insertAt(idx, std::make_unique<ItemData>(text));
			}

			void
			insertAt(size_t idx, std::unique_ptr<ItemData> item) {
				ComboBox_InsertItemData(this->Owner.handle(), idx, item.release());
			}
		};
	
		using WindowClass = ComboBoxWindowClass;

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		ItemCollection   Items;

	protected:
		forms::UnmanagedWindow  DroppedItemsList;
		forms::UnmanagedWindow  SelectedItemEdit;
		
	private:
		Font             TitleFont = ComboBoxControl::makeTitleFont(StockFont::DefaultGui);

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		implicit
		ComboBoxControl(uint16_t id) 
		  : base{id}, 
		    Items{*this}
		{}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		gsl::czstring
		static identifyNotification(::UINT notification) {
			static const ComboBoxNotificationDictionary names;
			return names.at(notification);
		}

	private:
		Font
		static makeTitleFont(Font const& textFont) {
			return Font{*textFont.handle(), std::nullopt, 3*textFont.height() / 2};
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		bool
		dropped() const {
			return ComboBox_GetDroppedState(this->handle()) != FALSE;
		}

		Rect
		droppedRect() const {
			Rect rc;
			ComboBox_GetDroppedControlRect(this->handle(), rc);
			return rc;
		}

		ComboBoxInfo
		info() const {
			::COMBOBOXINFO info{sizeof ::COMBOBOXINFO}; 
			this->send<CB_GETCOMBOBOXINFO>(std::nullopt, (::LPARAM)&info);
			return ComboBoxInfo{info};
		}

		bool
		virtual ownerDraw() const override {
			return this->style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawFixed|ComboBoxStyle::OwnerDrawVariable);
		}
		
		WindowRole
		virtual role() const override {
			return WindowRole::ComboBox;
		}
		
		Font
		titleFont() const {
			return this->TitleFont;
		}
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		//! TODO: Delete item data
		Response
		virtual onDeleteItem(void* args) = delete;
		
		Response 
		virtual onMeasureItem(MeasureItemEventArgs args) override {
			if (args.Ident == this->ident()) {
				this->LookNFeel->measure(*this, args);
				return TRUE;
			}

			return Unhandled;
		}

		Response 
		virtual onOwnerDraw(OwnerDrawEventArgs args) override {
			if (args.Ident == this->ident()) {
				this->LookNFeel->draw(*this, args);
				return TRUE;
			}

			return Unhandled;
		}
		
		Response
		virtual onSetFont(SetWindowFontEventArgs args) override {
			// Generate title font automatically when window font changes (or is reset to default)
			this->titleFont(
				ComboBoxControl::makeTitleFont(args.Font ? Font{args.Font} : StockFont::SystemFixed)
			);
			return Unhandled;
		}

		void
		titleFont(const Font& newFont) {
			this->TitleFont = newFont;
		}
		
		ComboBoxWindowClass::const_reference
		virtual wndcls() const override {
			static ComboBoxWindowClass c;
			return c;
		}

	protected:
		gsl::czstring
		virtual notificationName(::UINT notification) override {
			return ComboBoxControl::identifyNotification(notification);
		}

		//Response
		//virtual onOfferNotification(::UINT notification) override {
		//	switch (notification) {
		//	case CBN_SELCHANGE:
		//		this->update();
		//		//this->Clicked.raise();
		//		return 0;
		//	}
		//	return Unhandled;
		//}

		::LRESULT 
		virtual onRouteUnhandled(::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
			if (message == WM_CREATE) {
				auto const result = ::CallWindowProc(this->wndcls().OriginalWndProc, this->handle(), message, wParam, lParam);
				auto const info = this->info();
				this->DroppedItemsList = info.DroppedItemList;
				this->SelectedItemEdit = info.SelectedItemEdit;
				return result;
			} 

			return ::CallWindowProc(this->wndcls().OriginalWndProc, this->handle(), message, wParam, lParam);
		}
	};
}	// namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o