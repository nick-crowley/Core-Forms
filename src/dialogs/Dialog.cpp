#include "dialogs/Dialog.h"
using namespace core;
using namespace forms;

std::optional<Dialog::CreateWindowParameter>
thread_local Dialog::s_DialogCreationParameter {};
