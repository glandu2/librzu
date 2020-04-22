#pragma once

#include "LibGlobal.h"

class IWritableConsole {
public:
	virtual void write(const char* message) = 0;
	virtual void writef(const char* format, ...) PRINTFCHECK(2, 3) = 0;

	virtual void log(const char* message, ...) PRINTFCHECK(2, 3) = 0;
};

