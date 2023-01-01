#pragma once
#include "formsFramework.h"

class DialogTemplateBlob {
public:
	std::vector<std::byte>	Bytes;

public:
	explicit
	DialogTemplateBlob(std::vector<std::byte>&& src)
		: Bytes(std::move(src))
	{}

	template <typename InputIterator>
	DialogTemplateBlob(InputIterator start, InputIterator end) 
		: Bytes(start, end)
	{}

public:
	operator const DLGTEMPLATE* () const {
		return reinterpret_cast<const ::DLGTEMPLATE*>(this->Bytes.data());
	}
	
	operator const std::byte* () const {
		return this->Bytes.data();
	}

	operator DLGTEMPLATE* () {
		return reinterpret_cast<::DLGTEMPLATE*>(this->Bytes.data());
	}

	operator std::byte* () {
		return this->Bytes.data();
	}
};
