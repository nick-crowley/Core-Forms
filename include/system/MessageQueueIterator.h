#pragma once
#include "library/core.Forms.h"
#include "system/Message.h"

namespace core::forms
{
	class MessageQueueIterator
	  : public boost::iterator_facade<MessageQueueIterator, const Message, boost::forward_traversal_tag>
	{
		friend class boost::iterator_core_access;

		using type = MessageQueueIterator;
		using base = boost::iterator_facade<MessageQueueIterator, const Message, boost::forward_traversal_tag>;
	
	public:
		static const MessageQueueIterator npos;

	private:
		Message Current;
		::HWND  Window;
	
	private:
		MessageQueueIterator() noexcept {
			this->Current.message = WM_QUIT;
		}

	 public:
		explicit
		MessageQueueIterator(::HWND wnd) noexcept 
		  : Window(wnd)
		{
			this->increment();
		}
	
		satisfies(MessageQueueIterator,
			IsCopyable,
			IsMovable,
			NotSortable
		);

	 private:
		bool 
		equal(const type& r) const {
			return this->Current.message == r.Current.message;
		}

		const Message& 
		dereference() const { 
			return this->Current; 
		}

		void 
		increment() { 
			if (::GetMessage(&this->Current, this->Window, 0, 0) == -1) {
				win::LastError{}.throwAlways("Message pump failed");
			}
		}
	};
}	// namespace core::forms