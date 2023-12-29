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
#include "controls/CountingIterator.h"
#include "controls/ListBoxItemData.h"
#include "controls/ListBoxStyle.h"
#include "forms/WindowClass.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	enum class ListBoxFeature : uint32_t { 
		None, 
		Headings = 0x01,
		Icons = 0x04,
		BigIcons = 0x02,
	};
}
namespace core::meta 
{
	metadata bool Settings<bitwise_enum, forms::ListBoxFeature> = true;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	//! @brief	ComboBox supporting item text, item headings, custom fonts, and icons
	class ListBoxControl : public Control 
	{
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
		using base = Control;

	protected:
		class ListBoxNotificationDictionary : public forms::MessageDictionary {
			using base = forms::MessageDictionary;
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

		class ListBoxWindowClass : public forms::WindowClass {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			using const_reference = ListBoxWindowClass const&;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			::WNDPROC	OriginalWndProc;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			ListBoxWindowClass() : forms::WindowClass{win::ResourceId{WC_LISTBOX}}  {
				this->Name = win::ResourceId{L"Custom.LISTBOX"};
				this->OriginalWndProc = std::exchange(this->WndProc, Window::defaultMessageHandler);
				this->Style |= ClassStyle::GlobalClass;
				this->registér();
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		};
		
		//! @brief	Custom item data used for each element when in owner-draw mode
		using ItemData = ListBoxItemData;
		
		//! @brief	Proxy for a single item at a fixed index
		class ItemProxy {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			ListBoxControl*  Owner;
			int32_t          Index;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			ItemProxy(ListBoxControl& owner, int32_t idx) noexcept
			  : Owner{&owner}, 
			    Index{idx}
			{}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
			satisfies(ItemProxy, 
				NotDefaultConstructible,
				IsCopyable, 
				IsMovable,
				NotEqualityComparable,
				NotSortable
			);
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			Rect
			area() const noexcept {
				Rect r;
				ListBox_GetItemRect(this->Owner->handle(), this->Index, static_cast<::RECT*>(r));
				return r;
			}
			
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
				Invariant(this->Owner->style<ListBoxStyle>().test(ListBoxStyle::OwnerDrawVariable));
				return static_cast<uint32_t>(ListBox_GetItemHeight(this->Owner->handle(), this->Index));
			}
			
			std::wstring
			text() const {
				if (this->Owner->ownerDraw())
					return this->data<ItemData>()->Detail.Text;
				else if (auto const length = ListBox_GetTextLen(this->Owner->handle(), this->Index); !length)
					return {};
				else {
					std::wstring content(static_cast<size_t>(length), L'\0');
					ListBox_GetText(this->Owner->handle(), this->Index, &content[0]);
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
				if (::LRESULT itemData = ListBox_GetItemData(this->Owner->handle(), this->Index); !itemData)
					throw runtime_error{"Missing ListBox data for item {}", this->Index};
				else 
					return reinterpret_cast<AnyType*>(itemData);
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			void
			height(uint32_t individualItem) {
				Invariant(this->Owner->style<ListBoxStyle>().test(ListBoxStyle::OwnerDrawVariable));
				ListBox_SetItemHeight(this->Owner->handle(), this->Index, individualItem);
			}

			template <typename AnyType>
			void
			userData(AnyType* customUserData) {
				// When owner-draw is active, the item-data slot addresses an 'ItemData' object
				if (!this->Owner->ownerDraw())
					ListBox_SetItemData(this->Owner->handle(), this->Index, customUserData);
				else
					this->data<ItemData>()->UserData = static_cast<void*>(customUserData);
			}
		};
		
		//! @brief  Collection facade representing existing items
		class ItemCollection {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			using iterator = boost::transform_iterator<std::function<ItemProxy(int32_t)>, CountingIterator>;
			using const_iterator = boost::transform_iterator<std::function<ItemProxy const(int32_t)>, CountingIterator>;
			using reference = ItemProxy&;
			using const_reference = ItemProxy const&;
			using value_type = ItemProxy;
			using size_type = iterator::difference_type;
			using difference_type = iterator::difference_type;
			
			//! @bug  In C++20, @c boost::transform_iterator cannot model anything greater than input-iterator
			static_assert(!std::bidirectional_iterator<iterator>);
			static_assert(!std::forward_iterator<iterator>);
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			ListBoxControl& Owner;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			explicit
			ItemCollection(ListBoxControl& ctrl) noexcept
			  : Owner{ctrl}
			{}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
			satisfies(ItemCollection,
				NotDefaultConstructible,
				IsCopyConstructible, 
				IsMoveConstructible,
				NotCopyAssignable, 
				NotMoveAssignable,
				NotSortable,
				NotEqualityComparable
			);
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			ItemProxy
			back() const noexcept {
				return ItemProxy{this->Owner, 0};
			}

			const_iterator
			begin() const noexcept {
				return this->make_iterator<const_iterator>(0);
			}
		
			const_iterator
			end() const noexcept {
				return this->make_iterator<const_iterator>(this->size());
			}
			
			const_iterator
			cbegin() const noexcept {
				return this->make_iterator<const_iterator>(0);
			}
		
			const_iterator
			cend() const noexcept {
				return this->make_iterator<const_iterator>(this->size());
			}

			std::optional<ItemProxy>
			find(std::wstring_view item) const noexcept {
				if (size_type const idx = ListBox_FindStringExact(this->Owner.handle(), 0, item.data()); idx == CB_ERR)
					return nullopt;
				else
					return ItemProxy{this->Owner, idx};
			}

			ItemProxy
			front() const noexcept {
				return ItemProxy{this->Owner, 0};
			}

			uint32_t
			height() const {
				Invariant(!this->Owner.style<ListBoxStyle>().test(ListBoxStyle::OwnerDrawVariable));
				return static_cast<uint32_t>(this->Owner.send<LB_GETITEMHEIGHT>(0));
			}

			std::optional<ItemProxy>
			selected() const noexcept {
				if (size_type const idx = ListBox_GetCurSel(this->Owner.handle()); idx == CB_ERR)
					return nullopt;
				else 
					return ItemProxy{this->Owner, idx};
			}
			
			size_type 
			size() const noexcept {
				return ListBox_GetCount(this->Owner.handle());
			}
			
			std::optional<ItemProxy>
			substr(std::wstring_view substring) const noexcept {
				if (size_type const idx = ListBox_FindStringExact(this->Owner.handle(), 0, substring.data()); idx == CB_ERR)
					return nullopt;
				else
					return ItemProxy{this->Owner, idx};
			}

			ItemProxy
			operator[](size_type idx) const noexcept {
				return ItemProxy(this->Owner, idx);
			}

		private:
			template <nstd::AnyOf<iterator,const_iterator> AnyIterator>
			AnyIterator
			make_iterator(int32_t idx) const {
				return AnyIterator{
					CountingIterator{&this->Owner, idx},
					[this](int32_t n) { 
						return ItemProxy{this->Owner, n}; 
					}
				};
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			iterator
			begin() noexcept {
				return this->make_iterator<iterator>(0);
			}
		
			iterator
			end() noexcept {
				return this->make_iterator<iterator>(this->size());
			}
			
			void
			clear() noexcept {
				ListBox_ResetContent(this->Owner.handle());
			}
			
			void 
			focus(const_iterator pos) noexcept {
				ListBox_SetCaretIndex(this->Owner.handle(), pos->index());
			}
	
			void
			height(uint32_t allItems) {
				Invariant(!this->Owner.style<ListBoxStyle>().test(ListBoxStyle::OwnerDrawVariable));
				ListBox_SetItemHeight(this->Owner.handle(), 0, allItems);
			}
			
			iterator
			insert(const_iterator pos, std::wstring_view text) 
			{
				size_type idx{};
				// [NOT OWNER-DRAW] Store (or duplicate) a simple string (according to its 'HasStrings' style)
				if (!this->Owner.ownerDraw()) 
					idx = ListBox_InsertString(this->Owner.handle(), pos->index(), text.data());

				else {
					auto data = std::make_unique<ItemData>(text);
					
					// [HAS-STRINGS] Supplement item with non-visible text for screen-reader support 
					if (this->Owner.hasStrings()) {
						this->Owner.ExposingOwnerDrawItemsBugfix = {data->Detail, data->Heading};
						idx = ListBox_InsertString(this->Owner.handle(), pos->index(), data->Detail.Text.c_str());
						ListBox_SetItemData(this->Owner.handle(), idx, data.release());
						this->Owner.ExposingOwnerDrawItemsBugfix = nullopt;
					}
					// [NO-STRING] Only store our owner-draw data
					else
						idx = ListBox_InsertItemData(this->Owner.handle(), pos->index(), data.release());
				}
				return this->make_iterator<iterator>(idx);
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
					idx = ListBox_InsertString(this->Owner.handle(), pos->index(), data->Detail.Text.c_str());
					ListBox_SetItemData(this->Owner.handle(), idx, data.release());
					this->Owner.ExposingOwnerDrawItemsBugfix = nullopt;
				}
				// [NO-STRING] Only store our owner-draw data
				else
					idx = ListBox_InsertItemData(this->Owner.handle(), pos->index(), data.release());
				
				return this->make_iterator<iterator>(idx);
			}
			
			iterator
			insert(const_iterator      pos, 
			       std::wstring_view   text, 
			       std::wstring_view   heading, 
			       std::optional<Icon> icon = nullopt)
			{
				Invariant(this->Owner.features().test(ListBoxFeature::Headings));
				Invariant(this->Owner.style<ListBoxStyle>().test(ListBoxStyle::OwnerDrawVariable));
				return this->insert(pos, RichText{text}, RichText{heading}, icon);
			}
			
			iterator
			insert(const_iterator             pos, 
			       RichText                   text,
			       RichText                   heading,
			       std::optional<forms::Icon> icon = nullopt) 
			{
				Invariant(this->Owner.features().test(ListBoxFeature::Headings));
				Invariant(this->Owner.style<ListBoxStyle>().test(ListBoxStyle::OwnerDrawVariable));
				auto data = std::make_unique<ItemData>(text, heading, icon);
				size_type idx{};
				
				// [HAS-STRINGS] Supplement item with non-visible text for screen-reader support 
				if (this->Owner.hasStrings()) {
					this->Owner.ExposingOwnerDrawItemsBugfix = {data->Detail, data->Heading};
					idx = ListBox_InsertString(this->Owner.handle(), pos->index(), data->Heading->Text.c_str());
					ListBox_SetItemData(this->Owner.handle(), idx, data.release());
					this->Owner.ExposingOwnerDrawItemsBugfix = nullopt;
				}
				// [NO-STRING] Only store our owner-draw data
				else
					idx = ListBox_InsertItemData(this->Owner.handle(), pos->index(), data.release());
				
				return this->make_iterator<iterator>(idx);
			}
			
			void
			push_back(std::wstring_view text) {
				this->insert(this->make_iterator<const_iterator>(-1), text);
			}

			void
			push_back(RichText                   text,
			          std::optional<forms::Icon> icon = nullopt) {
				Invariant(this->Owner.ownerDraw());
				this->insert(this->make_iterator<const_iterator>(-1), text, icon);
			}
			
			void
			push_back(std::wstring_view   text, 
			          std::wstring_view   heading, 
			          std::optional<Icon> icon = nullopt) 
			{
				Invariant(this->Owner.features().test(ListBoxFeature::Headings));
				Invariant(this->Owner.style<ListBoxStyle>().test(ListBoxStyle::OwnerDrawVariable));
				this->insert(this->make_iterator<const_iterator>(-1), text, heading, icon);
			}
			
			void
			push_back(RichText                   text,
			          RichText                   heading,
			          std::optional<forms::Icon> icon = nullopt) 
			{
				Invariant(this->Owner.features().test(ListBoxFeature::Headings));
				Invariant(this->Owner.style<ListBoxStyle>().test(ListBoxStyle::OwnerDrawVariable));
				this->insert(this->make_iterator<const_iterator>(-1), text, heading, icon);
			}
			
			void
			scrollTo(const_iterator pos) noexcept {
				ListBox_SetTopIndex(this->Owner.handle(), pos->index());
			}
	
			void
			select(ItemProxy const& item) noexcept {
				ListBox_SetCurSel(this->Owner.handle(), item.index());
			}
			
			void
			select(const_iterator pos) noexcept {
				ListBox_SetCurSel(this->Owner.handle(), pos->index());
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
					IsCopyConstructible, 
					IsMoveConstructible,
					NotCopyAssignable, 
					NotMoveAssignable,
					NotSortable
				);

			private:
				bool 
				equal(const type& r) const noexcept {
					return this->ListBox.handle() == r.ListBox.handle()
						&& this->Index == r.Index;
				}

				size_t
				dereference() const noexcept {
					return this->Indicies[this->Index]; 
				}

				void 
				increment() noexcept {
					if (++this->Index == this->Indicies.size()) {
						this->Index = npos;
					}
				}
			};
	
		private:
			ListBoxControl&		ListBox;
		public:
		
		public:
			SelectedIndexCollection(ListBoxControl& listbox) noexcept 
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
			size() const noexcept {
				return (size_t)ListBox_GetSelCount(this->ListBox.handle());
			}
		};
		
	public:
		//! @brief	Temporary data passed to WM_MEASUREITEM handler to fix design flaw
		struct TemporaryMeasureItemData {
			RichText                Detail; 
			std::optional<RichText> Heading;
		};

		using WindowClass = ListBoxWindowClass;
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		SelectedIndexCollection		SelectedItems;
		ItemCollection				Items;
		WindowEvent                 SelectionChanged;
		
	private:
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
		ListBoxControl(uint16_t id) 
		  : Control{id}, 
		    SelectedItems(*this), 
		    Items(*this)
		{
			this->backColour(this->LookNFeel->control());
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
		satisfies(ListBoxControl,
			NotCopyable,
			NotMoveAssignable,
			NotEqualityComparable,
			NotSortable
		);

		ListBoxControl(ListBoxControl&& r) noexcept
		  : ListBoxControl{r.ident()}
		{}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		gsl::czstring
		static identifyNotification(::UINT notification) {
			ListBoxNotificationDictionary const  static names;
			return names.contains(notification) ? names.at(notification) : "????";
		}

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		size_t 
		caret() const noexcept {
			return (size_t)ListBox_GetCaretIndex(this->handle());
		}
	
		size_t 
		first() const noexcept {
			return (size_t)ListBox_GetTopIndex(this->handle());
		}
		
		nstd::bitset<ListBoxFeature>
		features() const noexcept {
			return base::features<ListBoxFeature>();
		}
	
		bool
		hasStrings() const noexcept {
			return this->style<ListBoxStyle>().test(ListBoxStyle::HasStrings);
		}
		
		std::optional<forms::Font>
		headingFont() const noexcept {
			return this->HeadingFont;
		}
		
		bool
		virtual ownerDraw() const noexcept override {
			return this->style<ListBoxStyle>().test(ListBoxStyle::OwnerDrawFixed|ListBoxStyle::OwnerDrawVariable);
		}
		
		WindowRole
		virtual role() const noexcept override {
			return WindowRole::List;
		}
		
		ListBoxWindowClass::const_reference
		virtual wndcls() const override {
			ListBoxWindowClass const  static wc;
			return wc;
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		void
		features(nstd::bitset<ListBoxFeature> newStyle) noexcept {
			base::features(newStyle);
		}
	
		void
		headingFont(const forms::Font& newFont) {
			this->HeadingFont = newFont;
		}
		
	protected:
		gsl::czstring
		virtual notificationName(::UINT notification) override {
			return ListBoxControl::identifyNotification(notification);
		}

		//Response 
		//virtual onControlColour(ControlColourEventArgs args) override
		//{
		//	// Apply the look-n-feel when owner-draw is disabled
		//	if (args.Managed == this) {
		//		args.Graphics.textColour(this->textColour());
		//		return *this->background();
		//	}

		//	return Unhandled;
		//}

		Response 
		virtual onEraseBackground(EraseBackgroundEventArgs args) override {
			if (!this->ownerDraw())
				return Unhandled;

			this->LookNFeel->erase(*this, args);
			return 0;
		}
		
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

		Response
		virtual onOfferNotification(::UINT notification) override {
			switch (notification) {
			case LBN_SELCHANGE:
				this->SelectionChanged.raise(*this);
				return 0;
			}
			return Unhandled;
		}
		
		::LRESULT 
		virtual onRouteUnhandled(::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
			return this->subclassedWndProc(message, wParam, lParam);
		}
	};
}	// namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o