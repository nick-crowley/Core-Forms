#pragma once
#include "formsFramework.h"

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
