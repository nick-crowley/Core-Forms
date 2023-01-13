#pragma once
#include "system/MessageName.h"

namespace core::forms
{
	class MessageDictionary : public std::map<::UINT, const char*> {
		using base = std::map<::UINT, const char*>;

	public:
		MessageDictionary() = default;

		template <typename InputIterator>
		MessageDictionary(InputIterator b, InputIterator e) : base(b,e) {
			/*for (const MessageName& m : _MessageNames) {
				this->emplace(m.Message, m.Name);
			}*/
		}
	
		MessageDictionary(std::initializer_list<value_type> init) : base(init) {
		}
	};
}