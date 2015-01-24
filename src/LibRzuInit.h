#ifndef LIBRZUINIT_H
#define LIBRZUINIT_H

#include "LibGlobal.h"

typedef void (*ClientConfigInit)();

//configDeclareInitCallback must bind or declare config keys to set their types (if there is no config used in client code, set to nullptr)
bool LIB_EXTERN LibRzuInit();

#endif // LIBRZUINIT_H
