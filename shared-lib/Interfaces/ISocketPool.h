#ifndef ISOCKETPOOL_H
#define ISOCKETPOOL_H

#include "IObject.h"

class ISocket;

class ISocketPool : public IObject
{
public:
	virtual void IFACECALLCONV addSocket(ISocket* socket) = 0;
	virtual void IFACECALLCONV removeSocket(ISocket* socket) = 0;

	virtual void IFACECALLCONV run() = 0;
	virtual void IFACECALLCONV stop() = 0;
};

#endif // ISOCKETPOOL_H
