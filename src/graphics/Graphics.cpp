#include "graphics/Graphics.h"
using namespace core;
using namespace forms;

DeviceContext 
DeviceContext::ScreenDC{SharedDeviceContext{::GetDC(nullptr)}};
