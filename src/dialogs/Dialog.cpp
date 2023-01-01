#include "dialogs/Dialog.h"

std::optional<Dialog::CreateWindowParameter>
thread_local Dialog::s_DialogCreationParameter {};
