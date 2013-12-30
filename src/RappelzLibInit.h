#ifndef RAPPELZLIBINIT_H
#define RAPPELZLIBINIT_H

#include "RappelzLib_global.h"

typedef void (*ClientConfigInit)();

//configDeclareInitCallback must bind or declare config keys to set their types (if there is no config used in client code, set to nullptr)
bool RAPPELZLIB_EXTERN RappelzLibInit(int argc, char **argv, ClientConfigInit configDeclareInitCallback);

#endif // RAPPELZLIBINIT_H
