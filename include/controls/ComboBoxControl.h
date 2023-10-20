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
#include "controls/ListBoxItemData.h"
#include "controls/RichText.h"
#include "graphics/Icon.h"
#include "forms/WindowClass.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	enum class ComboBoxFeature : uint32_t { 
		None, 
		Headings = 0x01,
		Icons = 0x04,
		BigIcons = 0x02,
	};
}
namespace core::meta 
{
	metadata bool Settings<bitwise_enum, forms::ComboBoxFeature> = true;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	//! @brief	ComboBox supporting item text, item headings, custom fonts, and icons
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
		
		//! @brief	Custom item data used for each element when in owner-draw mode
		using ItemData = ListBoxItemData;

		//! @brief	Facade for a single item at a fixed index
		class Item {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			ComboBoxControl*  Owner;
			int32_t           Index;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			Item(ComboBoxControl& owner, int32_t idx) noexcept
			  : Owner{&owner}, 
			    Index{idx}
			{}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
			satisfies(Item, 
				NotDefaultConstructible,
				IsCopyable, 
				IsMovable,
				NotEqualityComparable,
				NotSortable
			);
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			RichText
			detail() const {
				Invariant(this->Owner->ownerDraw());
				return this->data<ItemData>()->Detail;
			}

			std::optional<Icon>
			icon() const {
				Invariant(this->Owner->ownerDraw());
				return this->data<ItemData>()->Icon;
			}

			int32_t
			index() const noexcept {
				return this->Index;
			}
			
			std::optional<RichText>
			heading() const {
				Invariant(this->Owner->ownerDraw());
				return this->data<ItemData>()->Heading;
			}

			uint32_t
			height() const {
				Invariant(this->Owner->style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable));
				return static_cast<uint32_t>(this->Owner->send<CB_GETITEMHEIGHT>(this->Index));
			}
			
			std::wstring
			text() const {
				if (this->Owner->ownerDraw())
					return this->data<ItemData>()->Detail.Text;
				else if (auto const length = ComboBox_GetLBTextLen(this->Owner->handle(), this->Index); !length)
					return {};
				else {
					std::wstring content(static_cast<size_t>(length), L'\0');
					ComboBox_GetLBText(this->Owner->handle(), this->Index, &content[0]);
					return content;
				}
			}
			
			template <typename UserData>
			UserData*
			userData() const {
				// When owner-draw is active, the item-data slot addresses an 'ItemData' object
				if (!this->Owner->ownerDraw())
					return this->data<UserData>();
				else
					return static_cast<UserData*>(this->data<ItemData>()->UserData);
			}

		protected:
			template <typename AnyType>
			AnyType*
			data() const {
				if (::LRESULT itemData = ComboBox_GetItemData(this->Owner->handle(), this->Index); !itemData)
					throw runtime_error{"Missing ComboBox data for item {}", this->Index};
				else 
					return reinterpret_cast<AnyType*>(itemData);
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			void
			height(uint32_t individualItem) {
				Invariant(this->Owner->style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable));
				ComboBox_SetItemHeight(this->Owner->handle(), this->Index, individualItem);
			}

			template <typename AnyType>
			void
			userData(AnyType* customUserData) {
				// When owner-draw is active, the item-data slot addresses an 'ItemData' object
				if (!this->Owner->ownerDraw())
					ComboBox_SetItemData(this->Owner->handle(), this->Index, customUserData);
				else
					this->data<ItemData>()->UserData = static_cast<void*>(customUserData);
			}
		};

		class ItemCollection {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			template <nstd::AnyOf<Item,Item const> ValueType>
			class Iterator : public boost::iterator_facade<Iterator<ValueType>, ValueType, boost::random_access_traversal_tag, ValueType&, int32_t>
			{
				template <nstd::AnyOf<Item,Item const>>
				friend class Iterator;

				friend class boost::iterator_core_access;
				// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
			private:
				using type = Iterator<ValueType>;
				// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
			private:
				ComboBoxControl* Owner;
				int32_t			 Index;
				// o~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~o
			public:
				Iterator(ComboBoxControl& owner, int32_t initialIdx) noexcept
				  : Owner{&owner}, 
				    Index{initialIdx}
				{}

				explicit
				Iterator(ComboBoxControl& owner) noexcept
				  : Owner{&owner}, 
				    Index{ComboBox_GetCount(owner.handle())}
				{}
				
				template <nstd::AnyOf<Item const> Other>
					requires std::same_as<ValueType,Item>
				implicit
				Iterator(Iterator<Other> const& r) noexcept
				  : Owner{r.Owner},
				    Index{r.Index}
				{}
				// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~o
			public:
				satisfies(Iterator,
					NotDefaultConstructible,
					IsCopyable,
					IsMovable,
					NotSortable
				);
				// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

				// o~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~o
			public:
				int32_t
				index() const noexcept {
					return this->Index;
				}

				implicit
				operator int32_t() const noexcept {
					return this->Index;
				}

			private:
				template <nstd::AnyOf<Item,Item const> Other>
				bool 
				equal(const Iterator<Other>& r) const noexcept {
					return this->Owner->handle() == r.Owner->handle()
						&& this->Index == r.Index;
				}

				ValueType
				dereference() const noexcept { 
					return ValueType{*this->Owner, this->Index};
				}

				int32_t
				distance_to(const type& r) const noexcept {
					return r.Index - this->Index;
				}
				// o~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~o
			private:
				void 
				advance(int32_t n) noexcept { 
					this->Index += n;
				}

				void 
				decrement() noexcept { 
					--this->Index;
				}

				void 
				increment() noexcept { 
					++this->Index;
				}
			};
			
			using iterator = Iterator<Item>;
			using const_iterator = Iterator<Item const>;
			using reference = Item&;
			using const_reference = Item const&;
			using value_type = Item;
			using size_type = iterator::difference_type;
			using difference_type = iterator::difference_type;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			ComboBoxControl& Owner;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			explicit
			ItemCollection(ComboBoxControl& ctrl) noexcept
			  : Owner{ctrl}
			{}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
			satisfies(ItemCollection,
				NotDefaultConstructible,
				NotCopyable,
				NotMovable,
				NotSortable,
				NotEqualityComparable
			);
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			const_iterator
			begin() const noexcept {
				return const_iterator{this->Owner, 0};
			}
		
			const_iterator
			end() const noexcept {
				return const_iterator{this->Owner};
			}
			
			const_iterator
			cbegin() const noexcept {
				return const_iterator{this->Owner, 0};
			}
		
			const_iterator
			cend() const noexcept {
				return const_iterator{this->Owner};
			}

			std::optional<Item>
			find(std::wstring_view item) const noexcept {
				if (size_type const idx = ComboBox_FindStringExact(this->Owner.handle(), 0, item.data()); idx == CB_ERR)
					return nullopt;
				else
					return Item{this->Owner, idx};
			}
		
			uint32_t
			height() const {
				Invariant(!this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable));
				return ComboBox_GetItemHeight(this->Owner.handle());
			}

			std::optional<Item>
			selected() const noexcept {
				if (size_type const idx = ComboBox_GetCurSel(this->Owner.handle()); idx == CB_ERR)
					return nullopt;
				else 
					return Item{this->Owner, idx};
			}
			
			size_type 
			size() const noexcept {
				return ComboBox_GetCount(this->Owner.handle());
			}
			
			std::optional<Item>
			substr(std::wstring_view substring) const noexcept {
				if (size_type const idx = ComboBox_FindStringExact(this->Owner.handle(), 0, substring.data()); idx == CB_ERR)
					return nullopt;
				else
					return Item{this->Owner, idx};
			}

			Item
			operator[](size_type idx) const noexcept {
				return Item(this->Owner, idx);
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			iterator
			begin() noexcept {
				return iterator{this->Owner, 0};
			}
		
			iterator
			end() noexcept {
				return iterator{this->Owner};
			}
			
			void
			clear() noexcept {
				ComboBox_ResetContent(this->Owner.handle());
			}

			void
			height(uint32_t allItems) {
				Invariant(!this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable));
				ComboBox_SetItemHeight(this->Owner.handle(), 0, allItems);
			}
			
			iterator
			insert(const_iterator pos, std::wstring_view text) 
			{
				size_type idx{};
				// [NOT OWNER-DRAW] Store (or duplicate) a simple string (according to its 'HasStrings' style)
				if (!this->Owner.ownerDraw()) 
					idx = ComboBox_InsertString(this->Owner.handle(), pos, text.data());

				else {
					auto data = std::make_unique<ItemData>(text);
					
					// [HAS-STRINGS] Supplement item with non-visible text for screen-reader support 
					if (this->Owner.hasStrings()) {
						this->Owner.ExposingOwnerDrawItemsBugfix = {data->Detail, data->Heading};
						idx = ComboBox_InsertString(this->Owner.handle(), pos, data->Detail.Text.c_str());
						ComboBox_SetItemData(this->Owner.handle(), idx, data.release());
						this->Owner.ExposingOwnerDrawItemsBugfix = nullopt;
					}
					// [NO-STRING] Only store our owner-draw data
					else
						idx = ComboBox_InsertItemData(this->Owner.handle(), pos, data.release());
				}
				return iterator{this->Owner, idx};
			}
			
			iterator
			insert(const_iterator             pos, 
			       RichText                   text,
			       std::optional<forms::Icon> icon = nullopt) 
			{
				Invariant(this->Owner.ownerDraw());
				auto data = std::make_unique<ItemData>(text, nullopt, icon);
				size_type idx{};
				
				// [HAS-STRINGS] Supplement item with non-visible text for screen-reader support 
				if (this->Owner.hasStrings()) {
					this->Owner.ExposingOwnerDrawItemsBugfix = {data->Detail, data->Heading};
					idx = ComboBox_InsertString(this->Owner.handle(), pos, data->Detail.Text.c_str());
					ComboBox_SetItemData(this->Owner.handle(), idx, data.release());
					this->Owner.ExposingOwnerDrawItemsBugfix = nullopt;
				}
				// [NO-STRING] Only store our owner-draw data
				else
					idx = ComboBox_InsertItemData(this->Owner.handle(), pos, data.release());

				return iterator{this->Owner, idx};
			}
			
			iterator
			insert(const_iterator      pos, 
			       std::wstring_view   text, 
			       std::wstring_view   heading, 
			       std::optional<Icon> icon = nullopt)
			{
				Invariant(this->Owner.features().test(ComboBoxFeature::Headings));
				Invariant(this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable));
				return this->insert(pos, RichText{text}, RichText{heading}, icon);
			}
			
			iterator
			insert(const_iterator             pos, 
			       RichText                   text,
			       RichText                   heading,
			       std::optional<forms::Icon> icon = nullopt) 
			{
				Invariant(this->Owner.features().test(ComboBoxFeature::Headings));
				Invariant(this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable));
				auto data = std::make_unique<ItemData>(text, heading, icon);
				size_type idx{};
				
				// [HAS-STRINGS] Supplement item with non-visible text for screen-reader support 
				if (this->Owner.hasStrings()) {
					this->Owner.ExposingOwnerDrawItemsBugfix = {data->Detail, data->Heading};
					idx = ComboBox_InsertString(this->Owner.handle(), pos, data->Heading->Text.c_str());
					ComboBox_SetItemData(this->Owner.handle(), idx, data.release());
					this->Owner.ExposingOwnerDrawItemsBugfix = nullopt;
				}
				// [NO-STRING] Only store our owner-draw data
				else
					idx = ComboBox_InsertItemData(this->Owner.handle(), pos, data.release());
				
				return iterator{this->Owner, idx};
			}
			
			void
			push_back(std::wstring_view text) {
				this->insert(const_iterator{this->Owner,-1}, text);
			}

			void
			push_back(RichText                   text,
			          std::optional<forms::Icon> icon = nullopt) {
				Invariant(this->Owner.ownerDraw());
				this->insert(const_iterator{this->Owner,-1}, text, icon);
			}
			
			void
			push_back(std::wstring_view   text, 
			          std::wstring_view   heading, 
			          std::optional<Icon> icon = nullopt) 
			{
				Invariant(this->Owner.features().test(ComboBoxFeature::Headings));
				Invariant(this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable));
				this->insert(const_iterator{this->Owner,-1}, text, heading, icon);
			}
			
			void
			push_back(RichText                   text,
			          RichText                   heading,
			          std::optional<forms::Icon> icon = nullopt) 
			{
				Invariant(this->Owner.features().test(ComboBoxFeature::Headings));
				Invariant(this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable));
				this->insert(const_iterator{this->Owner,-1}, text, heading, icon);
			}

			void
			select(Item const& item) noexcept {
				ComboBox_SetCurSel(this->Owner.handle(), item.index());
			}

			void
			select(const_iterator pos) noexcept {
				ComboBox_SetCurSel(this->Owner.handle(), pos);
			}
		};

	public:
		//! @brief	Temporary data passed to WM_MEASUREITEM handler to fix design flaw
		struct TemporaryMeasureItemData {
			RichText                Detail; 
			std::optional<RichText> Heading;
		};

		using WindowClass = ComboBoxWindowClass;
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		ItemCollection   Items;
		WindowEvent      SelectionChanged;

	protected:
		forms::UnmanagedWindow  DroppedItemsList;
		forms::UnmanagedWindow  SelectedItemEdit;
		
	private:
		std::optional<forms::Font>   EditFont;
		std::optional<forms::Font>   HeadingFont;

		//! @bug  When 'HasStrings' and 'OwnerDraw' are both active and items are added, supplementary
		//!       item text should be provided for screen-readers; however, the API only permits item
		//!       data to be added retrospectively, so text must added first. This triggers a design
		//!       flaw whereby WM_MEASUREITEM is sent prior to item data being added. This fix is the 
		//!       suggested workaround.
		//! @see https://learn.microsoft.com/en-us/windows/win32/winauto/exposing-owner-drawn-combo-box-items
		std::optional<TemporaryMeasureItemData>  ExposingOwnerDrawItemsBugfix; 

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		implicit
		ComboBoxControl(uint16_t id) 
		  : base{id}, 
		    Items{*this}
		{
			this->Created += {*this, &ComboBoxControl::this_OnCreated};
			this->backColour(this->LookNFeel->window());
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		gsl::czstring
		static identifyNotification(::UINT notification) {
			ComboBoxNotificationDictionary const  static names;
			return names.at(notification);
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		bool
		dropped() const noexcept {
			return ComboBox_GetDroppedState(this->handle()) != FALSE;
		}

		Rect
		droppedRect() const noexcept {
			Rect rc;
			ComboBox_GetDroppedControlRect(this->handle(), rc);
			return rc;
		}
		
		forms::Font
		editFont() {
			return this->EditFont.value_or(this->font());
		}
		
		nstd::bitset<ComboBoxFeature>
		features() const noexcept {
			return base::features<ComboBoxFeature>();
		}
	
		bool
		hasStrings() const noexcept {
			return this->style<ComboBoxStyle>().test(ComboBoxStyle::HasStrings);
		}
		
		std::optional<forms::Font>
		headingFont() const noexcept {
			return this->HeadingFont;
		}
		
		ComboBoxInfo
		info() const noexcept {
			::COMBOBOXINFO info{sizeof ::COMBOBOXINFO}; 
			this->send<CB_GETCOMBOBOXINFO>(nullopt, (::LPARAM)&info);
			return ComboBoxInfo{info};
		}

		bool
		virtual ownerDraw() const noexcept override {
			return this->style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawFixed|ComboBoxStyle::OwnerDrawVariable);
		}
		
		WindowRole
		virtual role() const noexcept override {
			return WindowRole::ComboBox;
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		void
		editFont(const forms::Font& newFont) {
			Invariant(!this->exists());
			this->EditFont = newFont;
		}
		
		void
		features(nstd::bitset<ComboBoxFeature> newStyle) noexcept {
			base::features(newStyle);
		}
	
		void
		headingFont(const forms::Font& newFont) {
			this->HeadingFont = newFont;
		}
		
		Response 
		virtual onCommand(CommandEventArgs args) override
		{
			// [EDIT/LISTBOX] Ignore notification as controls are unmanaged
			if (args.Source == CommandEventArgs::Control) {
				if (args.Notification->Handle == this->DroppedItemsList.handle()
				 || args.Notification->Handle == this->SelectedItemEdit.handle())
					return Unhandled;
			}

			return base::onCommand(args);
		}
	
		Response 
		virtual onControlColour(ControlColourEventArgs args) override
		{
			// Apply the font and colour of the selected item
			if (args.Window == this->SelectedItemEdit) {
				if (auto const selected = this->Items.selected(); selected) {
					auto const element = selected->heading().value_or(selected->detail());
					args.Graphics.textColour(element.Colour.value_or(this->textColour()));
					args.Graphics.setFont(element.Font.value_or(this->editFont()));
					return *this->background();
				}
			}

			return Unhandled;
		}

		//! TODO: Delete item data
		Response
		virtual onDeleteItem(void* args) = delete;
		
		Response 
		virtual onMeasureItem(MeasureItemEventArgs args) override {
			if (args.Ident == this->ident()) {
				if (this->ExposingOwnerDrawItemsBugfix)
					args.Item.UserData = reinterpret_cast<uintptr_t>(&this->ExposingOwnerDrawItemsBugfix);
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
		
		ComboBoxWindowClass::const_reference
		virtual wndcls() const override {
			ComboBoxWindowClass const  static wc;
			return wc;
		}

	protected:
		gsl::czstring
		virtual notificationName(::UINT notification) override {
			return ComboBoxControl::identifyNotification(notification);
		}

		Response
		virtual onOfferNotification(::UINT notification) override {
			switch (notification) {
			case CBN_SELENDOK :
				this->SelectionChanged.raise(*this);
				return 0;
			}
			return Unhandled;
		}

		::LRESULT 
		virtual onRouteUnhandled(::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
			return this->subclassedWndProc(message, wParam, lParam);
		}

	private:
		void
		this_OnCreated([[maybe_unused]] Window& sender, [[maybe_unused]] CreateWindowEventArgs args) {
			auto const info = this->info();
			this->DroppedItemsList = info.DroppedItemList;
			this->SelectedItemEdit = info.SelectedItemEdit;
		}
	};

}	// namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o