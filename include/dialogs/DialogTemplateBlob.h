#pragma once
#include "library/core.Forms.h"

namespace core::forms
{
	class DialogTemplateBlob {
	public:
		std::vector<std::byte>	Bytes;

	public:
		explicit
		DialogTemplateBlob(std::vector<std::byte>&& src)
			: Bytes(std::move(src))
		{}

		template <std::input_iterator InputIterator>
		DialogTemplateBlob(InputIterator const start, InputIterator const end) 
			: Bytes(start, end)
		{}

		satisfies(DialogTemplateBlob,
			NotDefaultConstructible,
			IsCopyable,
			IsMovable,
			IsEqualityComparable,
			NotSortable
		);
		
	public:
		implicit operator 
		::DLGTEMPLATE const* () const {
			return reinterpret_cast<::DLGTEMPLATE const*>(this->Bytes.data());
		}
	
		implicit operator 
		std::byte const* () const {
			return this->Bytes.data();
		}

	public:
		implicit operator 
		::DLGTEMPLATE const* () {
			return reinterpret_cast<::DLGTEMPLATE*>(this->Bytes.data());
		}

		implicit operator 
		std::byte const* () {
			return this->Bytes.data();
		}
	};
}