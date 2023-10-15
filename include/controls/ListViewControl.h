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
#include "library/core.Forms.h"
#include "controls/Control.h"
#include "controls/ListBoxItemData.h"
#include "controls/ListViewStyle.h"
#include "controls/ListViewExStyle.h"
#include "forms/WindowClass.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Header Files o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	enum class ListViewFeature : uint32_t { 
		None, 
	};
}
namespace core::meta 
{
	metadata bool Settings<bitwise_enum, forms::ListViewFeature> = true;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	class ListViewControl : public Control 
	{
		using base = Control;
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	protected:
		class ListViewNotificationDictionary : public forms::MessageDictionary {
			using base = forms::MessageDictionary;
		public:
			ListViewNotificationDictionary() : base({
	#define MakeMessageName(msg)  { msg, #msg }
				MakeMessageName(LVN_ITEMCHANGING),
				MakeMessageName(LVN_ITEMCHANGED),
				MakeMessageName(LVN_INSERTITEM),
				MakeMessageName(LVN_DELETEITEM),
				MakeMessageName(LVN_DELETEALLITEMS),
				MakeMessageName(LVN_BEGINLABELEDIT),
				MakeMessageName(LVN_ENDLABELEDIT),
				MakeMessageName(LVN_COLUMNCLICK),
				MakeMessageName(LVN_BEGINDRAG),
				MakeMessageName(LVN_BEGINRDRAG),
				MakeMessageName(LVN_ODCACHEHINT),
				MakeMessageName(LVN_ODFINDITEM),
				MakeMessageName(LVN_ITEMACTIVATE),
				MakeMessageName(LVN_ODSTATECHANGED),
				MakeMessageName(LVN_ODFINDITEM),
				MakeMessageName(LVN_HOTTRACK),
				MakeMessageName(LVN_GETDISPINFO),
				MakeMessageName(LVN_SETDISPINFO),
				MakeMessageName(LVN_BEGINLABELEDIT),
				MakeMessageName(LVN_ENDLABELEDIT),
				MakeMessageName(LVN_GETDISPINFO),
				MakeMessageName(LVN_SETDISPINFO),
				MakeMessageName(LVN_KEYDOWN),
				MakeMessageName(LVN_MARQUEEBEGIN),
				MakeMessageName(LVN_GETINFOTIP),
				MakeMessageName(LVN_INCREMENTALSEARCH),
				MakeMessageName(LVN_COLUMNDROPDOWN),
				MakeMessageName(LVN_COLUMNOVERFLOWCLICK),
				MakeMessageName(LVN_BEGINSCROLL),
				MakeMessageName(LVN_ENDSCROLL),
				MakeMessageName(LVN_LINKCLICK),
				MakeMessageName(LVN_GETEMPTYMARKUP)
	#undef MakeMessageName
				})
			{}
		};
		
		class ListViewWindowClass : public forms::WindowClass {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			using const_reference = ListViewWindowClass const&;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			::WNDPROC	OriginalWndProc;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			ListViewWindowClass() : forms::WindowClass{win::ResourceId{WC_LISTVIEW}}  {
				this->Name = win::ResourceId{L"Custom.LISTVIEW"};
				this->OriginalWndProc = std::exchange(this->WndProc, Window::defaultMessageHandler);
				this->Style |= ClassStyle::GlobalClass;
				this->registér();
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		};

		//! @brief	Custom data used for each column when in owner-draw mode
		struct ColumnData {
			// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
			RichText                Detail;
			std::optional<Icon>     Icon;
			void*                   UserData = nullptr;
			// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
			explicit
			ColumnData(std::wstring_view           text, 
			           std::optional<forms::Icon>  icon = nullopt) 
			  : Detail{text}, 
				Icon{icon}
			{}
			
			explicit
			ColumnData(RichText                    text, 
			           std::optional<forms::Icon>  icon = nullopt) 
			  : Detail{text}, 
				Icon{icon}
			{}
			// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
			satisfies(ColumnData,
				IsSemiRegular,
				NotEqualityComparable,
				NotSortable
			);
		};

		//! @brief	Facade for a single column at a fixed index
		class Column {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			ListViewControl*  Owner;
			int32_t           Index;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			Column(ListViewControl& owner, int32_t idx) noexcept
			  : Owner{&owner}, 
			    Index{idx}
			{}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
			satisfies(Column, 
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
				return this->data<ColumnData>()->Detail;
			}

			std::optional<Icon>
			icon() const {
				Invariant(this->Owner->ownerDraw());
				return this->data<ColumnData>()->Icon;
			}

			int32_t
			index() const noexcept {
				return this->Index;
			}
			
			std::wstring
			text() const {
				if (this->Owner->ownerDraw())
					return this->data<ColumnData>()->Detail.Text;

				wchar_t buffer[64] {};
				::LVCOLUMNW col { .mask = LVCF_TEXT, .pszText = &buffer[0], .cchTextMax = 64 };
				if (!ListView_GetColumn(this->Owner->handle(), this->Index, &col))
					throw runtime_error{"ListView_GetColumn(#{}) failed", this->Index};
				else 
					return col.pszText;
			}
			
			template <nstd::Class UserData>
			UserData*
			userData() const {
				// When owner-draw is active, the item-data slot addresses an 'ColumnData' object
				if (!this->Owner->ownerDraw())
					return this->data<UserData>();
				else
					return static_cast<UserData*>(this->data<ColumnData>()->UserData);
			}
			
			uint32_t
			width() const noexcept {
				return ListView_GetColumnWidth(this->Owner->handle(), this->Index);
			}
			
		protected:
			template <nstd::Class AnyType>
			AnyType*
			data() const {
				throw runtime_error{"Headers don't provide user-data :-O"};
				/*::LVCOLUMNW col { .mask = LVCF_USER };
				if (!ListView_GetColumn(this->Owner->handle(), this->Index, &col))
					throw runtime_error{"ListView_GetColumn(#{}) failed", this->Index};
				else 
					return reinterpret_cast<AnyType*>(col.itemData);*/
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			void
			width(uint32_t individualColumn) {
				ListView_SetColumnWidth(this->Owner->handle(), this->Index, individualColumn);
			}

			template <typename AnyType>
			void
			userData(AnyType* customUserData) {
				throw runtime_error{"Headers don't provide user-data :-O"};
				// When owner-draw is active, the item-data slot addresses an 'ColumnData' object
				/*if (!this->Owner->ownerDraw())
					ListView_SetColumnData(this->Owner->handle(), this->Index, customUserData);
				else
					this->data<ColumnData>()->UserData = static_cast<void*>(customUserData);*/
			}
		};
		
		class ColumnCollection {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			template <nstd::AnyOf<Column,Column const> ValueType>
			class Iterator : public boost::iterator_facade<Iterator<ValueType>, ValueType, boost::random_access_traversal_tag, ValueType&, int32_t>
			{
				template <nstd::AnyOf<Column,Column const>>
				friend class Iterator;
				// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
			private:
				using type = Iterator<ValueType>;
				// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
			private:
				ListViewControl* Owner;
				int32_t          Index;
				// o~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~o
			public:
				Iterator(ListViewControl& owner, int32_t initialIdx) noexcept
				  : Owner{&owner}, 
				    Index{initialIdx}
				{}

				explicit
				Iterator(ListViewControl& owner) noexcept
				  : Owner{&owner}, 
				    Index{owner.Columns.size()}
				{}
				
				template <nstd::AnyOf<Column const> Other>
					requires std::same_as<ValueType,Column>
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
				template <nstd::AnyOf<Column,Column const> Other>
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
			
			using iterator = Iterator<Column>;
			using const_iterator = Iterator<Column const>;
			using reference = Column&;
			using const_reference = Column const&;
			using value_type = Column;
			using size_type = iterator::difference_type;
			using difference_type = iterator::difference_type;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			ListViewControl& Owner;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			explicit
			ColumnCollection(ListViewControl& ctrl) noexcept
			  : Owner{ctrl}
			{}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
			satisfies(ColumnCollection,
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

			size_type 
			size() const noexcept {
				return Header_GetItemCount(ListView_GetHeader(this->Owner.handle()));
			}
			
			Column
			operator[](size_type idx) const noexcept {
				return Column(this->Owner, idx);
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
				for (signed count = this->size(), idx = count-1; idx >= 0; --idx)
					ListView_DeleteColumn(this->Owner.handle(), idx);
			}
			
			iterator
			insert(const_iterator pos, std::wstring_view text, std::optional<int32_t> width = nullopt) 
			{
				size_type idx{};
				// [NOT OWNER-DRAW] Add Column
				if (!this->Owner.ownerDraw()) {
					::LVCOLUMNW col { 
						.mask = LVCF_FMT|LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM,
						.fmt = LVCFMT_LEFT,
						.cx = width.value_or(LVSCW_AUTOSIZE),
						.pszText = const_cast<wchar_t*>(text.data()),
						.iSubItem = pos,
					};
					if (idx = ListView_InsertColumn(this->Owner.handle(), pos, &col); idx == LB_ERR)
						throw runtime_error{"ListView_InsertColumn(#{}) failed", (int32_t)pos};
				}
				// [OWNER-DRAW] TODO
				else {
					throw runtime_error{"Not implemented"};
				}
				return iterator{this->Owner, idx};
			}

			void
			push_back(std::wstring_view text, std::optional<int32_t> width = nullopt) {
				this->insert(const_iterator{this->Owner,this->size()}, text, width);
			}
		};
	
		//! @brief	Custom data used for each item when in owner-draw mode
		class ItemData {
			// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
		private:
			void*  UserData = nullptr;

		public:
			RichText            Detail;
			std::optional<Icon> Icon;
			// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
		public:
			explicit
			ItemData(std::wstring_view           text, 
			         std::optional<forms::Icon>  icon = nullopt) 
			  : Detail{text}, 
				Icon{icon}
			{}
			
			explicit
			ItemData(RichText                    text, 
			         std::optional<forms::Icon>  icon = nullopt) 
			  : Detail{text}, 
				Icon{icon}
			{}
			// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
			satisfies(ItemData,
				IsSemiRegular,
				NotEqualityComparable,
				NotSortable
			);
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			template <nstd::Class UserData>
			UserData*
			userData() const {
				return static_cast<UserData*>(this->data<ItemData>()->UserData);
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			template <nstd::Class UserData>
			void
			userData(UserData* newData) const {
				this->UserData = newData;
			}
		};
		
		//! @brief	Facade for a single item at a fixed index
		class Item {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			ListViewControl*  Owner;
			int32_t           Index;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			Item(ListViewControl& owner, int32_t idx) noexcept
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
			Rect
			area() const noexcept {
				Rect r;
				ListView_GetItemRect(this->Owner->handle(), this->Index, static_cast<::RECT*>(r), LVIR_BOUNDS);
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
			
			std::wstring
			text(size_t maxLength = 256) const {
				if (this->Owner->ownerDraw())
					return this->data<ItemData>()->Detail.Text;

				std::wstring buffer(maxLength, '\0');
				::LVITEMW item {
					.mask = LVIF_TEXT,
					.iItem = this->Index,
					.pszText = buffer.data(),
					.cchTextMax = static_cast<int>(maxLength),
				};
				if (!ListView_GetItem(this->Owner->handle(), &item))
					throw runtime_error{"ListView_GetItem(#{}) failed", this->Index};
				else {
					buffer.resize(item.cchTextMax);
					return buffer;
				}
			}
			
			template <nstd::Class CustomUserData>
			CustomUserData*
			userData() const {
				// When owner-draw is active, the item-data slot is occupied by our 'ItemData' object
				if (this->Owner->ownerDraw())
					return this->data<ItemData>()->userData<CustomUserData>();
				else
					return this->data<CustomUserData>();
			}
			
		protected:
			template <nstd::Class AnyData>
			AnyData*
			data() const {
				::LVITEMW item {
					.mask = LVIF_PARAM,
					.iItem = this->Index,
				};
				if (!ListView_GetItem(this->Owner->handle(), &item))
					throw runtime_error{"ListView_GetItem(#{}) failed", this->Index};
				else 
					return reinterpret_cast<AnyData*>(item.lParam);
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			void
			text(std::wstring newText) {
				if (this->Owner->ownerDraw()) 
					this->data<ItemData>()->Detail.Text = newText;
				else {
					::LVITEMW item {
						.mask = LVIF_TEXT,
						.iItem = this->Index,
						.pszText = newText.data(),
						.cchTextMax = static_cast<int>(newText.length()),		// Docs say not needed: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/ns-commctrl-lvitema
					};
					if (!ListView_SetItem(this->Owner->handle(), &item))
						throw runtime_error{"ListView_SetItem(#{}) failed", this->Index};
				}
			}
			
			template <nstd::Class CustomUserData>
			void
			userData(CustomUserData* customUserData) {
				// When owner-draw is active, the item-data slot is occupied by our 'ItemData' object
				if (this->Owner->ownerDraw())
					this->data<ItemData>()->userData(customUserData);
				else
					ListView_SetItemData(this->Owner->handle(), this->Index, customUserData);
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
				// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
			private:
				using type = Iterator<ValueType>;
				// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
			private:
				ListViewControl* Owner;
				int32_t         Index;
				// o~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~o
			public:
				Iterator(ListViewControl& owner, int32_t initialIdx) noexcept
				  : Owner{&owner}, 
				    Index{initialIdx}
				{}

				explicit
				Iterator(ListViewControl& owner) noexcept
				  : Owner{&owner}, 
				    Index{owner.Items.size()}
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
			ListViewControl& Owner;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			explicit
			ItemCollection(ListViewControl& ctrl) noexcept
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
			find(std::wstring_view item) const /*noexcept*/ {
				throw runtime_error{"Not implemented"}; 
				/*if (size_type const idx = ListView_FindStringExact(this->Owner.handle(), 0, item.data()); idx == CB_ERR)
					return nullopt;
				else
					return Item{this->Owner, idx};*/
			}

			std::optional<Item>
			selected() const /*noexcept*/ {
				throw runtime_error{"Not implemented"}; 
				/*if (size_type const idx = ListView_GetCurSel(this->Owner.handle()); idx == CB_ERR)
					return nullopt;
				else 
					return Item{this->Owner, idx};*/
			}
			
			size_type 
			size() const noexcept {
				return ListView_GetItemCount(this->Owner.handle());
			}
			
			std::optional<Item>
			substr(std::wstring_view substring) const /*noexcept*/ {
				throw runtime_error{"Not implemented"}; 
				/*if (size_type const idx = ListView_FindStringExact(this->Owner.handle(), 0, substring.data()); idx == CB_ERR)
					return nullopt;
				else
					return Item{this->Owner, idx};*/
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
				ListView_DeleteAllItems(this->Owner.handle());
			}
			
			void 
			focus(const_iterator pos) /*noexcept*/ {
				throw runtime_error{"Not implemented"};  //ListView_SetCaretIndex(this->Owner.handle(), pos);
			}

			iterator
			insert(const_iterator pos, std::wstring_view text) 
			{
				size_type idx{};
				// [NOT OWNER-DRAW] Insert item
				if (!this->Owner.ownerDraw()) {
					::LVITEMW item {
						.mask = LVIF_TEXT,
						.iItem = (int)pos,
						.pszText = const_cast<wchar_t*>(text.data()),
						.cchTextMax = static_cast<int>(text.length()),		// Docs say not needed: https://learn.microsoft.com/en-us/windows/win32/api/commctrl/ns-commctrl-lvitema
					};
					if (idx = ListView_InsertItem(this->Owner.handle(), &item); idx == LB_ERR)
						throw runtime_error{"ListView_InsertItem(#{}) failed", (int)pos};
				}
				// [OWNER-DRAW] Insert parameter data
				else {
					auto data = std::make_unique<ItemData>(text);
					::LVITEMW item {
						.mask = LVIF_PARAM,
						.iItem = (int)pos,
						.lParam = std::bit_cast<::LPARAM>(data.get()),
					};
					if (idx = ListView_InsertItem(this->Owner.handle(), &item); idx == LB_ERR)
						throw runtime_error{"ListView_InsertItem(#{}) failed", (int)pos};
					data.release();
				}
				return iterator{this->Owner, idx};
			}
			
			iterator
			insert(const_iterator             pos, 
			       RichText                   text,
			       std::optional<forms::Icon> icon = nullopt) 
			{
				Invariant(this->Owner.ownerDraw());
				auto data = std::make_unique<ItemData>(text, icon);
				::LVITEMW item {
					.mask = LVIF_PARAM,
					.iItem = (int)pos,
					.lParam = std::bit_cast<::LPARAM>(data.get()),
				};
				if (size_type idx = ListView_InsertItem(this->Owner.handle(), &item); idx == LB_ERR)
					throw runtime_error{"ListView_InsertItem(#{}) failed", (int)pos};
				else {
					data.release();
					return iterator{this->Owner, idx};
				}
			}
			
			void
			push_back(std::wstring_view text) {
				this->insert(const_iterator{this->Owner,INT32_MAX}, text);
			}

			void
			push_back(RichText                   text,
			          std::optional<forms::Icon> icon = nullopt) {
				Invariant(this->Owner.ownerDraw());
				this->insert(const_iterator{this->Owner,INT32_MAX}, text, icon);
			}

			void
			scrollTo(const_iterator pos) /*noexcept*/ {
				throw runtime_error{"Not implemented"};  //ListView_SetTopIndex(this->Owner.handle(), pos);
				//! @see  https://learn.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-listview_ensurevisible
			}
	
			void
			select(Item const& item) /*noexcept*/ {
				throw runtime_error{"Not implemented"};  //ListView_SetCurSel(this->Owner.handle(), item.index());
			}
			
			void
			select(const_iterator pos) /*noexcept*/ {
				throw runtime_error{"Not implemented"};  //ListView_SetCurSel(this->Owner.handle(), pos);
			}
		};
	
	public:
		using WindowClass = ListViewWindowClass;
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		ColumnCollection Columns;
		ItemCollection   Items;
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		implicit
		ListViewControl(uint16_t id) 
		  : Control{id}, 
			Columns{*this},
			Items{*this}
		{
			this->backColour(this->LookNFeel->control());
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		gsl::czstring
		static identifyNotification(::UINT notification) {
			ListViewNotificationDictionary const  static names;
			return names.at(notification);
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		nstd::bitset<ListViewFeature>
		features() const noexcept {
			return base::features<ListViewFeature>();
		}
	
		bool
		virtual ownerDraw() const noexcept override {
			return this->style<ListViewStyle>().test(ListViewStyle::OwnerDrawFixed);
		}
		
		WindowRole
		virtual role() const noexcept override {
			return WindowRole::List;
		}

		ListViewWindowClass::const_reference
		virtual wndcls() const override {
			ListViewWindowClass const  static wc;
			return wc;
		}
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		void
		features(nstd::bitset<ListViewFeature> newStyle) noexcept {
			base::features(newStyle);
		}
	
	protected:
		gsl::czstring
		virtual notificationName(::UINT notification) override {
			return ListViewControl::identifyNotification(notification);
		}
		
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

		/*Response
		virtual onOfferNotification(::UINT notification) override {
			switch (notification) {
			case BN_CLICKED:
				this->Clicked.raise();
				return 0;
			}
			return Unhandled;
		}*/
		
		::LRESULT 
		virtual onRouteUnhandled(::UINT message, ::WPARAM wParam, ::LPARAM lParam) override {
			return this->subclassedWndProc(message, wParam, lParam);
		}
	};
}	// namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o