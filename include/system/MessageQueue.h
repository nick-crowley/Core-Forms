#pragma once

#include "system/MessageQueueIterator.h"

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
