#include "graphics/Brush.h"
#include "graphics/SystemBrush.h"
#include "graphics/StockBrush.h"
using namespace core;

forms::SharedBrush
forms::Brush::fromAnyColour(AnyColour col) 
{
	if (auto* const sysColour = std::get_if<SystemColour>(&col); sysColour)
		return SystemBrush::get(*sysColour).handle();
	else if (std::holds_alternative<meta::transparent_t>(col))
		return StockBrush::Hollow.handle();
	else if (SharedBrush handle{::CreateSolidBrush(win::DWord{std::get<Colour>(col)})}; !handle)
		win::LastError{}.throwAlways("CreateSolidBrush() failed");
	else
		return handle;
}