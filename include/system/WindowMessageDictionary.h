#pragma once
#include "library/core.Forms.h"
#include "system/MessageName.h"

namespace core::forms
{
	struct MessageName 
	{
		uint16_t	Ident;
		gsl::czstring	Name;

		constexpr
		MessageName(uint16_t id, gsl::czstring n) : Ident(id), Name(n)
		{}

		constexpr operator 
		std::pair<uint16_t,gsl::czstring>() const {
			return {this->Ident, this->Name};
		}
	};

	struct MessageProperties : public MessageName 
	{
		::LRESULT	Unhandled = 0xffffffff;
		bool        Common = false;

		MessageProperties(uint16_t id, gsl::czstring n) : MessageName(id,n)
		{}
	};

	class FormsExport WindowMessageDictionary  {
		using Dictionary = std::map<uint16_t, MessageProperties>;

		Dictionary	Storage;

	public:
		WindowMessageDictionary() 
		{
			// Populate dictionary
			for (uint16_t idx = 0; idx < _countof(g_WindowMessageNames); ++idx) {
				this->Storage.emplace(idx, MessageProperties{idx, g_WindowMessageNames[idx]});
			}

			// Special cases
			this->Storage.at(WM_NCCREATE).Unhandled = FALSE;

			// Common messages
			this->Storage.at(WM_NCHITTEST).Common = true;
			this->Storage.at(WM_NCMOUSEMOVE).Common = true;
			this->Storage.at(WM_MOUSEFIRST).Common = true;
			this->Storage.at(WM_SETCURSOR).Common = true;
			this->Storage.at(WM_MOVE).Common = true;
			this->Storage.at(WM_MOVING).Common = true;
			this->Storage.at(WM_WINDOWPOSCHANGED).Common = true;
			this->Storage.at(WM_WINDOWPOSCHANGING).Common = true;
		}

	public:
		bool 
		contains(::UINT id) const {
			return this->Storage.contains(static_cast<uint16_t>(id));
		}
	
		gsl::czstring
		name(::UINT id) const {
			return this->contains(id) ? (*this)[id].Name : "WM_????";
		}

		const MessageProperties&
		operator[](::UINT id) const {
			return this->Storage.at(static_cast<uint16_t>(id));
		}
	};
}