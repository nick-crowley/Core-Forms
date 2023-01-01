#pragma once

#include "formsFramework.h"

class ConstChildWindowIterator
  : public boost::iterator_facade<ConstChildWindowIterator, const ::HWND, boost::forward_traversal_tag>
{
	friend class boost::iterator_core_access;

	using type = ConstChildWindowIterator;
	using base = boost::iterator_facade<type, const ::HWND, boost::forward_traversal_tag>;
	
	using Storage = std::list<::HWND>;

public:
	static const ConstChildWindowIterator npos;

private:
	std::vector<::HWND>  Children;
	::HWND	  Parent;
	unsigned Index;

 public:
	explicit
    ConstChildWindowIterator(::HWND parent) noexcept 
	  : Parent(parent), Index(0)
	{
		::EnumChildWindows(parent, &on_next_child_window, (::LPARAM)(uintptr_t)this);
		if (this->Children.empty())
			this->Index = -1;
	}

	CopyMovable(ConstChildWindowIterator);

private:
	ConstChildWindowIterator() noexcept
      : Parent(nullptr), Index(-1)
	{}

private:
	static
	::BOOL CALLBACK on_next_child_window(::HWND child, ::LPARAM iterator);

 private:
    bool 
	equal(const type& r) const {
        return (this->Parent == r.Parent || !this->Parent || !r.Parent)
		    && this->Index == r.Index;
    }

    const ::HWND& 
	dereference() const { 
		return this->Children[this->Index]; 
	}

	void 
	increment() { 
		if (++this->Index == this->Children.size()) {
			this->Index = -1;
		}
	}
};

