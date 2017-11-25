#ifndef LIBRZUINIT_H
#define LIBRZUINIT_H

#include "Extern.h"

typedef void (*ClientConfigInit)();

// configDeclareInitCallback must bind or declare config keys to set their types (if there is no config used in client
// code, set to nullptr)
bool RZU_EXTERN LibRzuInit();
bool RZU_EXTERN LibRzuDeInit();

#endif  // LIBRZUINIT_H
