#pragma once
#include "formsFramework.h"
#include "support/DebugOutputBuffer.h"

class DebugStream : public std::ostream 
{
	using base = std::ostream;

	DebugOutputBuffer Buffer;

public:
	DebugStream() : base(&this->Buffer) {
	};
};

std::add_lvalue_reference_t<std::ostream>
inline operator<<(std::ostream& os, const std::wstring& ws) {
	return os << to_string(ws);
}

extern DebugStream cdebug;
