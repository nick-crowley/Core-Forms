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
#include "lookNfeel/ILookNFeelProvider.h"
#include "core/ObservableEvent.h"
#include "forms/Accessible.h"
#include "forms/BuiltInWindowMessages.h"
#include "forms/UnmanagedWindow.h"
#include "forms/WindowEventArgs.h"
#pragma comment (lib, "OleAcc.lib")
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::forms
{
	//! @brief	Manages the life-cycle and behaviour of a single window
	class FormsExport Window : private UnmanagedWindow
	{
		friend class DialogTemplate;	//!< Requires @c CreationData class

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		using base = UnmanagedWindow;

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
				Invariant(this->Length <= sizeof(Window*));
				// Suppress C4366 'result of the unary 'operator' operator may be unaligned' because we
				//  cast into byte-pointer before any read/writes are performed through it
#				pragma warning (suppress : 4366) 
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
		public:
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
		public:
			bool
			hasState(ProcessingState s) const {
				return this->State.State == s;
			}
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
			
			template <std::derived_from<Window> WindowType = Window>
			WindowType*
			find(key_type handle) const & {
				if (auto const pos = this->Storage.find(handle); pos == this->Storage.end())
					return nullptr;
				else
					return static_cast<WindowType*>(pos->second);
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
			enum SearchBehaviour { Ancestors = 1, Children = 2, Managed = 4, Unmanaged = 8 };
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			HandleCollection              mutable Results;
			nstd::bitset<SearchBehaviour> Flags;
			::HWND	                      Parent = nullptr;
			unsigned                      Index = type::npos;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		 public:
			HierarchyIterator(::HWND parent, SearchBehaviour descendants) noexcept;
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
				return this->Results[this->Index]; 
			}
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		private:
			void 
			increment() { 
				if (++this->Index == this->Results.size())
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
					HierarchyIterator{ self.Parent.handle(), HierarchyIterator::Children|HierarchyIterator::Managed }, 
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
			nstd::mirror_cv_ref_t<Self, Window>&
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

			template <std::derived_from<Window> WindowType = Window>
			WindowType*
			find(uint16_t const id) const {
				return static_cast<WindowType*>(Window::ExistingWindows.find(this->handle(id)));
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
			enum RoutingStatus { 
				Invalid,        //!< [internal] Design flaw within wndproc
				Handled,        //!< Message was handled 
				Unhandled,      //!< Message wasn't handled
				Error           //!< Error during handling [result will be 'unhandled']
			};
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			RoutingStatus             Status = Invalid;
			std::optional<::LRESULT>  Value;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			explicit constexpr
			Response(RoutingStatus status) noexcept : Status(status)
			{}

			template <typename Result>
				requires nstd::Integer<Result> 
			          || nstd::Enumeration<Result>
			implicit constexpr
			Response(Result value) noexcept : Status(Handled), Value(static_cast<::LRESULT>(value))
			{}

			template <nstd::ObjectPointer Result>
			implicit constexpr
			Response(Result value) noexcept : Response{std::bit_cast<::LRESULT>(value)}
			{}

			implicit constexpr
			Response(std::nullptr_t) noexcept = delete;
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			satisfies(Response,
				constexpr IsRegular noexcept,
				NotSortable
			);
			// o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

			// o~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~o
		public:
			explicit
			operator bool() const {
				return this->Status == Response::Handled;
			}
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
			setResult(Response::RoutingStatus status, ::LRESULT val) { 
				this->Text += (status == Response::Handled ? "Handled" : "Unhandled");
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
		inline static Unhandled { Response::Unhandled };

		//! @brief	Sentinel: error processing message
		Response const
		inline static Error { Response::Error };
		
		//! @brief	All window messages and their return values
		MessageDictionary const
		inline static MessageDatabase;

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
		std::optional<AnyColour>   BackColour;
		std::optional<Brush>       mutable Background;
		std::optional<AnyColour>   TextColour;
		std::optional<Font>        CustomFont;
		std::optional<Font>        Font;
		DebuggingAide              Debug;

	protected:
		SharedLookNFeelProvider LookNFeel;

	public:
		ChildWindowCollection	Children;
		TimerCollection         Timers;
		CreateWindowEvent	    Created;
		WindowEvent             Destroyed;
		SetFontEvent            FontChanged;
		ShowWindowEvent		    Shown;
		ShowWindowEvent		    Hidden;
		PaintWindowEvent	    Painted;
		ResizeWindowEvent       Resized;
		WindowEvent             Clicked;
	
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

			pThis->attach(hWnd);
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
				
				throw runtime_error{"Received {} for unrecognised window {}", Window::MessageDatabase.name(message), 
					to_hexString((uintptr_t)hWnd)};
			}
		}
	
	protected:
		::LRESULT 
		static CALLBACK defaultMessageHandler(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam)
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
				
				// Raise equivalent event, if any, after processing completed
				if (wnd)
					wnd->raiseMessageEvent(message, wParam, lParam);

				return result;
			} 
			// [ERROR] Return a value indicating we didn't handle the message (usually anything but zero)
			catch (const std::exception& e) {
				log_entry.setException(e);
				clog << Failure{"Exception processing {}: {}", Window::MessageDatabase[message].Name, e.what()};
				return Window::MessageDatabase[message].Unhandled;
			}
		}
		
		Warning
		static unrecognisedNotificationLogEntry(CommandEventArgs args);

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		using base::clientDC;
		using base::clientRect;
		using base::enabled;
		using base::exists;
		using base::exStyle;
		using base::ident;
		using base::info;
		using base::handle;
		using base::post;
		using base::send;
		using base::style;
		using base::text;
		using base::wndcls;
		using base::wndRect;
		using base::wndRgn;
		
		//! @brief  Retrieve cached background brush based on the background colour
		SharedBrush
		background() const {
			if (this->Background)
				return this->Background->handle();
			
			this->Background = Brush{this->BackColour.value_or(this->LookNFeel->window())};
			return this->Background->handle();
		}

		AnyColour
		backColour() const {
			return this->BackColour.value_or(this->LookNFeel->window());
		}
		
		Rect
		clientRect(Window& alternateCoordinateSystem) const {
			return base::clientRect(alternateCoordinateSystem.handle());
		}

		forms::Font
		font() const {
			return this->CustomFont.value_or(this->Font.value_or(this->LookNFeel->paragraph()));
		}

		Window*
		parent() const {
			// @bug	Refactor this to use Window::ExistingWindows.find() because parent may not be managed
			auto const wnd = ::GetParent(this->handle());
			return wnd ? &Window::ExistingWindows[wnd] : nullptr;
		}

		WindowRole
		virtual role() const {
			// Documentation states MSAA considers this a reasonable value for 'undefined'
			return WindowRole::Client;
		}

		AnyColour
		textColour() const {
			return this->TextColour.value_or(this->LookNFeel->primary());
		}
		
		Rect
		wndRect(Window& alternateCoordinateSystem) const {
			return base::wndRect(alternateCoordinateSystem.handle());
		}

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		using base::destroy;
		using base::enable;
		using base::hide;
		using base::invalidate;
		using base::order;
		using base::show;
		using base::move;
		using base::reposition;
		using base::resize;
		using base::update;

		void
		backColour(AnyColour newColour) {
			this->BackColour = newColour;
			this->Background = std::nullopt;
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
		focus() {
			::SetFocus(this->handle());
		}
		
		void
		font(const forms::Font& newFont) {
			Invariant(this->exists());
			this->CustomFont = newFont; 
			base::font(*newFont.handle(), true);
		}
		
		void
		textColour(AnyColour newColour) {
			ThrowIf(newColour, std::holds_alternative<meta::transparent_t>(newColour));
			this->TextColour = newColour;
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
			if (args.Source == CommandEventArgs::Control) {
				if (Window* sender = Window::ExistingWindows.find(args.Notification->Handle); sender)
					return sender->offerNotification(args.Notification->Code);

				// [DEBUG] Notification from child window we didn't create
				clog << Window::unrecognisedNotificationLogEntry(args);
				return Unhandled;
			}

			return this->offerNotification(args.Ident);
		}
	
		Response 
		virtual onControlColour(ControlColourEventArgs args) {
			return Unhandled;
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
		virtual onSetFont(SetWindowFontEventArgs args) {
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
		using base::attach;

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
			
			case WM_COMMAND:
				return this->onCommand({wParam, lParam});
		
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
			
			case WM_ERASEBKGND:
				return this->onEraseBackground({wParam,lParam});
				
			case WM_PAINT:
				return this->onPaint({this});
			
			case WM_CTLCOLORDLG:
			case WM_CTLCOLORSTATIC:
			case WM_CTLCOLOREDIT:
			case WM_CTLCOLORBTN:
			case WM_CTLCOLORLISTBOX:
			case WM_CTLCOLORSCROLLBAR:
				return this->onControlColour({message,wParam,lParam});

			case WM_DRAWITEM:
				return wParam ? this->onOwnerDraw({wParam,lParam}) 
							  : this->onOwnerDrawMenu({wParam,lParam});
				
			case WM_MEASUREITEM:
				return this->onMeasureItem({*this,wParam,lParam});

			case WM_SETFONT:
				return this->onSetFont({wParam,lParam});

			case WM_SHOWWINDOW:
				return wParam ? this->onShow({wParam,lParam})
							  : this->onHide({wParam,lParam});
		
			case WM_SIZE:
				return this->onResize({wParam,lParam});
			
			case WM_TIMER:
				return this->onTimer({wParam, lParam});
		
			case WM_NCACTIVATE: 
				return this->onNonClientActivate({*this,wParam,lParam});
			
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
				return this->onNonClientPaint({*this,wParam,lParam});

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
			
			case WM_SETFONT:
				this->FontChanged.raise(*this, SetWindowFontEventArgs{wParam,lParam});
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
			return ::DefWindowProc(this->handle(), message, wParam, lParam);
		}
		
		auto
		setTemporaryState(DebuggingAide::StateDescription newState) {
			return this->Debug.setTemporaryState(newState);
		}
	private:
		void 
		createInternal(CreateWindowParams const& w) 
		{
			Invariant(this->Debug.hasState(ProcessingState::Idle));

			if (!::CreateWindowExW(
				NULL, 
				w.Class, 
				w.Text.data(), 
				win::DWord{w.Style},
				w.Area.Left, w.Area.Top, w.Area.width(), w.Area.height(), 
				w.Parent, 
				w.Menu, 
				w.Module, 
				(void*)&w.Parameter)
			) {
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

		void
		this_FontChanged(Window&, SetWindowFontEventArgs args) {
			// Store a weak-reference to the incoming window font unless this message was generated
			//  due to calling @c Window::font() (because that just stored a strong-reference)
			if (!this->Font || args.NewFont != this->Font->handle())
				this->Font = forms::Font{args.NewFont};
		}
	};
} // namespace core::forms

namespace core::meta
{
	metadata bool Settings<bitwise_enum, forms::Window::HierarchyIterator::SearchBehaviour> = true;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o