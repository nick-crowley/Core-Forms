#include "system/ResourceId.h"

ResourceId const 
ResourceId::Null {};

std::wstring
to_wstring(ResourceId const& res)
{
	if (res.is_numeric())
		return L'#' + std::to_wstring(res.as_number());
	else 
		return std::wstring{res.as_string()};
}
