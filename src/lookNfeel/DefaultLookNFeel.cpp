#include "lookNfeel/ILookNFeelProvider.h"
#include "lookNfeel/Win31LookNFeel.h"
using namespace core;
using namespace forms;

SharedLookNFeelProvider
forms::lookNFeel = std::make_shared<Win31LookNFeel>();
