#pragma once
#include "library/core.Forms.h"
#include "controls/Control.h"

namespace core::forms
{
	class ControlDictionary
	{
		using Dictionary = std::map<uint16_t,Control*>;

	private:
		Dictionary m_items;

	public:
		satisfies(ControlDictionary,
			IsDefaultConstructible,
			IsCopyable,
			IsMovable,
			//IsEqualityComparable,
			NotSortable
		);

		ControlDictionary(std::initializer_list<Control*> controls)
		{
			Invariant(ranges::none_of(controls, lambda(=, const* c, c->ident() == /*IDC_STATIC*/ -1)));
			for (auto* c : controls)
				this->m_items[c->ident()] = c; 
		}

	public:
		Dictionary::const_iterator 
		begin() const { 
			return this->m_items.begin(); 
		}

		Dictionary::const_iterator
		end() const { 
			return this->m_items.end(); 
		}

		bool
		contains(uint16_t id) const {
			return this->m_items.contains(id);
		}
		
		bool
		empty() const {
			return this->m_items.empty();
		}

		Dictionary::size_type
		size() const { 
			return this->m_items.size();  
		}
		
		Control*
		operator[](uint16_t id) const {
			return this->m_items.at(id);
		}
		
		ControlDictionary
		operator+(ControlDictionary const& other) const {
			return ControlDictionary{*this} += other;
		}

	public:
		ControlDictionary&
		operator+=(ControlDictionary const& other) {
			for (auto kvp : other.m_items)
				this->m_items[kvp.first] = kvp.second;
			return *this;
		}
	};
}