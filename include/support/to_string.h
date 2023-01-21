#pragma once
#include "library/core.Forms.h"

namespace core::forms
{
	namespace detail {
		template <typename Output>
		auto inline to_static_cast = [](auto&& value) { return static_cast<Output>(std::forward<decltype(value)>(value)); };

		inline const auto to_narrow = to_static_cast<char>; // [](auto& wc) { return static_cast<char>(wc); };
		inline const auto to_wide = to_static_cast<wchar_t>; // [](auto& c) { return static_cast<wchar_t>(c); };
	}

	std::string 
	inline to_string(std::wstring const& ws) {
		return { 
			boost::make_transform_iterator(ws.begin(), detail::to_narrow),
			boost::make_transform_iterator(ws.end(), detail::to_narrow)
		};
	}

	std::string 
	inline to_string(std::wstring_view const& wsv) {
		return { 
			boost::make_transform_iterator(wsv.begin(), detail::to_narrow),
			boost::make_transform_iterator(wsv.end(), detail::to_narrow)
		};
	}

	std::string 
	inline to_string(gsl::cwzstring const ws) {
		return { 
			boost::make_transform_iterator(ws, detail::to_narrow),
			boost::make_transform_iterator(ws + wcslen(ws), detail::to_narrow)
		};
	}

	std::wstring 
	inline to_wstring(std::string const& s) {
		return { s.begin(), s.end() };
	}

	std::wstring 
	inline to_wstring(std::string_view const& sv) {
		return { sv.begin(), sv.end() };
	}

	std::wstring 
	inline to_wstring(gsl::czstring const s) {
		return { &s[0], &s[std::strlen(s)] };
	}
}