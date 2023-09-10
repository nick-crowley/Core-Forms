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
#include "core/DebugStream.h"
#include "core/FunctionLogging.h"
#include "com/SharedPtr.h"
#include "graphics/Graphics.h"
#include "lookNfeel/ILookNFeelProvider.h"
#include "support/ObservableEvent.h"
#include "forms/Accessible.h"
#include "forms/BuiltInWindowMessages.h"
#include "forms/WindowClass.h"
#include "forms/WindowEventArgs.h"
#include "forms/WindowInfo.h"
#include "forms/WindowStyle.h"
#include "win/Boolean.h"
#pragma comment (lib, "OleAcc.lib")
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	//! @brief	Manages the life-cycle and behaviour of a single window
	class FormsExport Window 
	{
		friend class DialogTemplate;	//!< Requires @c CreationData class

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		//! @brief	Dictionary of message names, expected return values, and special logging requirements
		class FormsExport MessageDictionary {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			struct MessageProperties 
			{
				// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
				uint16_t      Ident;
				gsl::czstring Name;
				::LRESULT     Unhandled = 0xffffffff;
				bool          Common = false;

				// o~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~o
				constexpr
				MessageProperties(uint16_t id, gsl::czstring n) : Ident{id}, Name{n}
				{}
			};

			//! @brief	Dictionary storage
			using MessagePropertyArray = std::array<MessageProperties, detail::MessageNames.size()>;
			
		public:
			using key_type = uint16_t;
			using mapped_type = MessageProperties;
			using value_type = meta::undefined_t;
			using reference = MessageProperties&;
			using const_reference = MessageProperties const&;
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			MessagePropertyArray  Entries;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			constexpr
			MessageDictionary() 
				: MessageDictionary(std::make_index_sequence<detail::MessageNames.size()>{})
			{}
			
		private:
			template <size_t... Idx>
			constexpr
			MessageDictionary(std::index_sequence<Idx...>) 
				: Entries({ MessageProperties{Idx, detail::MessageNames[Idx]}... })
			{	
				// Special cases
				this->Entries[WM_NCCREATE].Unhandled = FALSE;

				// Common messages
				this->Entries[WM_NCHITTEST].Common = true;
				this->Entries[WM_NCMOUSEMOVE].Common = true;
				this->Entries[WM_MOUSEFIRST].Common = true;
				this->Entries[WM_SETCURSOR].Common = true;
				this->Entries[WM_MOVE].Common = true;
				this->Entries[WM_MOVING].Common = true;
				this->Entries[WM_WINDOWPOSCHANGED].Common = true;
				this->Entries[WM_WINDOWPOSCHANGING].Common = true;
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			bool constexpr
			contains(::UINT id) const {
				return id < this->Entries.size();
			}
	
			gsl::czstring constexpr
			name(::UINT id) const {
				return this->contains(id) ? (*this)[id].Name : "WM_????";
			}

			const_reference constexpr
			operator[](::UINT id) const {
				return this->Entries.at(static_cast<uint16_t>(id));
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		};

	protected:
		//! @brief	Extends MSAA implementation to provide this window's _accessibility role_
		class FormsExport Accessible : public AccessibleDecorator {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			using base = AccessibleDecorator;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			Window&  Window;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			Accessible(forms::Window& wnd, com::shared_ptr<::IAccessible> impl) 
			  : base{std::move(impl)}, 
			    Window{wnd}
			{}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			satisfies(Accessible,
				NotCopyable,
				NotEqualityComparable,
				NotSortable
			);
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			::HRESULT
			COMAPI get_accRole(::VARIANT child, com::out_t<::VARIANT> role) override {
				
				if (!this->Window.Children.contains(child.iVal))
					return E_INVALIDARG;

				role->vt = VT_I4;
				role->lVal = static_cast<long>(this->Window.Children[child.iVal].role());
				return S_OK;
			}
		};

#		pragma pack (push, 1)
		//! @brief	Represents custom data provided at Window construction
		//! @remarks	Modifying layout will break ABI compatibility
		class FormsExport CreationData {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			uint16_t   Length = 0;
			uint16_t   DuplicateLength = 0;		// Fix: Required for ABI compatibility with DialogBoxIndirectParamW()
			Window*    Parameter = nullptr;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			explicit 
			CreationData(Window* w) 
			  : Length{sizeof(Window*)}, 
			    DuplicateLength{Length},
				Parameter{w}
			{}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			satisfies(CreationData,
				IsRegular noexcept,
				NotSortable
			);
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			std::vector<std::byte> 
			asBytes() const {
				Expects(this->Length <= sizeof(Window*));
				auto const r = std::as_bytes(std::span{&this->Parameter,1});
				return {r.begin(), r.begin() + this->Length};
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			Window* 
			get() {
				return this->Parameter;
			}
		};
		static_assert(sizeof(CreationData) == nstd::sizeof_v<Window*> + nstd::sizeof_n<uint16_t>(2));
#		pragma pack (pop)
		
		//! @brief	Window construction parameters
		struct FormsExport CreateWindowParams
		{
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
			
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
			Rect Area {CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT };
			win::ResourceId Class;
			std::wstring_view Text;
			WindowStyle Style = WindowStyle::None;
			::HWND Parent = nullptr;
			::HMENU Menu = nullptr;
			::HMODULE Module = nullptr;
			CreationData Parameter;

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
			satisfies(CreateWindowParams,
				IsRegular,
				NotSortable
			);
		};

		//! @brief	Simplifies providing multiple window construction parameters
		class FormsExport CreateWindowParamsBuilder 
		{
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
			using reference = CreateWindowParamsBuilder&;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			CreateWindowParams Params;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			satisfies(CreateWindowParamsBuilder,
				IsRegular,
				NotSortable
			);
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			CreateWindowParams&
			build() {
				return this->Params;
			}

			reference
			withArea(Rect const& area) {
				this->Params.Area = area;
				return *this;
			}

			reference
			withClass(WindowClass const& cls) {
				this->Params.Class = cls.Name;
				this->Params.Module = cls.Instance;
				return *this;
			}

			reference
			withParameter(Window* wnd) {
				this->Params.Parameter = CreationData{wnd};
				return *this;
			}

			reference
			withParent(Window const& parent) {
				this->Params.Parent = parent.handle();
				return *this;
			}

			reference
			withStyle(WindowStyle s) {
				this->Params.Style = s;
				return *this;
			}

			reference
			withText(std::wstring_view txt) {
				this->Params.Text = txt;
				return *this;
			}
		};
	
		//! @brief	Identifies current high-level processing loop
		enum class ProcessingState { 
			NotApplicable,           //!< Window does not exist
			BeingCreated,            //!< Creation initiated; not yet complete
			BeingDestroyed,          //!< Destruction initiated; not yet complete
			Idle,                    //!< Exists; doing nothing
			DefaultProcessing,       //!< Default (system) processing for unhandled message
			MessageProcessing,       //!< Processing a message
			EventProcessing,         //!< Dispatching post-message event to attached listeners
			NotificationProcessing   //!< Reflecting notification message back to sender
		};

		//! @brief	Provides useful details for display within the debugger (Eg. window text, window class, and message-handler)
		class FormsExport DebuggingAide 
		{
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			class FormsExport StateDescription {
			public:
				ProcessingState State;
				gsl::czstring   Message = "";

			public:
				implicit
				StateDescription(ProcessingState s) 
				  : State(s)
				{}

				StateDescription(ProcessingState s, gsl::czstring const msg) 
				  : State(s), Message(msg) 
				{}

			public:
				satisfies(StateDescription,
					IsRegular noexcept,
					NotSortable
				);
			};
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			uint32_t const       MagicNumber = 12345678;
			std::array<char,16>  Class {};
			StateDescription     State {ProcessingState::NotApplicable};
			std::array<char,16>	 Text  {};
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			satisfies(DebuggingAide,
				IsDefaultConstructible noexcept,
				IsCopyConstructible noexcept,
				NotCopyAssignable,
				IsEqualityComparable,
				NotSortable
			);
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			StateDescription
			setState(StateDescription newstate) {
				return std::exchange(this->State, newstate);
			}
		
			StateDescription
			setState(StateDescription state, std::wstring_view cls, std::wstring_view txt) {
				// Copy short preview; assume text is latin charset
				*ranges::transform(views::take(cls,lengthof(this->Class)-1), this->Class.begin(), nstd::convert_to<char>).out = '\0';
				*ranges::transform(views::take(txt,lengthof(this->Text)-1), this->Text.begin(), nstd::convert_to<char>).out = '\0';
				return std::exchange(this->State, state);
			}
		
			auto
			setTemporaryState(StateDescription newState) {
				return gsl::finally([this, prev = this->setState(newState)]{ 
					this->State = prev; 
				});
			}
		};
		
		//! @brief	Collection of core-forms windows
		class FormsExport ExistingWindowCollection {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			using RawHandleDictionary = std::map<::HWND, Window*>;
			
		public:
			using key_type = ::HWND;
			using mapped_type = Window;
			using value_type = meta::undefined_t;
			using reference = Window&;
			using const_reference = Window const&;
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			RawHandleDictionary	Storage;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			satisfies(ExistingWindowCollection,
				IsRegular,
				NotSortable
			);
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			template <typename Self>
			nstd::mirror_cv_ref_t<Self, Window>&
			at(this Self&& self, key_type handle) {
				if (auto const pos = self.Storage.find(handle); pos == self.Storage.end())
					throw runtime_error{"Unrecognised window handle {}", to_hexString<8>(uintptr_t(handle))};
				else
					return *pos->second;
			}

			template <typename Self>
			nstd::mirror_cv_ref_t<Self, Window>&
			operator[](this Self&& self, key_type handle) {
				return *self.Storage.at(handle);
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			bool 
			contains(key_type handle) const {
				return this->Storage.contains(handle);
			}
			
			Window*
			find(key_type handle) const & {
				if (auto const pos = this->Storage.find(handle); pos == this->Storage.end())
					return nullptr;
				else
					return pos->second;
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			void 
			add(key_type handle, reference object) {
				this->Storage.emplace(handle, &object);
			}

			void 
			remove(key_type handle) {
				this->Storage.erase(handle);
			}
		};
		
		//! @brief	Navigates over (the handles of) descendant windows
		class FormsExport HierarchyIterator
		  : public boost::iterator_facade<HierarchyIterator, ::HWND, boost::forward_traversal_tag>
		{
			friend class boost::iterator_core_access;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			using type = HierarchyIterator;
			using base = boost::iterator_facade<type, ::HWND, boost::forward_traversal_tag>;
			using HandleCollection = std::vector<::HWND>;

			unsigned constexpr
			inline static npos = UINT32_MAX;

		public:
			enum SearchBehaviour { AllDescendants = 1, DirectDescendants = 2 };
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			mutable HandleCollection  Children;
			SearchBehaviour           Flags = static_cast<SearchBehaviour>(0);
			::HWND	                  Parent = nullptr;
			unsigned                  Index = npos;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		 public:
			HierarchyIterator(::HWND parent, SearchBehaviour descendants) noexcept 
			  : Flags{descendants}, Parent{parent}, Index{0}
			{
				std::ignore = ::EnumChildWindows(parent, &type::onNextChildWindow, (::LPARAM)(uintptr_t)this);
				if (this->Children.empty()) 
					*this = type{};
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			satisfies(HierarchyIterator,
				IsSemiRegular,
				NotSortable
			);
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			::BOOL
			static CALLBACK onNextChildWindow(::HWND child, ::LPARAM iterator);
			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			bool 
			equal(const type& r) const {
				return this->Parent == r.Parent 
				    && this->Index == r.Index;
			}

			::HWND& 
			dereference() const { 
				return this->Children[this->Index]; 
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			void 
			increment() { 
				if (++this->Index == this->Children.size())
					*this = type{};
			}
		};
		
		//! @brief	Collection of (direct) child windows
		//! @details  Enumeration occurs over a fixed-length snapshot taken by @c begin()
		//! @remarks  Requires the declaration of @c HierarchyIterator, above
		class FormsExport ChildWindowCollection 
		{
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			template <nstd::AnyOf<Window,Window const> MaybeConstWindow>
			using iterator_t = boost::transform_iterator<std::decay_t<MaybeConstWindow&(::HWND)>, HierarchyIterator>;

		public:
			using const_iterator = iterator_t<Window const>;
			using iterator = iterator_t<Window>;
			using value_type = Window;
			using reference = Window&;
			using const_reference = Window const&;
			using size_type = size_t;
			using difference_type = ptrdiff_t;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			const Window&  Parent;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			ChildWindowCollection(const Window& owner) : Parent{owner}
			{}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			satisfies(ChildWindowCollection,
				NotDefaultConstructible,
				IsCopyConstructible,
				NotCopyAssignable,
				NotEqualityComparable,
				NotSortable
			);
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			template <typename Self>
			iterator_t<nstd::mirror_cv_ref_t<Self,Window>>
			begin(this Self&& self) {
				Invariant(self.Parent.exists());
				return boost::make_transform_iterator(
					HierarchyIterator{ self.Parent.handle(), HierarchyIterator::DirectDescendants }, 
					&ChildWindowCollection::lookupWindow<nstd::mirror_cv_ref_t<Self,Window>>
				);
			}

			template <typename Self>
			iterator_t<nstd::mirror_cv_ref_t<Self,Window>>
			end(this Self&& self) {
				return boost::make_transform_iterator(
					HierarchyIterator{}, 
					&ChildWindowCollection::lookupWindow<nstd::mirror_cv_ref_t<Self,Window>>
				);
			}
			
			template <typename Self>
			nstd::return_t<nstd::mirror_cv_ref_t<Self, Window>&>
			operator[](this Self&& self, uint16_t const id)  {
				return Window::ExistingWindows[self.handle(id)];
			}

		private:
			template <nstd::AnyOf<Window,Window const> ReturnType>
			nstd::return_t<ReturnType&>
			static lookupWindow(::HWND handle) {
				return Window::ExistingWindows[handle];
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			const_iterator
			cbegin() const {
				return this->begin();
			}

			const_iterator
			cend() const {
				return this->end();
			}

			bool
			contains(uint16_t const id) const {
				//! TODO: Check this no longer returns 'true' for any managed window after fix to handle() below
				return Window::ExistingWindows.contains(this->handle(id));
			}
		
			bool
			exists(uint16_t const id) const {
				return this->handle(id) != nullptr;
			}

			Window*
			find(uint16_t const id) const {
				return Window::ExistingWindows.find(this->handle(id));
			}

			::HWND
			handle(uint16_t const id) const {
				return ::GetDlgItem(this->Parent.handle(), id);
			}

			size_type
			size() const = delete;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		};
	
		//! @brief	Enhances message results with state indicating whether they were handled at all
		class FormsExport Response {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			enum Result { 
				Invalid,        //!< [internal] Design flaw within wndproc
				Handled,        //!< Message was handled 
				Unhandled,      //!< Message wasn't handled
				Error           //!< Error during handling [result will be 'unhandled']
			};
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			Result                    Status = Invalid;
			std::optional<::LRESULT>  Value;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			explicit constexpr
			Response(Result r) noexcept : Status(r)
			{}

			implicit constexpr
			Response(::LRESULT value) noexcept : Status(Handled), Value(value)
			{}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			satisfies(Response,
				constexpr IsRegular noexcept,
				NotSortable
			);
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		};

		//! @brief	Virtual collection of registered timers for this window
		class FormsExport TimerCollection
		{
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			Window& Owner;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			TimerCollection(Window& wnd) : Owner{wnd}
			{}
		
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			satisfies(TimerCollection,
				NotCopyable,
				NotEqualityComparable,
				NotSortable
			);
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			void
			add(uintptr_t ident, chrono::milliseconds period) {
				if (!::SetTimer(this->Owner.handle(), ident, win::DWord{period.count()}, nullptr))
					win::LastError{}.throwAlways("Failed to set timer #{}", std::to_string(ident));
			}
		
			void
			remove(uintptr_t ident) {
				if (!::KillTimer(this->Owner.handle(), ident))
					win::LastError{}.throwAlways("Failed to cancel timer #{}", std::to_string(ident));
			}
		};

		//! @brief	Logging sentry customized for the re-entrant nature of window procedures
		class FormsExport WndProcLoggingSentry {
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			std::string  Text;
			bool		 Common = false;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			WndProcLoggingSentry(::UINT message, const char* func = __builtin_FUNCTION()) {
				using namespace std::literals;
				if (!Window::MessageDatabase.contains(message)) 
					this->Text = func + " : Processing unrecognised message "s + to_hexString<4>(message) + " ";
				else if (!Window::MessageDatabase[message].Common)
					this->Text = func + " : Processing "s + Window::MessageDatabase[message].Name + " ";
				else
					this->Common = true;
			}

			~WndProcLoggingSentry() {
				if (!this->Common) 
					cdebugger << this->Text << std::endl;
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			satisfies(WndProcLoggingSentry,
				NotCopyable,
				NotEqualityComparable,
				NotSortable
			);
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			void 
			setResult(Response::Result res, ::LRESULT val) { 
				this->Text += (res == Response::Handled ? "Handled" : "Unhandled");
				this->Text += (" (" + to_hexString(val) + ")");
			}

			void 
			setException(const std::exception& e) { 
				using namespace std::literals;
				this->Text += (" (ERROR: "s + e.what() + ')');
			}
		};
	
	public:
		//! @brief	Sentinel: the message was not handled
		Response const
		static inline Unhandled { Response::Unhandled };

		//! @brief	Sentinel: error processing message
		Response const
		static inline Error { Response::Error };
		
		//! @brief	All window messages and their return values
		MessageDictionary const
		static inline MessageDatabase;

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Representation o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		//! @brief	Window beneath the cursor, if any
		nstd::return_t<Window const*>
		static BeneathCursor;

	public:
		//! @brief	All windows created by core-forms
		ExistingWindowCollection 
		static ExistingWindows; 
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		AnyColour           BackColour = SystemColour::Window;
		AnyColour           TextColour = SystemColour::WindowText;

	protected:
		//! @remarks Due to message handling being re-entrant there is a significant delay 
		//!          between releasing a smart-pointer and its release delegate returning,
		//!          so the raw-pointer window handle is preferable here for simplicity.
		::HWND                  Handle = nullptr;
		std::optional<Brush>    Background;
		DebuggingAide           Debug;
		SharedLookNFeelProvider LookNFeel;

	public:
		ChildWindowCollection	Children;
		TimerCollection         Timers;

		CreateWindowEvent	Created;
		WindowEvent         Destroyed;
		ShowWindowEvent		Shown;
		ShowWindowEvent		Hidden;
		PaintWindowEvent	Painted;
		ResizeWindowEvent   Resized;
		WindowEvent         Clicked;
	
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		Window();
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
		satisfies(Window,
			NotCopyable,
			NotMovable,
			NotEqualityComparable,
			NotSortable,
			IsPolymorphic
		);
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		void
		static onFirstSight(::HWND hWnd, CreateWindowEventArgs args) {
			if (!args.Data->lpCreateParams) {
				cdebugger << "Observed creation of native '" << args.Data->lpszClass << '\''
				          << "Window: title='" << (args.Data->lpszName ? L"" : args.Data->lpszName ) << '\''
				          << std::endl;
				return;
			}
			
			Invariant(args.data<CreationData*>() != nullptr);
			auto* const param = args.data<CreationData*>();
			Window* pThis = param->get();

			pThis->Handle = hWnd;
			pThis->Debug.setState(ProcessingState::BeingCreated,
								  args.Data->lpszClass, 
								  args.Data->lpszName);	// Not yet set for dialog controls
			
			Window::ExistingWindows.add(hWnd, *pThis);
		}
	
		Response 
		static onMinMaxInfo(MinMaxEventArgs args) {
			return Unhandled;
		}
	
		Response
		static onUnexpectedMessage(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) {
			if (message == WM_GETMINMAXINFO) {
				return Window::onMinMaxInfo({wParam,lParam});
			}
			else {
				if (message == WM_MOUSEMOVE && Window::BeneathCursor)
					Window::BeneathCursor = nullptr;
				
				using namespace std::literals;
				throw runtime_error{"Received {} for unrecognised window {}", Window::MessageDatabase.name(message), 
					to_hexString((uintptr_t)hWnd)};
			}
		}
	
	protected:
		::LRESULT 
		static CALLBACK DefaultMessageHandler(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam)
		{
			WndProcLoggingSentry log_entry(message);
			try {
				Window* wnd {};
				Response response;

				// Window lifetime tracking
				if (message == WM_NCCREATE) 
					Window::onFirstSight(hWnd, {wParam,lParam});
			
				// Search for the C++ object managing this handle
				if (wnd = Window::ExistingWindows.find(hWnd); !wnd) 
					response = Window::onUnexpectedMessage(hWnd, message, wParam, lParam);
				else {
					// Window lifetime tracking
					if (message == WM_DESTROY) 
						wnd->onDestructionStarted();

					// Offer the message to the C++ object managing this handle
					response = wnd->offerMessage(message, wParam, lParam);

					// Raise equivalent event, if any, after processing completed
					wnd->raiseMessageEvent(message, wParam, lParam);
				}

				Invariant(response.Status != Response::Invalid);
			
				::LRESULT result;
				// [HANDLED] Return the result provided by the handler
				if (response.Status == Response::Handled)
					result = *response.Value;

				// [UNHANDLED/ERROR] Let the C++ object managing this handle pass message to ::DefWindowProc()
				else if (wnd) 
					result = wnd->routeUnhandled(message, wParam, lParam);
				
				// [UNMANAGED] Pass message to ::DefWindowProc()
				else 
					result = ::DefWindowProc(hWnd, message, wParam, lParam);

				log_entry.setResult(response.Status == Response::Handled ? response.Status : Response::Unhandled, result);

				// Window lifetime tracking
				if (message == WM_CREATE) {
					wnd->onConstructionFinished();
				}
				else if (message == WM_NCDESTROY) {
					Invariant(wnd != nullptr);
					wnd->onLastSight(hWnd);
				}
						
				return result;
			} 
			// [ERROR] Return a value indicating we didn't handle the message (usually anything but zero)
			catch (const std::exception& e) {
				log_entry.setException(e);
				return Window::MessageDatabase[message].Unhandled;
			}
		}
		
		Warning
		static unrecognisedNotificationLogEntry(CommandEventArgs args);

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		SharedBrush
		background() const {
			if (this->Background)
				return this->Background->handle();

			if (std::holds_alternative<SystemColour>(this->BackColour))
				return SystemBrush::get(std::get<SystemColour>(this->BackColour)).handle();
			
			return this->wndcls().Background;
		}

		AnyColour
		backColour() const {
			return this->BackColour;
		}
		
		DeviceContext
		clientDC() const {
			return DeviceContext{
				SharedDeviceContext{::GetDC(this->handle()), this->handle()}
			};
		}
	
		Rect
		clientRect() const {
			Rect rc;
			::GetClientRect(this->handle(), rc);
			return rc;
		}
	
		// FIXME: I have no idea what this method does
		Rect
		clientRect(std::nullptr_t) const {
			Rect rc = this->clientRect();
			auto* pointsArray = reinterpret_cast<::POINT*>(static_cast<::RECT*>(rc));
			::MapWindowPoints(this->handle(), nullptr, pointsArray, 2);
			return rc;
		}
	
		Rect
		clientRect(Window const& alternateCoordinateSystem) const {
			Rect rc = this->clientRect();
			auto* pointsArray = reinterpret_cast<::POINT*>(static_cast<::RECT*>(rc));
			::MapWindowPoints(this->handle(), alternateCoordinateSystem.handle(), pointsArray, 2);
			return rc;
		}

		bool
		enabled() const {
			return ::IsWindowEnabled(this->handle()) != FALSE;
		}
	
		bool
		exists() const {
			return this->handle() && ::IsWindow(this->handle()) != FALSE;
		}

		template <nstd::Enumeration Style = ExWindowStyle>
		nstd::bitset<Style>
		exStyle() const {
			return static_cast<Style>(::GetWindowLongW(this->handle(),GWL_EXSTYLE));
		}
	
		::HFONT
		font() const {
			return GetWindowFont(this->handle());
		}

		uint16_t
		ident() const {
			return static_cast<uint16_t>(::GetDlgCtrlID(this->handle()));
		}
	
		WindowInfo
		info() const {
			::WINDOWINFO info{sizeof(info)};
			::GetWindowInfo(this->handle(), &info);
			return WindowInfo{info};
		}

		::HWND 
		handle() const {
			return this->Handle;
		}
		
		Window*
		parent() const {
			auto const wnd = ::GetParent(this->handle());
			return wnd ? &Window::ExistingWindows[wnd] : nullptr;
		}
		
		template <unsigned MessageId>
		::LRESULT
		post(std::optional<::WPARAM> first = std::nullopt, std::optional<::LPARAM> second = std::nullopt) const {
			return ::PostMessageW(this->handle(), MessageId, first.value_or(0), second.value_or(0));
		}
		
		WindowRole
		virtual role() const abstract;

		template <unsigned MessageId>
		::LRESULT
		send(std::optional<::WPARAM> first = std::nullopt, std::optional<::LPARAM> second = std::nullopt) const {
			return ::SendMessageW(this->handle(), MessageId, first.value_or(0), second.value_or(0));
		}

		template <nstd::Enumeration Style = WindowStyle>
		nstd::bitset<Style>
		style() const {
			return static_cast<Style>(::GetWindowLongW(this->handle(),GWL_STYLE));
		}

		std::wstring
		text() const {
			if (std::wstring::size_type const capacity = ::GetWindowTextLengthW(this->handle())+1; capacity == 1) 
				return {};
			else {
				std::wstring buffer(capacity, L'\0');
				if (auto const n = ::GetWindowTextW(this->handle(), buffer.data(), win::DWord{capacity}); n != capacity)
					buffer.erase(n, buffer.npos);
				return buffer;
			}
		}
		
		AnyColour
		textColour() const {
			return this->TextColour;
		}
		
		WindowClass::const_reference
		virtual wndcls() const abstract;
		
		Rect
		wndRect() const {
			Rect rc;
			::GetWindowRect(this->handle(), rc);
			return rc;
		}
	
		Rect
		wndRect(Window const& alternateCoordinateSystem) const {
			Rect rc = this->wndRect();
			::POINT* pointsArray = reinterpret_cast<POINT*>(static_cast<::RECT*>(rc));
			::MapWindowPoints(nullptr, alternateCoordinateSystem.handle(), pointsArray, 2);
			return rc;
		}
	
		std::optional<Region>
		wndRgn() const {
			Region rgn;
			if (!::GetWindowRgn(this->handle(), rgn))
				return std::nullopt;
			return rgn;
		}
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		void
		backColour(AnyColour newColour) {
			this->BackColour = newColour;
		}
		
		void
		background(SharedBrush newBackground) {
			ThrowIfEmpty(newBackground);
			this->Background = Brush{newBackground};
		}

		void 
		create(std::wstring_view text, WindowStyle style, std::optional<Rect> area = std::nullopt) {
			auto params = CreateWindowParamsBuilder{}.withClass(this->wndcls())
			                                         .withStyle(style)
			                                         .withText(text)
			                                         .withParameter(this)
			                                         .build();
			if (area)
				params.Area = *area;

			this->createInternal(params);
		}

		void 
		create(const Window& parent, std::wstring_view text, WindowStyle style, Rect area)
		{
			auto const params = CreateWindowParamsBuilder{}.withClass(this->wndcls())
			                                               .withStyle(style)
			                                               .withParent(parent)
			                                               .withText(text)
			                                               .withParameter(this)
			                                               .withArea(area)
			                                               .build();
			this->createInternal(params);
		}

		void 
		destroy() {
			if (::DestroyWindow(this->handle()))
				this->Handle = nullptr;
		}
	
		void
		enable(bool enabled) const {
			::EnableWindow(this->handle(), win::Boolean{enabled});
		}
	
		void
		virtual font(const Font& newFont) {
			SetWindowFont(this->handle(), *newFont.handle(), FALSE);
		}
		
		void 
		hide() {
			::ShowWindow(this->handle(), SW_HIDE);
		}
		
		void
		invalidate(bool redraw = false) {
			::InvalidateRect(this->handle(), nullptr, win::Boolean{redraw});
		}

		void
		invalidate(Rect rc, bool redraw = false) {
			::InvalidateRect(this->handle(), rc, win::Boolean{redraw});
		}

		void
		lookNfeel(SharedLookNFeelProvider newFeel) {
			this->LookNFeel = ThrowIfEmpty(newFeel);
		}

		void
		order(::HWND after) {
			::SetWindowPos(this->handle(), after, -1, -1, -1, -1, 
				SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
		}

		void 
		show() {
			::ShowWindow(this->handle(), SW_SHOW);
		}
		
		void 
		show(signed flags) {
			::ShowWindow(this->handle(), flags);
		}
		
		void 
		move(Point pt) {
			::SetWindowPos(this->handle(), nullptr, pt.X, pt.Y, -1, -1, 
				SWP_NOSIZE|SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOACTIVATE);
		}
	
		void 
		reposition(Rect wnd) {
			::SetWindowPos(this->handle(), nullptr, wnd.Left, wnd.Top, wnd.width(), wnd.height(),
				SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOACTIVATE);
		}
	
		void 
		resize(Size sz) {
			::SetWindowPos(this->handle(), nullptr, -1, -1, sz.Width, sz.Height,
				SWP_NOMOVE|SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOACTIVATE);
		}
	
		void
		text(std::wstring_view s)  {
			::SetWindowTextW(this->handle(), s.data());
		}

		void
		textColour(AnyColour newColour) {
			ThrowIf(newColour, std::holds_alternative<meta::transparent_t>(newColour));
			this->TextColour = newColour;
		}

		void
		update() {
			::UpdateWindow(this->handle());
		}
		
		void
		wndRgn(Region rgn) const {
			::SetWindowRgn(this->handle(), rgn.detach(), win::Boolean{true});
		}

		Response 
		virtual onClose() {
			return Unhandled;
		}
	
		Response 
		virtual onCreate(CreateWindowEventArgs args) {
			return Unhandled;
		}
	
		Response 
		virtual onDestroy() {
			for (Window& c : this->Children)
				c.destroy();

			return Unhandled;
		}

		Response 
		virtual onCommand(CommandEventArgs args)
		{
			// [CONTROL] Reflect notification back to child control
			if (args.Source == CommandEventArgs::Control) 
				if (Window::ExistingWindows.contains(args.Notification->Handle))
					return Window::ExistingWindows[args.Notification->Handle].offerNotification(args.Notification->Code);

			// [DEBUG] Notification from child window we didn't create
			if (args.Source == CommandEventArgs::Control) {
				clog << Window::unrecognisedNotificationLogEntry(args);
				return Unhandled;
			}

			return this->offerNotification(args.Ident);
		}
	
		Response 
		virtual onEraseBackground(EraseBackgroundEventArgs args) {
			return Unhandled;
		}
		
		Response
		virtual onGetObject(GetObjectEventArgs args) {
			if (args.Object != ObjectId::Client)
				return Unhandled;

			gsl::cwzstring clsName {};
			switch (this->role())
			{
			case WindowRole::CheckButton:    clsName = WC_BUTTON; break;
			case WindowRole::ComboBox:		 clsName = WC_COMBOBOX; break;
			case WindowRole::Dialog:		 clsName = WC_DIALOG; break;
			case WindowRole::Graphic:		 clsName = WC_STATIC; break;
			case WindowRole::Grouping:		 clsName = WC_BUTTON; break;
			case WindowRole::Link:			 clsName = WC_LINK; break;
			case WindowRole::List:			 clsName = WC_LISTBOX; break;
			case WindowRole::ProgressBar:	 clsName = PROGRESS_CLASS; break;
			case WindowRole::PushButton:	 clsName = WC_BUTTON; break;
			case WindowRole::RadioButton:	 clsName = WC_BUTTON; break;
			case WindowRole::StaticText:	 clsName = WC_STATIC; break;
			case WindowRole::Text:			 clsName = WC_EDIT; break;

			default:
				return Unhandled;
			}

			com::shared_ptr<::IAccessible> accessible;
			if (clsName)
				win::ThrowingHResult hr = ::CreateStdAccessibleProxyW(this->handle(), clsName, args.Flags, IID_IAccessible, std::out_ptr(accessible, adopt));
			else
				win::ThrowingHResult hr = ::CreateStdAccessibleObject(this->handle(), args.Flags, IID_IAccessible, std::out_ptr(accessible, adopt));

			com::shared_ptr<::IAccessible> decorator = com::make_shared<::IAccessible,Accessible>(*this,accessible);
			return ::LresultFromObject(IID_IAccessible, args.Flags, decorator);
		}

		Response
		virtual onMouseDown(MouseEventArgs args) {
			return Unhandled;
		}
	
		Response
		virtual onMouseEnter(MouseEventArgs args) {
			return Unhandled;
		}

		Response
		virtual onMouseLeave() {
			return Unhandled;
		}

		Response
		virtual onMouseMove(MouseEventArgs args) {
			return Unhandled;
		}
	
		Response
		virtual onMouseUp(MouseEventArgs args) {
			return Unhandled;
		}

		Response 
		virtual onMeasureItem(MeasureItemEventArgs args) {
			if (auto* wnd = this->Children.find(args.Ident); wnd)
				return wnd->onMeasureItem(args);

			return Unhandled;
		}

		Response 
		virtual onOwnerDraw(OwnerDrawEventArgs args) {
			if (args.Window && args.Window != this)
				return args.Window->onOwnerDraw(args);

			return Unhandled;
		}
	
		Response 
		virtual onOwnerDrawMenu(OwnerDrawMenuEventArgs args) {
			return Unhandled;
		}

		Response 
		virtual onPaint(PaintWindowEventArgs args) {
			return Unhandled;
		}
		
		Response 
		virtual onResize(ResizeWindowEventArgs args) {
			return Unhandled;
		}
	
		Response 
		virtual onShow(ShowWindowEventArgs args) {
			return Unhandled;
		}
	
		Response 
		virtual onHide(ShowWindowEventArgs args) {
			return Unhandled;
		}
	
		Response 
		virtual onTimer(TimerEventArgs args) {
			return Unhandled;
		}
		
		Response 
		virtual onUser(UserEventArgs args) {
			return Unhandled;
		}

		Response
		virtual onNonClientActivate(NonClientActivateEventArgs args) {
			return Unhandled;
		}
	
		Response 
		virtual onNonClientCreate(CreateWindowEventArgs args) {
			return Unhandled;
		}
	
		Response 
		virtual onNonClientDestroy() {
			return Unhandled;
		}
	
		Response
		virtual onNonClientHitTest(NonClientHitTestEventArgs args) {
			return Unhandled;
		}
	
		Response
		virtual onNonClientMouseDown(NonClientMouseEventArgs args) {
			return Unhandled;
		}
	
		Response
		virtual onNonClientMouseLeave() {
			return Unhandled;
		}

		Response
		virtual onNonClientMouseMove(NonClientMouseEventArgs args) {
			return Unhandled;
		}
	
		Response
		virtual onNonClientMouseUp(NonClientMouseEventArgs args) {
			return Unhandled;
		}
	
		Response
		virtual onNonClientPaint(NonClientPaintEventArgs args) {
			return Unhandled;
		}

	protected:
		//! @brief	Stringify notification this window sends to its parent
		gsl::czstring
		virtual notificationName(::UINT [[maybe_unused]] notification) {
			// Default implementation sends no notifications so there's never need to stringize them
			return "";
		}
		

		//! @brief	Decode message arguments and offer to this object
		Response
		offerMessage(::UINT message, ::WPARAM wParam, ::LPARAM lParam) 
		{
			auto const _ = this->Debug.setTemporaryState(
				{ProcessingState::MessageProcessing, Window::MessageDatabase.name(message)}
			);
			return this->onOfferMessage(message, wParam, lParam);
		}
		
		Response
		virtual onOfferMessage(::UINT message, ::WPARAM wParam, ::LPARAM lParam) 
		{
			switch (message) {
			case WM_CLOSE: 
				return this->onClose();

			case WM_CREATE: 
				return this->onCreate({wParam,lParam});
			
			case WM_DESTROY:
				return this->onDestroy();
			
			case WM_ERASEBKGND:
				return this->onEraseBackground({wParam,lParam});
				
			case WM_GETOBJECT:
				return this->onGetObject({wParam,lParam});
			
			case WM_LBUTTONDOWN:
				return this->onMouseDown({MouseMessage::ButtonDown,MouseButton::Left,wParam,lParam});
			
			case WM_MOUSELEAVE:
				return this->onMouseLeave();
			
			case WM_MOUSEMOVE:
				if (Window::BeneathCursor != this) {
					Window::BeneathCursor = this;
					// FIXME: Merge onMouseEnter() into onMouseMove() with MouseMessage::Enter to reduce # of mouse-related events
					if (auto r = this->onMouseEnter({MouseMessage::Enter,MouseButton::None,wParam,lParam}); r != Unhandled)
						return r;
				}
				return this->onMouseMove({MouseMessage::Move,MouseButton::None,wParam,lParam});
			
			case WM_LBUTTONUP:
				return this->onMouseUp({MouseMessage::ButtonUp,MouseButton::Left,wParam,lParam});
			
			case WM_PAINT:
				return this->onPaint({this});
			
			case WM_MEASUREITEM:
				return this->onMeasureItem({*this,wParam,lParam});

			case WM_DRAWITEM:
				return wParam ? this->onOwnerDraw({wParam,lParam}) 
							  : this->onOwnerDrawMenu({wParam,lParam});

			case WM_SHOWWINDOW:
				return wParam ? this->onShow({wParam,lParam})
							  : this->onHide({wParam,lParam});
		
			case WM_SIZE:
				return this->onResize({wParam,lParam});
			
			case WM_COMMAND:
				return this->onCommand({wParam, lParam});
		
			case WM_TIMER:
				return this->onTimer({wParam, lParam});
		
			case WM_NCACTIVATE: 
				return this->onNonClientActivate({this,wParam,lParam});
			
			case WM_NCCREATE: 
				return this->onNonClientCreate({wParam,lParam});
			
			case WM_NCDESTROY: 
				return this->onNonClientDestroy();
		
			case WM_NCHITTEST: 
				return this->onNonClientHitTest({wParam,lParam});
			
			case WM_NCLBUTTONDOWN:
				return this->onNonClientMouseDown({MouseMessage::ButtonDown,MouseButton::Left,wParam,lParam});
			
			case WM_NCMOUSELEAVE:
				return this->onNonClientMouseLeave();
			
			case WM_NCMOUSEMOVE:
				return this->onNonClientMouseMove({MouseMessage::Move,MouseButton::None,wParam,lParam});
			
			case WM_NCLBUTTONUP:
				return this->onNonClientMouseUp({MouseMessage::ButtonUp,MouseButton::Left,wParam,lParam});
			
			case WM_NCPAINT:
				return this->onNonClientPaint({this,wParam,lParam});

			default:
				if (message >= WM_USER && message < WM_APP)
					return this->onUser({message,wParam,lParam});
				
				return Unhandled;
			}
		} 
		
		//! @brief	Reflect notification back to sender
		Response
		offerNotification(::UINT notification) {
			auto const _ = this->Debug.setTemporaryState(
				{ProcessingState::NotificationProcessing, this->notificationName(notification)}
			);
			return this->onOfferNotification(notification);
		}
		
		Response
		virtual onOfferNotification(::UINT [[maybe_unused]] notification) {
			// Default implementation ignores all reflected notifications
			return Unhandled;
		}
		
		//! @brief	Notify arbitrary set of listeners for this message
		void
		raiseMessageEvent(::UINT message, ::WPARAM wParam, ::LPARAM lParam) {
			auto const _ = this->Debug.setTemporaryState(
				{ProcessingState::EventProcessing, Window::MessageDatabase.name(message)}
			);
			this->onRaiseMessageEvent(message, wParam, lParam);
		}

		void
		virtual onRaiseMessageEvent(::UINT message, ::WPARAM wParam, ::LPARAM lParam) 
		{
			switch (message) {
			case WM_CREATE: 
				this->Created.raise(*this, CreateWindowEventArgs{wParam,lParam});
				return;

			case WM_DESTROY:
				this->Destroyed.raise(*this);
				return;
			
			case WM_SHOWWINDOW:
				if (wParam) {
					this->Shown.raise(*this, ShowWindowEventArgs{wParam,lParam});
				}
				else {
					this->Hidden.raise(*this, ShowWindowEventArgs{wParam,lParam});
				}
				return;
				
			case WM_SIZE:
				this->Resized.raise(*this, ResizeWindowEventArgs{wParam,lParam});
				return;
			
			case WM_PAINT:
				this->Painted.raise(*this, PaintWindowEventArgs{this});
				return;
			}
		} 
	
		//! @brief	Pass an unhandled message to a different window procedure (eg. system default)
		::LRESULT 
		routeUnhandled(::UINT message, ::WPARAM wParam, ::LPARAM lParam) {
			auto const _ = this->Debug.setTemporaryState(
				{ProcessingState::DefaultProcessing, Window::MessageDatabase.name(message)}
			);
			return this->onRouteUnhandled(message, wParam, lParam);
		}
		
		::LRESULT 
		virtual onRouteUnhandled(::UINT message, ::WPARAM wParam, ::LPARAM lParam) {
			// Default implementation passes to system default
			return ::DefWindowProc(this->Handle, message, wParam, lParam);
		}
		
	private:
		void 
		createInternal(CreateWindowParams const& w) 
		{
			this->Debug.setState(ProcessingState::BeingCreated);

			if (!::CreateWindowExW(NULL, w.Class, w.Text.data(), win::DWord{w.Style},
				w.Area.Left, w.Area.Top, w.Area.width(), w.Area.height(), w.Parent, w.Menu, 
				w.Module, (void*)&w.Parameter)) 
			{
				this->Debug.setState(ProcessingState::Idle);
				win::LastError{}.throwIfError("Failed to create '{}' window", to_string(w.Class));
			}

			this->Debug.setState(ProcessingState::Idle);
		}

		void
		onLastSight(::HWND hWnd) {
			Window::ExistingWindows.remove(hWnd);
			this->Debug.setState(ProcessingState::NotApplicable);
		}
	
		void
		onConstructionFinished() {
			this->Debug.setState(ProcessingState::Idle);
		}
	
		void
		onDestructionStarted() {
			this->Debug.setState(ProcessingState::BeingDestroyed);
		}
	};
} // namespace core::forms
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o