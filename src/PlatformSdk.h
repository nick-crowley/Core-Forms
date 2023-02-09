#pragma once

// Including SDKDDKVer.h defines the highest available Windows platform.
// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.
#include <SDKDDKVer.h>
#include <Windows.h>

// NB: Add library-specific Windows dependencies here
#include <Windowsx.h>
#include <CommCtrl.h>
#include <commoncontrols.h>                       //!< ImageList
#include <vsstyle.h>                              //!< Parts and States
#include <Vssym32.h>                              //!< Properties
#include <Uxtheme.h>
#include "win/LResult.h"
