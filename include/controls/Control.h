#pragma once
#include "library/core.Forms.h"
#include "system/MessageDictionary.h"
#include "forms/Window.h"

namespace core::forms 
{
	class Control : public Window
	{
		uint16_t const  Ident;

	public:
		implicit
		Control(uint16_t id)
		  : Ident{id}
		{}
		
	public:
		uint16_t
		ident() const {
			return this->Ident;
		}
		
		bool
		virtual ownerDraw() const abstract;
		
	protected:
		gsl::czstring
		virtual notificationName(::UINT notification) abstract;
	};
}
