#include "system/ResourceId.h"
#include "core/ToString.h"
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
		return as_string(res.as_string());
}
