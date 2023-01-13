#include "system/ResourceId.h"
using namespace core;
using namespace forms;

ResourceId const 
forms::ResourceId::Null {};

std::wstring
forms::to_wstring(ResourceId const& res)
{
	if (res.is_numeric())
		return L'#' + std::to_wstring(res.as_number());
	else 
		return std::wstring{res.as_string()};
}
