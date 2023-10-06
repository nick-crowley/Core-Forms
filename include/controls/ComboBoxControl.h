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
	//! @brief	Display element of each owner-draw item
	struct ComboBoxItemElement {
		// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		std::wstring              Text;
		std::optional<AnyColour>  Colour = SystemColour::WindowText;
		std::optional<Font>       Font;
		// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		explicit
		ComboBoxItemElement(
			std::wstring_view          text, 
			std::optional<AnyColour>   colour = std::nullopt, 
			std::optional<forms::Font> font = std::nullopt
		) : Text{text}, 
			Colour{colour}, 
			Font{font}
		{}
		// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		satisfies(ComboBoxItemElement,
			IsSemiRegular,
			NotEqualityComparable,
			NotSortable
		);
	};

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
		
	public:
		//! @brief	Custom item data used for each element when in owner-draw mode
		struct ComboBoxItemData {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
			ComboBoxItemElement                Detail;   //!< Primary item content when no title present
			std::optional<ComboBoxItemElement> Heading;
			std::optional<Icon>                Icon;
			void*                              UserData = nullptr;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
			explicit
			ComboBoxItemData(std::wstring_view                text, 
			                 std::optional<std::wstring_view> heading = std::nullopt, 
			                 std::optional<forms::Icon>       icon = std::nullopt) 
			  : Detail{text}, 
			    Heading{heading}, 
			    Icon{icon}
			{}
			
			explicit
			ComboBoxItemData(ComboBoxItemElement                text, 
			                 std::optional<ComboBoxItemElement> heading = std::nullopt, 
			                 std::optional<forms::Icon>         icon = std::nullopt) 
			  : Detail{text}, 
			    Heading{heading}, 
			    Icon{icon}
			{}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
			satisfies(ComboBoxItemData,
				IsSemiRegular,
				NotEqualityComparable,
				NotSortable
			);
		};

	public:
		//! @brief	Facade for a single item at a fixed index
		class ComboBoxItem {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			ComboBoxControl&  Owner;
			size_t            Index;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			ComboBoxItem(ComboBoxControl& Combo, size_t idx) 
			  : Owner{Combo}, 
			    Index{idx}
			{}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			ComboBoxItemElement
			detail() const {
				Invariant(this->Owner.ownerDraw());
				return this->data<ComboBoxItemData>()->Detail;
			}

			std::optional<Icon>
			icon() const {
				Invariant(this->Owner.ownerDraw());
				return this->data<ComboBoxItemData>()->Icon;
			}

			size_t
			index() const {
				return this->Index;
			}
			
			std::optional<ComboBoxItemElement>
			heading() const {
				Invariant(this->Owner.ownerDraw());
				return this->data<ComboBoxItemData>()->Heading;
			}

			uint32_t
			height() const {
				Invariant(this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable));
				return static_cast<uint32_t>(this->Owner.send<CB_GETITEMHEIGHT>(this->Index));
			}
			
			std::optional<std::wstring>
			title() const {
				Invariant(this->Owner.ownerDraw());
				if (auto const hasTitle = this->data<ComboBoxItemData>()->Heading; hasTitle)
					return hasTitle->Text;
				return std::nullopt;
			}
			
			std::wstring
			text() const {
				if (this->Owner.ownerDraw())
					return this->data<ComboBoxItemData>()->Detail.Text;
				else if (auto const length = ComboBox_GetLBTextLen(this->Owner.handle(), this->Index); !length)
					return {};
				else {
					std::wstring content(static_cast<size_t>(length), L'\0');
					ComboBox_GetLBText(this->Owner.handle(), this->Index, &content[0]);
					return content;
				}
			}
			
			template <typename UserData>
			UserData*
			userData() const {
				// When owner-draw is active, the item-data slot addresses an 'ComboBoxItemData' object
				if (!this->Owner.ownerDraw())
					return this->data<UserData>();
				else
					return static_cast<UserData*>(this->data<ComboBoxItemData>()->UserData);
			}

		protected:
			template <typename AnyType>
			AnyType*
			data() const {
				if (::LRESULT itemData = ComboBox_GetItemData(this->Owner.handle(), this->Index); !itemData)
					throw runtime_error{"Missing ComboBox data for item {}", this->Index};
				else 
					return reinterpret_cast<AnyType*>(itemData);
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			void
			height(uint32_t individualItem) {
				Invariant(this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable));
				ComboBox_SetItemHeight(this->Owner.handle(), this->Index, individualItem);
			}

			template <typename AnyType>
			void
			userData(AnyType* customUserData) {
				// When owner-draw is active, the item-data slot addresses an 'ComboBoxItemData' object
				if (!this->Owner.ownerDraw())
					ComboBox_SetItemData(this->Owner.handle(), this->Index, customUserData);
				else
					this->data<ComboBoxItemData>()->UserData = static_cast<void*>(customUserData);
			}
		};

		class ItemCollection {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			class ItemIterator : public boost::iterator_facade<ItemIterator, ComboBoxItem, boost::random_access_traversal_tag>{
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

				ComboBoxItem
				dereference() const { 
					return ComboBoxItem{this->Owner, this->Index};
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

			std::optional<ComboBoxItem>
			find(std::wstring_view item) const {
				if (signed const idx = ComboBox_FindStringExact(this->Owner.handle(), 0, item.data()); idx == -1)
					return std::nullopt;
				else
					return ComboBoxItem{this->Owner, static_cast<size_t>(idx)};
			}
		
			uint32_t
			height() const {
				Invariant(!this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable));
				return ComboBox_GetItemHeight(this->Owner.handle());
			}

			std::optional<ComboBoxItem>
			selected() const {
				if (signed const idx = ComboBox_GetCurSel(this->Owner.handle()); idx == -1)
					return std::nullopt;
				else 
					return ComboBoxItem{this->Owner, static_cast<size_t>(idx)};
			}
			
			size_t 
			size() const {
				return ComboBox_GetCount(this->Owner.handle());
			}
			
			std::optional<ComboBoxItem>
			substr(std::wstring_view substring) const {
				if (signed const idx = ComboBox_FindStringExact(this->Owner.handle(), 0, substring.data()); idx == -1)
					return std::nullopt;
				else
					return ComboBoxItem{this->Owner, static_cast<size_t>(idx)};
			}

			ComboBoxItem
			operator[](size_t idx) const {
				return ComboBoxItem(this->Owner, idx);
			}

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			void
			height(uint32_t allItems) {
				Invariant(!this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable));
				ComboBox_SetItemHeight(this->Owner.handle(), 0, allItems);
			}
			
			void
			insert(size_t idx, std::wstring_view text) 
			{
				// [NOT OWNER-DRAW] Store (or duplicate) a simple string (according to its 'HasStrings' style)
				if (!this->Owner.ownerDraw()) 
					ComboBox_InsertString(this->Owner.handle(), idx, text.data());

				else {
					auto data = std::make_unique<ComboBoxItemData>(text);
					
					// [HAS-STRINGS] Supplement item with non-visible text for screen-reader support 
					if (this->Owner.hasStrings()) {
						this->Owner.ExposingOwnerDrawItemsBugfix = data.get();
						idx = ComboBox_InsertString(this->Owner.handle(), idx, data->Detail.Text.c_str());
						ComboBox_SetItemData(this->Owner.handle(), idx, data.release());
						this->Owner.ExposingOwnerDrawItemsBugfix = nullptr;
					}
					// [NO-STRING] Only store our owner-draw data
					else
						ComboBox_InsertItemData(this->Owner.handle(), idx, data.release());
				}
			}
			
			void
			insert(size_t                     idx, 
			       ComboBoxItemElement        text,
			       std::optional<forms::Icon> icon = std::nullopt) 
			{
				auto data = std::make_unique<ComboBoxItemData>(text, std::nullopt, icon);
				
				// [HAS-STRINGS] Supplement item with non-visible text for screen-reader support 
				if (this->Owner.hasStrings()) {
					this->Owner.ExposingOwnerDrawItemsBugfix = data.get();
					idx = ComboBox_InsertString(this->Owner.handle(), idx, data->Detail.Text.c_str());
					ComboBox_SetItemData(this->Owner.handle(), idx, data.release());
					this->Owner.ExposingOwnerDrawItemsBugfix = nullptr;
				}
				// [NO-STRING] Only store our owner-draw data
				else
					ComboBox_InsertItemData(this->Owner.handle(), idx, data.release());
			}
			
			void
			insert(size_t              idx, 
			       std::wstring_view   text, 
			       std::wstring_view   heading, 
			       std::optional<Icon> icon = std::nullopt)
			{
				Invariant(this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable));
				this->insert(idx, ComboBoxItemElement{text}, ComboBoxItemElement{heading}, icon);
			}
			
			void
			insert(size_t                     idx, 
			       ComboBoxItemElement        text,
			       ComboBoxItemElement        heading,
			       std::optional<forms::Icon> icon = std::nullopt) 
			{
				Invariant(this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable));
				auto data = std::make_unique<ComboBoxItemData>(text, heading, icon);
				
				// [HAS-STRINGS] Supplement item with non-visible text for screen-reader support 
				if (this->Owner.hasStrings()) {
					this->Owner.ExposingOwnerDrawItemsBugfix = data.get();
					idx = ComboBox_InsertString(this->Owner.handle(), idx, data->Heading->Text.c_str());
					ComboBox_SetItemData(this->Owner.handle(), idx, data.release());
					this->Owner.ExposingOwnerDrawItemsBugfix = nullptr;
				}
				// [NO-STRING] Only store our owner-draw data
				else
					ComboBox_InsertItemData(this->Owner.handle(), idx, data.release());
			}
			
			void
			push_back(std::wstring_view text) {
				this->insert(static_cast<size_t>(-1), text);
			}

			void
			push_back(ComboBoxItemElement        text,
			          std::optional<forms::Icon> icon = std::nullopt) {
				this->insert(static_cast<size_t>(-1), text, icon);
			}
			
			void
			push_back(std::wstring_view   text, 
			          std::wstring_view   heading, 
			          std::optional<Icon> icon = std::nullopt) 
			{
				Invariant(this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable));
				this->insert(static_cast<size_t>(-1), text, heading, icon);
			}
			
			void
			push_back(ComboBoxItemElement        text,
			          ComboBoxItemElement        heading,
			          std::optional<forms::Icon> icon = std::nullopt) 
			{
				Invariant(this->Owner.style<ComboBoxStyle>().test(ComboBoxStyle::OwnerDrawVariable));
				this->insert(static_cast<size_t>(-1), text, heading, icon);
			}

			void
			select(ComboBoxItem const& item) {
				this->select(item.index());
			}

			void
			select(size_t idx) {
				ComboBox_SetCurSel(this->Owner.handle(), idx);
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
		std::optional<forms::Font>   EditFont;
		std::optional<forms::Font>   TitleFont;

		//! @bug  When 'HasStrings' and 'OwnerDraw' are both active and items are added, supplementary
		//!       item text should be provided for screen-readers; however, the API only permits item
		//!       data to be added retrospectively, so text must added first. This triggers a design
		//!       flaw whereby WM_MEASUREITEM is sent prior to item data being added. This fix is the 
		//!       suggested workaround.
		//! @see https://learn.microsoft.com/en-us/windows/win32/winauto/exposing-owner-drawn-combo-box-items
		ComboBoxItemData* ExposingOwnerDrawItemsBugfix; 

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
		dropped() const {
			return ComboBox_GetDroppedState(this->handle()) != FALSE;
		}

		Rect
		droppedRect() const {
			Rect rc;
			ComboBox_GetDroppedControlRect(this->handle(), rc);
			return rc;
		}
		
		forms::Font
		editFont() {
			return this->EditFont.value_or(this->font());
		}
		
		bool
		hasStrings() const {
			return this->style<ComboBoxStyle>().test(ComboBoxStyle::HasStrings);
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
		
		std::optional<forms::Font>
		titleFont() const {
			return this->TitleFont;
		}
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		void
		editFont(const forms::Font& newFont) {
			Invariant(!this->exists());
			this->EditFont = newFont;
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
	
		//! TODO: Delete item data
		Response
		virtual onDeleteItem(void* args) = delete;
		
		Response 
		virtual onMeasureItem(MeasureItemEventArgs args) override {
			if (args.Ident == this->ident()) {
				args.Item.UserData = (uintptr_t)this->ExposingOwnerDrawItemsBugfix;
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
		
		void
		titleFont(const forms::Font& newFont) {
			this->TitleFont = newFont;
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

		/*Response
		virtual onOfferNotification(::UINT notification) override {
			switch (notification) {
			case CBN_SELENDOK:
				if (auto selectedItem = this->Items.selected(); selectedItem)
					this->Items.select(*selectedItem);
				return 0;
			}
			return Unhandled;
		}*/

		::LRESULT 
		virtual onRouteUnhandled(::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
			return this->subclassedWndProc(message, wParam, lParam);
		}

	private:
		void
		this_OnCreated(Window& [[maybe_unused]] sender, CreateWindowEventArgs [[maybe_unused]] args) {
			auto const info = this->info();
			this->DroppedItemsList = info.DroppedItemList;
			this->SelectedItemEdit = info.SelectedItemEdit;
		}
	};
}	// namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o