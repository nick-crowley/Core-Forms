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
#include "controls/CountingIterator.h"
#include "controls/RichText.h"
#include "graphics/Icon.h"
#include "system/SharedHandle.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	enum class MenuItemState : uint32_t {
		None        = NULL,           //!< Enabled, un-checked, and un-highlighted
		Checked     = MFS_CHECKED,    //!< Checks the menu item.
		Default     = MFS_DEFAULT,    //!< Specifies that the menu item is the default.
		Disabled    = MFS_DISABLED,   //!< Disables the menu item and grays it so that it cannot be selected.
		Highlighted = MFS_HILITE,     //!< Highlights the menu item.
	};
}
namespace core::meta 
{
	metadata bool Settings<bitwise_enum, forms::MenuItemState> = true;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms 
{
	class Menu
	{
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		//! @brief	Custom item data used for each element when in owner-draw mode
		struct ItemData {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
			RichText                Detail;
			std::optional<Icon>     Icon;
			void*                   UserData = nullptr;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
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
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
			satisfies(ItemData,
				IsSemiRegular,
				NotEqualityComparable,
				NotSortable
			);
		};

	protected:
		//! @brief  Distinguishes item ID type from similar index type
		enum class ItemId : uint16_t {};

		//! @brief	Facade for a single item identified either by zero-based index or unique identifier
		class Item {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
			using IdentVariant = std::variant<ItemId, int32_t>;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			Menu*        Owner;
			IdentVariant Ident;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			Item(Menu& owner, ItemId id) noexcept
			  : Owner{&owner}, 
			    Ident{id}
			{}
			
			Item(Menu& owner, int32_t idx) noexcept
			  : Owner{&owner}, 
			    Ident{idx}
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
		private:
			std::string
			static to_string(IdentVariant v) {
				if (std::holds_alternative<int32_t>(v))
					return std::format("#{}", std::get<int32_t>(v));
				return std::format("id={}", std::to_underlying(std::get<ItemId>(v)));
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			Rect
			area() const {
				Invariant(this->hasIndex());
				Rect r;
				if (!::GetMenuItemRect(win::Unused<::HWND>, *this->Owner->handle(), this->index(), static_cast<::RECT*>(r)))
					win::LastError{}.throwAlways("GetMenuItemRect({}) failed", to_string(this->Ident));
				return r;
			}
			
			RichText
			detail() const {
				Invariant(this->ownerDraw());
				return this->data<ItemData>()->Detail;
			}

			std::optional<Icon>
			icon() const {
				Invariant(this->ownerDraw());
				return this->data<ItemData>()->Icon;
			}
			
			uint16_t
			id() const {
				if (!std::holds_alternative<ItemId>(this->Ident))
					return win::Word{::GetMenuItemID(*this->Owner->handle(), this->index())};
				else
					return std::to_underlying(std::get<ItemId>(this->Ident));
			}

			int32_t
			index() const {
				Invariant(std::holds_alternative<int32_t>(this->Ident));
				return std::get<int32_t>(this->Ident);
			}
			
			bool
			ownerDraw() const {
				::MENUITEMINFO info{sizeof(info), MIIM_FTYPE};
				if (!::GetMenuItemInfoW(*this->Owner->handle(), this->ident(), win::Bool{this->Ident.index()}, &info))
					win::LastError{}.throwAlways("GetMenuItemType(#{}) failed", to_string(this->Ident));
				else 
					return (info.fType & MFT_OWNERDRAW) != 0;
			}

			bool
			separator() const {
				::MENUITEMINFO info{sizeof(info), MIIM_FTYPE};
				if (!::GetMenuItemInfoW(*this->Owner->handle(), this->ident(), win::Bool{this->Ident.index()}, &info))
					win::LastError{}.throwAlways("GetMenuItemType(#{}) failed", to_string(this->Ident));
				else 
					return (info.fType & MFT_SEPARATOR) != 0;
			}

			nstd::bitset<MenuItemState>
			state() const {
				::MENUITEMINFO info{sizeof(info), MIIM_STATE};
				if (!::GetMenuItemInfoW(*this->Owner->handle(), this->ident(), win::Bool{this->Ident.index()}, &info))
					win::LastError{}.throwAlways("GetMenuItemState(#{}) failed", to_string(this->Ident));
				else 
					return static_cast<MenuItemState>(info.fState);
			}

			std::optional<Menu>
			submenu() const {
				Invariant(this->hasIndex());
				if (::HMENU const sm = ::GetSubMenu(*this->Owner->handle(), this->index()); !sm)
					return nullopt;
				else
					return Menu{SharedMenu{sm, weakref}};
			}
			
			std::wstring
			text() const {
				if (this->ownerDraw())
					return this->data<ItemData>()->Detail.Text;
				
				::MENUITEMINFO info{sizeof(info), MIIM_STRING};
				wchar_t buffer[64] {};
				info.dwTypeData = buffer;
				info.cch = lengthof(buffer);
				if (!::GetMenuItemInfoW(*this->Owner->handle(), this->ident(), win::Bool{this->Ident.index()}, &info)) 
					win::LastError{}.throwAlways("GetMenuItemString(#{}) failed", to_string(this->Ident));
				return buffer;
			}
			
			template <nstd::Class UserData>
			UserData*
			userData() const {
				// When owner-draw is active, the item-data slot addresses an 'ItemData' object
				if (!this->ownerDraw())
					return this->data<UserData>();
				else
					return static_cast<UserData*>(this->data<ItemData>()->UserData);
			}
			
		protected:
			template <nstd::Class AnyType>
			AnyType*
			data() const {
				::MENUITEMINFO info{sizeof(info), MIIM_DATA};
				if (!::GetMenuItemInfoW(*this->Owner->handle(), this->ident(), win::Bool{this->Ident.index()}, &info))
					win::LastError{}.throwAlways("GetMenuItemData(#{}) failed", to_string(this->Ident));
				else 
					return reinterpret_cast<AnyType*>(info.dwItemData);
			}

			uint16_t
			ident() const {
				return std::holds_alternative<ItemId>(this->Ident) ? this->id() : win::Word{this->index()};
			}

			bool
			hasIndex() const {
				return std::holds_alternative<int32_t>(this->Ident);
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			void
			ownerDraw(bool newState) {
				if (bool const oldState = this->ownerDraw(); oldState == newState)
					return;
				else if (newState) {
					auto data = std::make_unique<ItemData>(this->text());
					::MENUITEMINFO info{sizeof(info), MIIM_DATA|MIIM_FTYPE};
					info.dwItemData = reinterpret_cast<uintptr_t>(data.get());
					info.fType = MFT_OWNERDRAW;
					if (!::SetMenuItemInfoW(*this->Owner->handle(), this->ident(), win::Bool{this->Ident.index()}, &info))
						win::LastError{}.throwAlways("SetMenuItemType(#{}) failed", to_string(this->Ident));
					data.release();
				}
				else
					ThrowInvalidArg(newState, "Disabling owner-draw not implemented");
			}
			
			void
			state(MenuItemState newState) {
				::MENUITEMINFO info{sizeof(info), MIIM_STATE};
				info.fState = std::to_underlying(newState);
				if (!::GetMenuItemInfoW(*this->Owner->handle(), this->ident(), win::Bool{this->Ident.index()}, &info))
					win::LastError{}.throwAlways("GetMenuItemState(#{}, {}) failed", to_string(this->Ident), core::to_string(newState));
			}
			
			void
			text(std::wstring_view wstr) {
				if (this->ownerDraw())
					this->data<ItemData>()->Detail.Text = wstr;
				else {
					::MENUITEMINFO info{sizeof(info), MIIM_STRING};
					info.dwTypeData = const_cast<wchar_t*>(wstr.data());
					if (!::SetMenuItemInfoW(*this->Owner->handle(), this->ident(), win::Bool{this->Ident.index()}, &info)) 
						win::LastError{}.throwAlways("SetMenuItemText(#{}, '{}') failed", to_string(this->Ident), cnarrow(wstr));
				}
			}
			
			template <nstd::Class AnyType>
			void
			userData(AnyType* customUserData) {
				// When owner-draw is active, the item-data slot addresses an 'ItemData' object
				if (this->Owner->ownerDraw()) 
					this->data<ItemData>()->UserData = static_cast<void*>(customUserData);
				else {
					::MENUITEMINFO info{sizeof(info), MIIM_DATA};
					info.dwItemData = reinterpret_cast<uintptr_t>(customUserData);
					if (!::SetMenuItemInfoW(*this->Owner->handle(), this->ident(), win::Bool{this->Ident.index()}, &info))
						win::LastError{}.throwAlways("SetMenuItemData(#{}) failed", to_string(this->Ident));
				}
			}
		};
		
		class ItemCollection {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			using iterator = boost::transform_iterator<std::function<Item(int32_t)>, CountingIterator>;
			using const_iterator = boost::transform_iterator<std::function<Item const(int32_t)>, CountingIterator>;
			using reference = Item&;
			using const_reference = Item const&;
			using value_type = Item;
			using size_type = iterator::difference_type;
			using difference_type = iterator::difference_type;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			Menu&  Owner;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			explicit
			ItemCollection(Menu& menu) noexcept
			  : Owner{menu}
			{}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
			satisfies(ItemCollection,
				NotDefaultConstructible,
				IsCopyConstructible noexcept, 
				IsMoveConstructible noexcept,
				NotCopyAssignable, 
				NotMoveAssignable,
				NotSortable,
				NotEqualityComparable
			);
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			Item
			at(size_type idx) const noexcept {
				return Item{this->Owner, idx};
			}

			Item
			back() const noexcept {
				return Item{this->Owner, this->size() - 1};
			}

			const_iterator
			begin() const noexcept {
				return this->cbegin();
			}
		
			const_iterator
			end() const noexcept {
				return this->cend();
			}
			
			const_iterator
			cbegin() const noexcept {
				return this->make_iterator<const_iterator>(0);
			}
		
			const_iterator
			cend() const noexcept {
				return make_iterator<const_iterator>(this->size());
			}
			
			Item
			front() const noexcept {
				return Item{this->Owner, 0};
			}

			size_type 
			size() const noexcept {
				return ::GetMenuItemCount(*this->Owner.handle());
			}
			
			Item
			operator[](uint16_t id) const noexcept {
				return Item{this->Owner, static_cast<ItemId>(id)};
			}

		private:
			template <nstd::AnyOf<iterator,const_iterator> AnyIterator>
			AnyIterator
			make_iterator(int32_t idx) const {
				return AnyIterator{
					CountingIterator{&this->Owner, idx},
					[this](int32_t n) { 
						return Item{this->Owner, n}; 
					}
				};
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			iterator
			begin() noexcept {
				return make_iterator<iterator>(0);
			}
		
			iterator
			end() noexcept {
				return make_iterator<iterator>(this->size());
			}
	
			iterator
			insert(const_iterator pos, std::wstring_view text) 
			{
				Item{this->Owner, pos->index()}.text(text);
				return this->make_iterator<iterator>(pos->index());
			}
			
			iterator
			insert(const_iterator             pos, 
			       RichText                   text,
			       std::optional<forms::Icon> icon = nullopt) 
			{
				// [OWNER-DRAW] Create item data
				auto data = std::make_unique<ItemData>(text, icon);
				::MENUITEMINFO info{sizeof(info), MIIM_DATA};
				info.dwItemData = reinterpret_cast<uintptr_t>(data.get());
				if (!::SetMenuItemInfoW(*this->Owner.handle(), pos->index(), TRUE, &info))
					win::LastError{}.throwAlways("SetMenuItemData(#{}, '{}') failed", pos->index(), cnarrow(text.Text));
				data.release();
				return this->make_iterator<iterator>(pos->index());
			}
			
			void
			push_back(std::wstring_view text) {
				this->insert(this->cend(), text);
			}

			void
			push_back(RichText                   text,
			          std::optional<forms::Icon> icon = nullopt) {
				this->insert(this->cend(), text, icon);
			}

			void
			ownerDraw(bool newState) {
				for (Item item : *this) {
					item.ownerDraw(newState);
					if (auto submenu = item.submenu(); submenu) 
						for (Item subitem : submenu->Items) 
							subitem.ownerDraw(newState);
				}
			}
		};
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		SharedMenu  Handle;

	public:
		ItemCollection  Items;
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		explicit
		Menu(SharedMenu existingMenu)
		  : Handle{std::move(ThrowIfEmpty(existingMenu))},
		    Items{*this}
		{}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
		satisfies(Menu,
			NotCopyable,
			NotMoveAssignable,
			NotEqualityComparable,
			NotSortable
		);

		Menu(Menu&& r) noexcept
		  : Menu{std::move(r.Handle)}
		{}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		SharedMenu
		handle() const {
			return this->Handle;
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	};
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o