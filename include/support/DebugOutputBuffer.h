#pragma once
#include "formsFramework.h"

namespace core::forms
{
	class DebugOutputBuffer : public std::streambuf {
		std::vector<char>	Buffer;

	public:
		DebugOutputBuffer() : Buffer(256) {
			this->setp(this->Buffer.data(), this->Buffer.data() + 255); //-1 to make overflow() easier
		}

		satisfies(DebugOutputBuffer,
			NotCopyable
		);

	private:
		int_type overflow(int_type ch) override {
			*this->pptr() = ch;
			this->pbump(1);
			this->sync();

			return ch;
		}

		int sync() override {
			::OutputDebugStringA(this->Buffer.data());
		
			ptrdiff_t len = this->pptr() - this->pbase();
			this->pbump(static_cast<int>(-len));

			for (unsigned idx = 0; idx < 256; ++idx) 
				this->Buffer[idx] = '\0';
			//this->Buffer.clear();
			return 0;
		}
	};
}