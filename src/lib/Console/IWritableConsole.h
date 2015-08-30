#ifndef IWRITABLECONSOLE
#define IWRITABLECONSOLE

#include "LibGlobal.h"

class IWritableConsole
{
public:
	virtual void write(const char* message) = 0;
	virtual void writef(const char* format, ...) = 0 PRINTFCHECK(2, 3);

	virtual void log(const char *message, ...) = 0 PRINTFCHECK(2, 3);
};

#endif // IWRITABLECONSOLE

