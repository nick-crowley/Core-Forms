#pragma once
#include "formsFramework.h"

namespace core::forms
{
	struct Message : public ::MSG {
		using base = ::MSG;
		
		Message() : base({}) 
		{}

		void dispatch() const {
			::DispatchMessage(this);
		}

		void translate() const {
			::TranslateMessage(this);
		}
	};
}