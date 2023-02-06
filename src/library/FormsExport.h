#pragma once
#if defined(BuildCoreFormsStaticLib)
#	define FormsExport 
#elif defined(BuildCoreFormsDLL)
#	define FormsExport __declspec(dllexport)
#else
#	define FormsExport __declspec(dllimport)
#endif