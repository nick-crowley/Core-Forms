#pragma once
#include "library/core.Forms.h"
#include "windows/ChildWindowIterator.h"
#include "support/DebugStream.h"
#include "dialogs/DialogTemplateReader.h"
#include "dialogs/DialogTemplateWriter.h"
#include "graphics/Font.h"
#include "system/WindowMessageDictionary.h"
#include "graphics/Region.h"
#include "support/ObservableEvent.h"
#include "windows/ILookNFeelProvider.h"
#include "windows/WindowClass.h"
#include "windows/WindowEventArgs.h"
#include "windows/WindowInfo.h"
#include "windows/WindowStyle.h"

namespace core::forms
{
	class Window {
	public:
		class Response {
		public:
			enum Result { Invalid, Handled, Unhandled, Error };

		public:
			Result                    Status = Invalid;
			std::optional<::LRESULT>  Value;

		public:
			explicit
			Response(Result r) : Status(r)
			{}

			implicit
			Response(::LRESULT value) : Status(Handled), Value(value)
			{}

			satisfies(Response,
				IsRegular,
				NotSortable
			);
		};

		class ChildWindowCollection {
			using const_iterator = boost::transform_iterator<std::decay_t<Window*(::HWND)>, ConstChildWindowIterator>;
	
		private:
			const Window&  Parent;

		public:
			ChildWindowCollection(const Window& owner) : Parent(owner)
			{}

		public:
			const_iterator
			begin() const {
				return boost::make_transform_iterator(
					ConstChildWindowIterator{ this->Parent.handle() }, 
					[](::HWND w) { return Window::s_ExistingWindows[w]; }
				);
			}

			const_iterator
			end() const {
				return boost::make_transform_iterator(
					ConstChildWindowIterator::npos, 
					[](::HWND w) { return Window::s_ExistingWindows[w]; }
				);
			}

			bool
			contains(uint16_t const id) const {
				return Window::s_ExistingWindows.contains(this->handle(id));
			}
		
			bool
			exists(uint16_t const id) const {
				return this->handle(id) != nullptr;
			}

			::HWND
			handle(uint16_t const id) const {
				return ::GetWindow(this->Parent.handle(), id);
			}

			Window&
			operator[](uint16_t const id) const {
				return *Window::s_ExistingWindows[this->handle(id)];
			}
		};

		//! Encapsulates registered window timers
		class TimerCollection
		{
			Window& Owner;

		public:
			TimerCollection(Window& wnd) : Owner{wnd}
			{}
		
			satisfies(TimerCollection,
				NotCopyable,
				NotEqualityComparable
			);

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

	protected:
		class ExistingWindowCollection {
			using RawHandleDictionary = std::map<::HWND, Window*>;
			using key_t = ::HWND;

		private:
			RawHandleDictionary	Storage;

		public:
			ExistingWindowCollection() = default;

		public:
			Window* 
			at(key_t handle) const & {
				if (auto pos = this->Storage.find(handle); pos == this->Storage.end())
					throw runtime_error{"Unrecognised window handle {}", to_hexString<8>(uintptr_t(handle))};
				else
					return pos->second;
			}

			bool 
			contains(key_t handle) const {
				return this->Storage.contains(handle);
			}

			Window* 
			operator[](key_t handle) const & {
				return this->Storage.at(handle);
			}

		public:
			void 
			add(key_t handle, Window* object) {
				this->Storage.emplace(handle, object);
			}

			void 
			remove(key_t handle) {
				this->Storage.extract(handle);
			}
		};
	
		class CreateWindowParameter {		
			uint16_t   Length = 0;
			uint16_t   DuplicateLength = 0;		// Fix: ABI compatibility with how DialogBoxIndirectParamW does it
			std::byte  Data[8] {};

		public:
			CreateWindowParameter() = default;

			explicit 
			CreateWindowParameter(Window* w) 
			  : Length{sizeof(Window*)}, DuplicateLength{Length}
			{
				auto const src = std::as_bytes(std::span<Window*>{&w,1});
				std::copy(src.begin(), src.end(), std::begin(this->Data));
			}

		public:
			std::vector<std::byte> 
			asBytes() const {
				return { std::begin(this->Data), std::begin(this->Data)+this->Length };
			}

			Window* 
			get() {
				return std::span<Window*>{reinterpret_cast<Window**>(this->Data),1}.front();
			}
		};

		class CreateWindowBuilder {
		public:
			Rect Area {CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT };
			ResourceId Class;
			std::wstring_view Text;
			WindowStyle Style = WindowStyle::None;
			::HWND Parent = nullptr;
			::HMENU Menu = nullptr;
			::HMODULE Module = nullptr;
			CreateWindowParameter Parameter;

		public:
			CreateWindowBuilder() = default;
		};
	
		class WindowProcLoggingSentry {
		private:
			std::string  Text;
			bool		 Common = false;

		public:
			WindowProcLoggingSentry(gsl::czstring func, ::UINT message) {
				using namespace std::literals;
				if (!s_MessageDatabase.contains(message)) 
					this->Text = func + " : Processing unrecognised message "s + to_hexString<4>(message) + " ";
				else if (!s_MessageDatabase[message].Common)
					this->Text = func + " : Processing "s + s_MessageDatabase[message].Name + " ";
				else
					this->Common = true;
			}

			~WindowProcLoggingSentry() {
				if (!this->Common) 
					cdebug << this->Text << std::endl;
			}

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
	
		//! @brief	Tracks the Window life-cycle and identifies current high-level processing loop
		class ProcessingState {
		public:
			enum CurrentState { NotApplicable, BeingCreated, BeingDestroyed, Idle, 
				DefaultProcessing, DialogProcessing, MessageProcessing, EventProcessing, NotificationProcessing };

		public:
			CurrentState State;
			gsl::czstring Message = "";

		public:
			implicit
			ProcessingState(CurrentState s) 
			  : State(s)
			{}

			ProcessingState(CurrentState s, gsl::czstring const msg) 
			  : State(s), Message(msg) 
			{}
		};

		//! @brief	Identifies the window, its state, and its current message-processing loop
		class DebuggingAide 
		{
			auto constexpr
			inline static toChar = [](wchar_t c){ return static_cast<char>(c); };

		private:
			uint32_t const       MagicNumber = 12345678;
			std::array<char,16>  Class {};
			ProcessingState      State {ProcessingState::NotApplicable};
			std::array<char,16>	 Text  {};

		public:
			DebuggingAide() = default;
		
		public:
			ProcessingState
			setState(ProcessingState newstate) {
				return std::exchange(this->State, newstate);
			}
		
			ProcessingState
			setState(ProcessingState state, std::wstring_view cls, std::wstring_view txt) {
				// Copy short preview; assume text is latin charset
				std::transform(cls.data(), cls.data()+std::min<int>(cls.size(),lengthof(this->Class)-1), this->Class.begin(), toChar);
				std::transform(txt.data(), txt.data()+std::min<int>(txt.size(),lengthof(this->Text)-1), this->Text.begin(), toChar);
				this->Class.back() = '\0';
				this->Text.back() = '\0';
				return std::exchange(this->State, state);
			}
		
			auto
			setTemporaryState(ProcessingState newState) {
				return gsl::finally([this, prev = this->setState(newState)]{ 
					this->State = prev; 
				});
			}
		};

	private:
		using SharedLookNFeelProvider = std::shared_ptr<ILookNFeelProvider>;
		using wndclass_constref_t = WindowClass const&;

	public:
		Response const  
		static inline Unhandled { Response::Unhandled };

		Response const
		static inline Error { Response::Error };

		ExistingWindowCollection 
		static s_ExistingWindows; 

		WindowMessageDictionary 
		static s_MessageDatabase;

	protected:
		//! @remarks Due to message handling being re-entrant there is a significant delay 
		//!          between releasing smart-pointer and the release delegate returning.
		//!          Therefore raw-pointers are preferable here due to simplicity.
		::HWND                  Handle;
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
		WindowEvent         Clicked;
	
	public:
		Window();

		satisfies(Window,
			NotCopyable,
			NotMovable,
			NotEqualityComparable,
			NotSortable,
			virtual IsDestructible
		);

	public:
		Rect
		clientRect() const {
			Rect rc;
			::GetClientRect(this->handle(), rc);
			return rc;
		}
	
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
	
		template <meta::Enumeration Style = ExWindowStyle>
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
			return ::GetDlgCtrlID(this->handle());
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
			return wnd ? s_ExistingWindows[wnd] : nullptr;
		}

		template <unsigned MessageId>
		::LRESULT
		send(std::optional<::WPARAM> first = std::nullopt, std::optional<::LPARAM> second = std::nullopt) const {
			return ::SendMessageW(this->handle(), MessageId, first.value_or(0), second.value_or(0));
		}

		template <meta::Enumeration Style = WindowStyle>
		nstd::bitset<Style>
		style() const {
			return static_cast<Style>(::GetWindowLongW(this->handle(),GWL_STYLE));
		}

		std::wstring
		text() const {
			if (std::wstring::size_type capacity = ::GetWindowTextLengthW(this->handle())+1; capacity == 1) 
				return {};
			else {
				std::wstring s(capacity, L'\0');
				::GetWindowTextW(this->handle(), s.data(), static_cast<int>(capacity));
				s.pop_back();
				return s;
			}
		}
	
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
	
		Region
		wndRgn() const {
			Region rgn;
			::GetWindowRgn(this->handle(), rgn);
			return rgn;
		}

	public:
		void 
		create(std::wstring_view text, WindowStyle style, std::optional<Rect> area = std::nullopt) {
			CreateWindowBuilder wnd;
			if (area) {
				wnd.Area = *area;
			}
			wnd.Class = ResourceId::parse(this->wndcls().lpszClassName);
			wnd.Module = this->wndcls().hInstance;
			wnd.Parameter = CreateWindowParameter{this};
			wnd.Text = text;
			wnd.Style = style;

			this->create(wnd);
		}

		void 
		create(const Window& parent, std::wstring_view text, WindowStyle style, Rect area)
		{
			CreateWindowBuilder wnd;
			wnd.Area = area;
			wnd.Parent = parent.handle();
			wnd.Class = ResourceId::parse(this->wndcls().lpszClassName);
			wnd.Module = this->wndcls().hInstance;
			wnd.Parameter = CreateWindowParameter{this};
			wnd.Text = text;
			wnd.Style = style;

			this->create(wnd);
		}
	
		wndclass_constref_t
		virtual wndcls() abstract;
	
	public:
		void 
		destroy() {
			if (::DestroyWindow(this->handle()))
				this->Handle = nullptr;
		}
	
		void
		font(::HFONT f) {
			SetWindowFont(this->handle(), f, FALSE);
		}
	
		void
		font(const Font& f) {
			SetWindowFont(this->handle(), f.handle(), FALSE);
		}
	
		void
		order(::HWND after) {
			::SetWindowPos(this->handle(), after, -1, -1, -1, -1, 
				SWP_NOMOVE|SWP_NOSIZE|/*SWP_NOOWNERZORDER|*/SWP_NOACTIVATE);
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
		update() {
			::UpdateWindow(this->handle());
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
			// NB: Destroying child controls while iterating over them would invalidate the 
			//     iterators because child-control collection is a virtual view
			std::list<Window*> const controls { this->Children.begin(), this->Children.end() };
			for (Window* const c : controls) {   // FIXME: Should ChildWindowCollection be std::ranges::view instead?
				c->destroy();
			}

			return Unhandled;
		}
	
		Response 
		virtual onCommand(CommandEventArgs args)
		{
			// [CONTROL] Reflect notification back to child control
			if (args.Source == CommandEventArgs::Control) 
				if (s_ExistingWindows.contains(args.Notification->Handle))
					return s_ExistingWindows[args.Notification->Handle]->offerNotification(args.Notification->Code);

			// [DEBUG] Notification from child window we didn't create
			if (args.Source == CommandEventArgs::Control) {
				cdebug << "Window::onCommand : warning : WM_COMMAND Received " << to_hexString<4>(args.Notification->Code)
						<< " from unknown window id=" << ::GetDlgCtrlID(args.Notification->Handle) 
						<< " handle=" << to_hexString<8>((uintptr_t)args.Notification->Handle)
						<< std::endl;
				return Unhandled;
			}

			return this->offerNotification(args.Ident);
		}
	
		Response 
		virtual onEraseBackground(EraseBackgroundEventArgs args) {
			return Unhandled;
		}

		Response 
		virtual onOwnerDraw(OwnerDrawEventArgs args) {
			if (args.Window != this)
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
		virtual onActivateNonClient(ActivateNonClientEventArgs args) {
			return Unhandled;
		}
	
		Response 
		virtual onCreateNonClient(CreateWindowEventArgs args) {
			return Unhandled;
		}
	
		Response 
		virtual onDestroyNonClient() {
			return Unhandled;
		}
	
		Response
		virtual onHitTestNonClient(HitTestNonClientEventArgs args) {
			return Unhandled;
		}
	
		Response
		virtual onMouseDownNonClient(MouseNonClientEventArgs args) {
			return Unhandled;
		}
	
		Response
		virtual onMouseLeaveNonClient() {
			return Unhandled;
		}

		Response
		virtual onMouseMoveNonClient(MouseNonClientEventArgs args) {
			return Unhandled;
		}
	
		Response
		virtual onMouseUpNonClient(MouseNonClientEventArgs args) {
			return Unhandled;
		}
	
		Response
		virtual onPaintNonClient(PaintNonClientEventArgs args) {
			return Unhandled;
		}

	protected:
		Response
		virtual offerMessage(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) 
		{
			switch (message) {
			case WM_CLOSE: 
				return this->onClose();

			case WM_CREATE: 
				return this->onCreate({wParam,lParam});
			
			case WM_DESTROY:
				return this->onDestroy();
			
			case WM_ERASEBKGND:
				return this->onEraseBackground({hWnd,wParam,lParam});

			case WM_PAINT:
				return this->onPaint({this});
			
			case WM_DRAWITEM:
				return wParam ? this->onOwnerDraw({wParam,lParam}) 
							  : this->onOwnerDrawMenu({wParam,lParam});

			case WM_SHOWWINDOW:
				return wParam ? this->onShow({wParam,lParam})
							  : this->onHide({wParam,lParam});
		
			case WM_COMMAND:
				return this->onCommand({wParam, lParam});
		
			case WM_TIMER:
				return this->onTimer({wParam, lParam});
		
			case WM_NCACTIVATE: 
				return this->onActivateNonClient({this,wParam,lParam});
			
			case WM_NCCREATE: 
				return this->onCreateNonClient({wParam,lParam});
			
			case WM_NCDESTROY: 
				return this->onDestroyNonClient();
		
			case WM_NCHITTEST: 
				return this->onHitTestNonClient({wParam,lParam});
			
			case WM_NCLBUTTONDOWN:
				return this->onMouseDownNonClient({MouseEvent::ButtonDown,MouseButton::Left,wParam,lParam});
			
			case WM_NCMOUSELEAVE:
				return this->onMouseLeaveNonClient();
			
			case WM_NCMOUSEMOVE:
				return this->onMouseMoveNonClient({MouseEvent::Move,MouseButton::None,wParam,lParam});
			
			case WM_NCLBUTTONUP:
				return this->onMouseUpNonClient({MouseEvent::ButtonUp,MouseButton::Left,wParam,lParam});
			
			case WM_NCPAINT:
				return this->onPaintNonClient({this,wParam,lParam});
			}

			return Unhandled;
		} 

		Response
		virtual offerNotification(::UINT notification) {
			return Unhandled;
		}
	
		gsl::czstring
		virtual notificationName(::UINT notification) {
			// FIXME: Window::notificationName() isn't threadsafe. Consider returning fixed-string instead.
			static thread_local std::string str;
			return (str = to_hexString<4>(notification)).c_str();
		}

		void
		virtual raiseMessageEvent(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) 
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

			case WM_PAINT:
				this->Painted.raise(*this, PaintWindowEventArgs{this});
				return;
			}
		} 
	
		::LRESULT
		performDefaultProcessing(::UINT message, ::WPARAM wParam, ::LPARAM lParam)
		{
			auto const on_exit = this->Debug.setTemporaryState({ProcessingState::DefaultProcessing,s_MessageDatabase.name(message)});
			return this->unhandledMessage(this->handle(), message, wParam, lParam);
		}

		::LRESULT 
		virtual unhandledMessage(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam) {
			return ::DefWindowProc(hWnd, message, wParam, lParam);
		}

	private:
		void 
		create(CreateWindowBuilder& w) 
		{
			this->Debug.setState(ProcessingState::BeingCreated);

			if (!::CreateWindowExW(NULL, w.Class, w.Text.data(), static_cast<DWORD>(w.Style),
				w.Area.Left, w.Area.Top, w.Area.width(), w.Area.height(), w.Parent, w.Menu, w.Module, &w.Parameter)) 
			{
				this->Debug.setState(ProcessingState::Idle);
				win::LastError{}.throwIfError("Failed to create '{}' window", to_string(w.Class));
			}

			this->Debug.setState(ProcessingState::Idle);
		}

		//void
		//onFirstSight(::HWND hWnd) {
		//	s_ExistingWindows.add(hWnd, this);
		//	this->Handle = hWnd;	// this->Handle = make_handle(hWnd);
		//	this->DebugState = {ProcessingState::BeingCreated};	// Not yet set for dialog controls
		//}
	
		void
		onLastSight(::HWND hWnd) {
			s_ExistingWindows.remove(hWnd);
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

	private:
		void
		static onFirstSight(::HWND hWnd, CreateWindowEventArgs args) {
			if (!args.Data->lpCreateParams) 
				return;

			assert(args.data<CreateWindowParameter*>() != nullptr);
			auto* const param = args.data<CreateWindowParameter*>();
			Window* pThis = param->get();
		
			s_ExistingWindows.add(hWnd, pThis);
			assert(s_ExistingWindows[hWnd] != nullptr);

			pThis->Handle = hWnd;
			pThis->Debug.setState(ProcessingState::BeingCreated,
								  args.Data->lpszClass, 
								  args.Data->lpszName);	// Not yet set for dialog controls
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
				using namespace std::literals;
				throw runtime_error{"Received {} for unrecognised window {}", s_MessageDatabase.name(message), 
					to_hexString((uintptr_t)hWnd)};
			}
		}
	
	protected:
		::LRESULT 
		static CALLBACK DefaultMessageHandler(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam)
		{
			WindowProcLoggingSentry log_entry(__FUNCTION__, message);
			try {
				gsl::czstring const name = s_MessageDatabase.name(message);
				Window* wnd {};
				Response response;

				// Window lifetime tracking
				if (message == WM_NCCREATE) 
					Window::onFirstSight(hWnd, {wParam,lParam});
			
				// Search for the C++ object managing this handle
				if (!s_ExistingWindows.contains(hWnd)) 
					response = onUnexpectedMessage(hWnd, message, wParam, lParam);
				else {
					wnd = s_ExistingWindows[hWnd];

					// Window lifetime tracking
					if (message == WM_DESTROY) 
						wnd->onDestructionStarted();

					{
						// Offer the message to the C++ object managing this handle
						auto const on_exit = wnd->Debug.setTemporaryState({ProcessingState::MessageProcessing, name});
						response = wnd->offerMessage(hWnd, message, wParam, lParam);
					}

					{
						// [POST] Raise the associated event, if any
						auto const on_exit = wnd->Debug.setTemporaryState({ProcessingState::EventProcessing, name});
						wnd->raiseMessageEvent(hWnd, message, wParam, lParam);
					}
				}

				assert(response.Status != Response::Invalid);
			
				::LRESULT result;
				// [HANDLED] Return the result provided by the handler
				if (response.Status == Response::Handled) 			
					result = *response.Value;

				// [UNHANDLED/ERROR] Let the C++ object managing this handle pass message to ::DefWindowProc()
				else if (wnd) {	
					auto const on_exit = wnd->Debug.setTemporaryState({ProcessingState::DefaultProcessing, name});
					result = wnd->unhandledMessage(hWnd, message, wParam, lParam);
				}
				// [UNHANDLED/ERROR] Pass message to ::DefWindowProc()
				else 
					result = ::DefWindowProc(hWnd, message, wParam, lParam);

				log_entry.setResult(response.Status == Response::Handled ? response.Status : Response::Unhandled, result);

				// Window lifetime tracking
				if (message == WM_CREATE) {
					wnd->onConstructionFinished();
				}
				else if (message == WM_NCDESTROY) {
					assert(wnd != nullptr);
					wnd->onLastSight(hWnd);
				}
						
				return result;
			} 
			// [ERROR] Return a value indicating we didn't handle the message (usually anything but zero)
			catch (const std::exception& e) {
				log_entry.setException(e);
				return s_MessageDatabase[message].Unhandled;
			}
		}
	};
} // namespace core::forms