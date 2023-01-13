#pragma once
#include "formsFramework.h"

namespace core::forms
{
	std::string
	inline to_hexString(std::span<std::byte const> bytes)
	{
		char constinit
		static digits[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	
		std::string buffer(2*bytes.size(), '\0');
		auto out = buffer.begin();
		for (auto const& byte : bytes) {
			*(out++) = digits[static_cast<uint8_t>(byte) / 16];
			*(out++) = digits[static_cast<uint8_t>(byte) % 16];
		}
		return buffer;
	}

	template <unsigned Width = -1, meta::IntegralOrEnum Value>
	auto 
	inline to_hexString(Value const n)
	{
		std::stringstream s; 
		s << "0x" << std::setfill('0') << std::hex;

		if (Width != -1)
			s << std::setw(Width) << static_cast<std::make_unsigned_t<Value>>(n);
		else if (static_cast<unsigned>(n) == 0)
			s << "00";
		else if (static_cast<uint64_t>(n) & 0xFFffFFff00000000) 
			s << std::setw(16) << static_cast<uint64_t>(n);
		else if (static_cast<uint32_t>(n) & 0xff000000) 
			s << std::setw(8) << static_cast<uint32_t>(n);
		else if (static_cast<uint16_t>(n) & 0x0000ff00) 
			s << std::setw(4) << static_cast<uint16_t>(n);
		else 
			s << std::setw(2) << static_cast<uint16_t>(n);
	
		return s.str(); 
	};
}