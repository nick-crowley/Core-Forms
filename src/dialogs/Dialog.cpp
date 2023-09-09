#include "dialogs/Dialog.h"
using namespace core;
using namespace forms;

std::optional<Dialog::CreationData>
/*thread_local*/ Dialog::DialogCreationParameter {};
