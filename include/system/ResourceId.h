#pragma once
#include "formsFramework.h"

class ResourceId 
{
private:
	using Storage = std::variant<uint16_t, std::wstring>;

private:
	Storage		Ident;
	
public:
	constexpr
	ResourceId() : ResourceId{uint16_t(0)}
	{
	}

	explicit constexpr
	ResourceId(uint16_t numeric) : Ident{numeric}
	{
	}

	explicit 
	ResourceId(std::wstring_view name) : Ident{std::wstring{name}}
	{
	}

	satisfies(ResourceId,
		constexpr_IsCopyable,
		constexpr_IsMovable,
		constexpr IsEqualityComparable,
		NotSortable
	);
	
public:
	ResourceId const 
	static Null;

public:
	ResourceId constexpr
	static parse(wchar_t const* id) 
	{
		return IS_INTRESOURCE(id) 
		  ? ResourceId{static_cast<uint16_t>(reinterpret_cast<uintptr_t>(id))} 
		  : ResourceId{id};
	}

public:
	bool constexpr
	is_numeric() const 
	{
		return this->Ident.index() == 0;
	}

	uint16_t constexpr
	as_number() const 
	{
		return *std::get_if<uint16_t>(&this->Ident);
	}
	
	std::wstring_view constexpr
	as_string() const 
	{
		return *std::get_if<std::wstring>(&this->Ident);
	}
	
	constexpr operator 
	wchar_t const*() const 
	{
		if (auto* numeric = std::get_if<uint16_t>(&this->Ident))
			return MAKEINTRESOURCE(*numeric);
		else 
			return std::get_if<std::wstring>(&this->Ident)->c_str(); 
	}
};

std::wstring
to_wstring(ResourceId const& res);
