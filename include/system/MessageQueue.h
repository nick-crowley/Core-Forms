#pragma once
#include "system/MessageQueueIterator.h"

namespace core::forms
{
	struct MessageQueue
	{
		MessageQueue() = default;

		MessageQueueIterator
		begin() const {
			return MessageQueueIterator{nullptr};
		}

		MessageQueueIterator
		end() const {
			return MessageQueueIterator::npos;
		}
	};
} // namespace core::forms