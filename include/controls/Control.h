#pragma once
#include "library/core.Forms.h"
#include "system/MessageDictionary.h"
#include "windows/Window.h"

namespace core::forms 
{
	class Control : public Window
	{
		uint16_t const  Ident;

	public:
		Control(uint16_t id)
		  : Ident{id}
		{}
		
	public:
		uint16_t
		ident() const {
			return this->Ident;
		}
	
	protected:
		gsl::czstring
		virtual notificationName(::UINT notification) abstract;
	};
}
