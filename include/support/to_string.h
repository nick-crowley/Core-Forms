#pragma once
#include "library/core.Forms.h"
#include "nstd/Functional.h"

namespace core::forms
{
	std::string 
	inline to_string(std::wstring const& ws) {
		return { 
			boost::make_transform_iterator(ws.begin(), nstd::convert_to<char>),
			boost::make_transform_iterator(ws.end(), nstd::convert_to<char>)
		};
	}

	std::string 
	inline to_string(std::wstring_view const& wsv) {
		return { 
			boost::make_transform_iterator(wsv.begin(), nstd::convert_to<char>),
			boost::make_transform_iterator(wsv.end(), nstd::convert_to<char>)
		};
	}

	std::string 
	inline to_string(gsl::cwzstring const ws) {
		return { 
			boost::make_transform_iterator(ws, nstd::convert_to<char>),
			boost::make_transform_iterator(ws + wcslen(ws), nstd::convert_to<char>)
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