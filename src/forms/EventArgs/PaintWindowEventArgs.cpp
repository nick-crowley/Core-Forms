#include "forms/WindowEventArgs.h"
#include "forms/Window.h"
using namespace core;
using namespace forms;

void 
forms::PaintWindowEventArgs::beginPaint() 
{
	if (auto dc = ::BeginPaint(this->Window->handle(), &this->Data)) {
		this->Graphics = DeviceContext{SharedDeviceContext{dc, weakref}};
		this->Area = this->Data.rcPaint;
		this->Erase = this->Data.fErase;
		this->Restore = this->Data.fRestore;
		this->Update = this->Data.fIncUpdate;
	}
}

void 
forms::PaintWindowEventArgs::endPaint() 
{
	if (::EndPaint(this->Window->handle(), &this->Data)) {
		this->Graphics.reset();
		this->Area.reset();
		this->Erase.reset();
		this->Restore.reset();
		this->Update.reset();
	}
}
