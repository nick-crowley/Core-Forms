#include "system/ResourceId.h"
#include "support/to_string.h"
using namespace core;
using namespace forms;

ResourceId const 
forms::ResourceId::Null {};

std::string
forms::to_string(ResourceId const& res)
{
	if (res.is_numeric())
		return '#' + std::to_string(res.as_number());
	else 
		return forms::to_string(res.as_string());
}

std::wstring
forms::to_wstring(ResourceId const& res)
{
	if (res.is_numeric())
		return L'#' + std::to_wstring(res.as_number());
	else 
		return std::wstring{res.as_string()};
}
